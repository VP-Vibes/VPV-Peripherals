/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "qspi.h"
#include "gen/Apb3SpiXdrMasterCtrl_regs.h"
#include <scc/report.h>
#include <scc/utilities.h>
#include <limits>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace sc_dt;

qspi::qspi(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, Apb3SpiXdrMasterCtrl_regs)
{
    regs->registerResources(*this);
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    regs->status.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time d) -> bool {
        regs->r_status.tx_free=32;
        return false;
    });
    //TODO write to irq
}

qspi::~qspi() = default;

void qspi::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
    for(auto i=0U; i<32; ++i){
        dq_o[i].write(false);
        oe_o[i].write(false);
    }
}
} /* namespace minres */
} /* namespace vpvper */
