/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "timer.h"
#include "gen/timer.h"

#include "scc/utilities.h"

#include "util.h"

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(timer);// NOLINT

timer::timer(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::timer_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;

  // CFG_LO register
  regs->CFG_LO.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->CFG_LO.set_write_cb(vpvper::pulpissimo::simple_write);

  // CFG_HI register
  regs->CFG_HI.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->CFG_HI.set_write_cb(vpvper::pulpissimo::simple_write);
}

timer::~timer() {} // NOLINT

void timer::clock_cb() { this->clk = clk_i.read(); }

void timer::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} /* namespace pulpissimo */
