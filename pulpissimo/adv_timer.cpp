/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "adv_timer.h"
#include "gen/adv_timer.h"

#include "scc/utilities.h"

namespace pulpissimo {
SC_HAS_PROCESS(adv_timer);// NOLINT

adv_timer::adv_timer(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::adv_timer_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
}

adv_timer::~adv_timer() {} // NOLINT

void adv_timer::clock_cb() { this->clk = clk_i.read(); }

void adv_timer::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} /* namespace pulpissimo */
