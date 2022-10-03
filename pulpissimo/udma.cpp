/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "udma.h"

#include "gen/udma.h"
#include "scc/utilities.h"

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(udma);  // NOLINT

udma::udma(sc_core::sc_module_name nm) : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::udma_regs) {
  regs->registerResources(*this);

  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  /*
    regs->i_uart.STATUS.set_write_cb(
        [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
    regs->i_uart.VALID.set_write_cb(
        [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
    regs->i_uart.DATA.set_write_cb(
        [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });
  */

  regs->i_spi.SPIM_RX_SADDR.set_write_cb(
      [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool {
        std::cout << "UDMA-SPIM-SPIM_RX_SADDR: hello world - accessed properly!\n";
        return true;
      });
}

udma::~udma() {}  // NOLINT

void udma::clock_cb() { this->clk = clk_i.read(); }

void udma::reset_cb() {
  if (rst_i.read()) {
    regs->reset_start();
  } else {
    regs->reset_stop();
  }
}

}  // namespace vpvper::pulpissimo
