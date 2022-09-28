/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "irq.h"
#include "gen/irq_regs.h"
#include <scc/report.h>
#include <scc/utilities.h>
#include <limits>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace sc_dt;

SC_HAS_PROCESS(irq);// NOLINT

irq::irq(sc_core::sc_module_name nm, unsigned irq_width)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, pending_irq_i("pending_irq_i", irq_width)
, regs(scc::make_unique<irq_regs>("regs"))
{
    regs->registerResources(*this);
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    SC_METHOD(update_irqs);
    for(auto& pin: pending_irq_i)
        sensitive<<pin.pos();

    regs->ip.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        regs->r_ip = data;
        update_irqs();
        return true;
    });
    regs->ie.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        regs->r_ip = data;
        update_irqs();
        return true;
    });
}

irq::~irq() = default;

void irq::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
    irq_o.write(false);
}

void irq::update_irqs() {
    unsigned mask = 1;
    for(unsigned i=0; i<pending_irq_i.size(); ++i, mask<<=1) {
        if(pending_irq_i[i].read()) regs->r_ip |= mask;
    }
    irq_o.write(regs->r_ip&regs->r_ie);
}


} /* namespace minres */
} /* namespace vpvper */
