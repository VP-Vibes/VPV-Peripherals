/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:47 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#include "ethmac.h"
#include "gen/ethmac_regs.h"

#include <scc/utilities.h>

namespace vpvper {
namespace minres {

SC_HAS_PROCESS(ethmac); // NOLINT

ethmac::ethmac(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::ethmac_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    // callback functions to enable readonly/writeonly functionality
    regs->tx_avail_reg.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
    regs->rx_data_reg.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
    regs->rx_stat_reg.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
}

ethmac::~ethmac() = default; // NOLINT

void ethmac::clock_cb() { this->clk = clk_i.read(); }

void ethmac::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

} // namespace minres
} // namespace vpvper