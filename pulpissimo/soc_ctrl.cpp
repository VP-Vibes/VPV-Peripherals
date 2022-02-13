/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc_ctrl.h"
#include "gen/soc_ctrl.h"

#include <scc/utilities.h>

namespace pulpissimo {
SC_HAS_PROCESS(soc_ctrl);// NOLINT

soc_ctrl::soc_ctrl(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::soc_ctrl_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;

    auto ro_wr_cb = [this](scc::sc_register<uint32_t>&, uint32_t const & v, sc_core::sc_time t)-> bool {return true;};
    regs->INFO.set_write_cb(ro_wr_cb);
    regs->INFO.set_read_cb([this](scc::sc_register<gen::soc_ctrl_regs::INFO_t> const & reg, uint32_t& v, sc_core::sc_time t)-> bool {
        gen::soc_ctrl_regs::INFO_t st = reg.get();
        st.NumOfCores = 1;
        st.NumOfCluster=1;
        reg = st;
        return false;
    });
}

soc_ctrl::~soc_ctrl() {} // NOLINT

void soc_ctrl::clock_cb() { this->clk = clk_i.read(); }

void soc_ctrl::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} /* namespace pulpissimo */
