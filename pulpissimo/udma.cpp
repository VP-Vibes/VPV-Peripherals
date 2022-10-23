/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "udma.h"

#include "gen/udma.h"
#include "scc/utilities.h"

// TODO: chip select logic -> what chip select is live and associated error handling

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(udma);  // NOLINT

udma::udma(sc_core::sc_module_name nm, l2mem_t *l2_mem)
    : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::udma_regs), l2_mem_{l2_mem} {
  regs->registerResources(*this);

  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  // CTRL_CFG_CG register
  regs->CTRL_CFG_CG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->CTRL_CFG_CG.set_write_cb(vpvper::pulpissimo::simple_write);

  spim_.spim_regs_cb();
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

udma::SPIM::SPIM(gen::spi_channel_regs *regs, l2mem_t *l2_mem) : regs_{regs}, l2_mem_{l2_mem} {}

void udma::SPIM::spim_regs_cb() {
  // SPIM_RX_SADDR register
  // right now the UDMA moel is instantaneous i.e. the whole txn data is received atomically from external device
  // hence this reg should always be 0 (possibly by some other part of the model)
  regs_->SPIM_RX_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_RX_SADDR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v >= 0x1c000000 && v < 0x1c080000) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_RX_SIZE register
  // similar to above read_cb of RX_SPIM_SADDR, we expect to return 0 as 'size left' of rx-buffer here
  regs_->SPIM_RX_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_RX_SIZE.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v <= 1048576) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_RX_CFG register
  regs_->SPIM_RX_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_RX_CFG.set_write_cb(vpvper::pulpissimo::simple_write);

  // SPIM_CMD_SADDR register
  regs_->SPIM_CMD_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_CMD_SADDR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v >= 0x1c000000 && v < 0x1c080000) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_CMD_SIZE register
  regs_->SPIM_CMD_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_CMD_SIZE.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v <= 1048576) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_CMD_CFG register
  regs_->SPIM_CMD_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_CMD_CFG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) {
    // EN | PENDING | CLR | DATASIZE | CONTINOUS
    reg.put(v);

    current_cfg_ = regs_->SPIM_CMD_CFG.get();
    bool status{false};

    // // make sure that command configurations programmed by CPU are valid before inspecting the CMD buffer
    // status = isCMDCFGOk();
    // if (!status) {
    //   return false;
    // }

    // this is modeled at a very high level such that the SPI transactions to external devices are
    // instantaneous i.e. happen at the same simulation tick
    // this makes CMD CFG register kind of useless e.g. CLR does not make sense as data can never be
    // outstanding
    // so right now just checking if there is new transfer request or not
    if (!current_cfg_.EN) {
      return true;
    }

    // now inspecting the command buffer to update state
    status = handleCommands();
    if (!status) {
      return false;
    }

    return true;
  });
}

// bool udma::SPIM::isCMDCFGOk() {
//   // printCMDCFG();
//
//   // queue a transfer if one is already ongoing
//
//   return true;
// }

void udma::SPIM::printCMDCFG() {
  std::cout << "EN = " << current_cfg_.EN << "\t";
  std::cout << "PENDING = " << current_cfg_.PENDING << "\t";
  std::cout << "CLR = " << current_cfg_.CLR << "\t";
  std::cout << "DATASIZE = " << current_cfg_.DATASIZE << "\t";
  std::cout << "CONTINOUS = " << current_cfg_.CONTINOUS << "\n";
}

int udma::SPIM::handleCommands() {
  uint32_t cmd_buffer_baseaddr{regs_->SPIM_CMD_SADDR};
  auto cmd{std::make_unique<uint32_t[]>(regs_->SPIM_CMD_SIZE / 4)};

  // loading commands from memory:
  // creating at TLM transaction
  sc_core::sc_time delay{sc_core::SC_ZERO_TIME};

  tlm::tlm_generic_payload gp{};
  gp.set_command(tlm::TLM_READ_COMMAND);
  gp.set_address(cmd_buffer_baseaddr - kL2MemBaseAddr);
  gp.set_data_ptr(reinterpret_cast<unsigned char *>(cmd.get()));
  gp.set_data_length(regs_->SPIM_CMD_SIZE);
  gp.set_streaming_width(regs_->SPIM_CMD_SIZE);

  l2_mem_->handle_operation(gp, delay);

  // decoding the commands
  for (int i = 0; i < regs_->SPIM_CMD_SIZE / 4; ++i) {
    auto opcode{cmd[i] >> 28 & 0xf};
    switch (opcode) {
      case 0x0:
        // SPI_CMD_CFG : sets the configuration for SPIM IP
        // not relevent for us as this is HW info
        break;
      case 0x1:
        // SPI_CMD_SOT : sets the chip select (CS)
        chip_select_ = cmd[i] & 0x3;
        transfer_started_ = true;
        break;
      case 0x7:
        // SPI_CMD_RX_DATA : receive data (max 256 Kb)
        if (!transfer_started_) {
          return false;
        }

        std::cout << "got here\n";
        exit(1);

        break;
      case 0x9:
        // SPI_CMD_EOT : clears the chip select (CS)
        transfer_started_ = false;
        break;
    }
  }

  return true;
}

}  // namespace vpvper::pulpissimo
