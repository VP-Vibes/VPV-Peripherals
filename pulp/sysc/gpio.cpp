/*
 * Copyright (c) 2019 -2022 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio.h"
#include "gen/gpio.h"

#include "scc/utilities.h"

namespace sysc {
SC_HAS_PROCESS(gpio);// NOLINT

gpio::gpio(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMED(clk_i)
, NAMED(rst_i)
, NAMEDD(regs, gen::gpio_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
}

gpio::~gpio() {} // NOLINT

void gpio::clock_cb() { this->clk = clk_i.read(); }

void gpio::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} /* namespace sysc */
