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

  spim_regs_cb();
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

////////////
// the callback functions associated with SPI-M registers
///////////
// as per datasheet, the processor (CPU) prepares the stream of uDMA commands at an appropriate memory location.
// Then it configures the command channels and the TX/RX channels accordingly.
void udma::spim_regs_cb() {
  // the write callback for SPIM_RX_SADDR register
  // Reg Description: RX SPI uDMA transfer address of associated buffer
  //
  //
  // returning false:
  // -
  regs->i_spi.SPIM_RX_SADDR.set_write_cb(
      [this](scc::sc_register<uint32_t> &, uint32_t const &v, sc_core::sc_time t) -> bool {
        std::cout << "UDMA-SPIM-SPIM_RX_SADDR: hello world - accessed properly!\n";
        return true;
      });
}

//////////
// the callback functions associated with I2S registers
//////////

}  // namespace vpvper::pulpissimo
