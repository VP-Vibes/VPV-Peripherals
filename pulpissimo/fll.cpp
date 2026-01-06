/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "fll.h"
#include "gen/fll.h"

#include "scc/utilities.h"

namespace pulpissimo {
SC_HAS_PROCESS(fll); // NOLINT

fll::fll(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::fll_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;

    auto ro_wr_cb = [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; };
    regs->STATUS.set_write_cb(ro_wr_cb);
    regs->STATUS.set_read_cb([this](scc::sc_register<gen::fll_regs::STATUS_t> const& reg, uint32_t& v, sc_core::sc_time t) -> bool {
        gen::fll_regs::STATUS_t st = reg.get();
        st.MF = regs->r_CFG1.MFN;
        regs->r_STATUS = st;
        return false;
    });
}

fll::~fll() {} // NOLINT

void fll::clock_cb() { this->clk = clk_i.read(); }

void fll::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} /* namespace pulpissimo */
