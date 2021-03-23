/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "aon.h"
#include "gen/aon_regs.h"

#include "scc/utilities.h"

namespace vpvper {
namespace sifive {

aon::aon(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, NAMED(clk_i)
, NAMED(erst_n_i)
, NAMED(lfclkc_o)
, NAMED(rst_o)
, NAMEDD(regs, aon_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << erst_n_i;
}

void aon::start_of_simulation() { rst_o = true; }

void aon::clock_cb() { this->clk = clk_i.read(); }

aon::~aon() {} // NOLINT

void aon::reset_cb() {
    if (!erst_n_i.read()) {
        regs->reset_start();
        rst_o = true;
    } else {
        regs->reset_stop();
        rst_o = false;
    }
    lfclkc_o.write(sc_core::sc_time(1 / 32768., sc_core::SC_SEC));
}

void aon::reset_internal_cb() {}

} /* namespace sifive */
} /* namespace vpvper */
