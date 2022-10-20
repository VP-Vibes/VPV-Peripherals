/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "udma.h"

#include "gen/udma.h"
#include "scc/utilities.h"
#include "util.h"

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(udma);  // NOLINT

udma::udma(sc_core::sc_module_name nm, scc::memory<512_kB, 32> *l2_mem)
    : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::udma_regs), l2_mem_{l2_mem} {
  regs->registerResources(*this);

  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  // CTRL_CFG_CG register
  regs->CTRL_CFG_CG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->CTRL_CFG_CG.set_write_cb(vpvper::pulpissimo::simple_write);

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
void udma::spim_regs_cb() {
  // SPIM_RX_SADDR register
  // right now the UDMA moel is instantaneous i.e. the whole txn data is received atomically from external device
  // hence this reg should always be 0 (possibly by some other part of the model)
  regs->i_spi.SPIM_RX_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_RX_SADDR.set_write_cb(
      [this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
        if (v >= 0x1c000000 && v < 0x1c080000) {
          reg.put(v);
          return true;
        } else {
          return false;
        }
      });

  // SPIM_RX_SIZE register
  // similar to above read_cb of RX_SPIM_SADDR, we expect to return 0 as 'size left' of rx-buffer here
  regs->i_spi.SPIM_RX_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_RX_SIZE.set_write_cb(
      [this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
        if (v <= 1048576) {
          reg.put(v);
          return true;
        } else {
          return false;
        }
      });

  // SPIM_RX_CFG register
  regs->i_spi.SPIM_RX_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_RX_CFG.set_write_cb(vpvper::pulpissimo::simple_write);

  // SPIM_CMD_SADDR register
  regs->i_spi.SPIM_CMD_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_CMD_SADDR.set_write_cb(
      [this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
        if (v >= 0x1c000000 && v < 0x1c080000) {
          reg.put(v);
          return true;
        } else {
          return false;
        }
      });

  // SPIM_CMD_SIZE register
  regs->i_spi.SPIM_CMD_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_CMD_SIZE.set_write_cb(
      [this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
        if (v <= 1048576) {
          reg.put(v);
          return true;
        } else {
          return false;
        }
      });

  // SPIM_CMD_CFG register
  regs->i_spi.SPIM_CMD_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->i_spi.SPIM_CMD_CFG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) {
    std::cout << "checking SPI commands\n";
    uint32_t cmd_buffer_addr{regs->i_spi.SPIM_CMD_SADDR.get()};
    for (size_t i = 0; i < regs->i_spi.SPIM_CMD_SIZE / 4; ++i) {
      // get data from vp memory via tlm payload
      cmd_buffer_addr += 4;
    }
    exit(1);

    reg.put(v);
    return true;
  });
}

//////////
// the callback functions associated with I2S registers
//////////

}  // namespace vpvper::pulpissimo
