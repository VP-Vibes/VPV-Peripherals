/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio.h"
#include "gen/gpio_regs.h"
#include <limits>
#include <scc/report.h>
#include <scc/utilities.h>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace sc_dt;

gpio::gpio(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, gpio_regs) {
    regs->registerResources(*this);
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    regs->value.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        data = 0;
        for(auto i = 0U; i < 32; ++i) {
            if(pins_i[i].read())
                data |= 1 << i;
        }
        return true;
    });
    regs->write.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        for(auto i = 0U; i < 32; ++i)
            pins_o[i].write(data & (1 << i));
        return true;
    });
    regs->writeEnable.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        for(auto i = 0U; i < 32; ++i)
            oe_o[i].write(data & (1 << i));
        return true;
    });
    regs->boot_sel.set_read_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
            regs->r_boot_sel.bootSel = boot_sel.get_value();
            data = regs->r_boot_sel;
            return true;
        });
}

gpio::~gpio() = default;

void gpio::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
    for(auto i = 0U; i < 32; ++i) {
        pins_o[i].write(false);
        oe_o[i].write(false);
    }
}
} /* namespace minres */
} /* namespace vpvper */
