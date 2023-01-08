/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "udma.h"

#include "gen/udma.h"
#include "scc/utilities.h"

// TODO: chip select logic -> what chip select is live and associated error handling
// TODO: SPIM events RX, TX, CMD, EOT

namespace vpvper::pulpissimo {

udma::udma(sc_core::sc_module_name nm, SoC *soc)
    : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::udma_regs), soc_{soc} {
  regs->registerResources(*this);

  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  // CTRL_CFG_CG register
  regs->CTRL_CFG_CG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->CTRL_CFG_CG.set_write_cb(vpvper::pulpissimo::simple_write);

  spim_.regs_cb();
  i2s_.regs_cb();
}

void udma::clock_cb() { this->clk = clk_i.read(); }

void udma::reset_cb() {
  if (rst_i.read()) {
    regs->reset_start();
  } else {
    regs->reset_stop();
  }
}

udma::PeriphBase::PeriphBase(sc_core::sc_module_name name, gen::udma_regs *udma_regs, SoC *soc)
    : sc_core::sc_module{name}, udma_regs_{udma_regs}, soc_{soc} {}

udma::SPIM::SPIM(gen::udma_regs *udma_regs, SoC *soc)
    : udma::PeriphBase{sc_core::sc_module_name{"udma-spim"}, udma_regs, soc} {
  SC_THREAD(notifyRxEventGenerator);
  sensitive << rx_eot_events_;
  SC_THREAD(notifyTxEventGenerator);
  sensitive << tx_eot_events_;

  regs_ = &udma_regs->i_spi;
}

void udma::SPIM::regs_cb() {
  // SPIM_RX_SADDR register
  // TODO: implement below functionality
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
  // TODO: implement below functionality
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
  regs_->SPIM_RX_CFG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    reg.put(v);

    gen::spi_channel_regs::SPIM_RX_CFG_t rx_cfg{regs_->SPIM_RX_CFG.get()};
    if (rx_cfg.EN == 1) {
      rx_initiated_ = true;
    }

    if (cmd_initiated_ == true) {
      int status = handleCommands();
      if (!status) {
        return false;
      }

      cmd_initiated_ = false;
      rx_initiated_ = false;
    }

    return true;
  });

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

    current_cmd_cfg_ = regs_->SPIM_CMD_CFG.get();
    bool status{false};

    // // make sure that command configurations programmed by CPU are valid before inspecting the CMD buffer
    // status = isCMDCFGOk();
    // if (!status) {
    //   return false;
    // }

    // TODO: implement below functionality
    // this is modeled at a very high level such that the SPI transactions to external devices are
    // instantaneous i.e. happen at the same simulation tick
    // this makes CMD CFG register kind of useless e.g. CLR does not make sense as data can never be
    // outstanding
    // so right now just checking if there is new transfer request or not
    if (!current_cmd_cfg_.EN) {
      return true;
    }

    cmd_initiated_ = true;

    // if TX,RX channels are already configured then we can already handle commands here
    if (tx_initiated_ || rx_initiated_) {
      // now inspecting the command buffer to update state
      status = handleCommands();
      if (!status) {
        return false;
      }

      tx_initiated_ = false;
      rx_initiated_ = false;
      cmd_initiated_ = false;
    }

    return true;
  });

  // SPIM_TX_SADDR register
  regs_->SPIM_TX_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_TX_SADDR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v >= 0x1c000000 && v < 0x1c080000) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_TX_SIZE register
  regs_->SPIM_TX_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_TX_SIZE.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v <= 1048576) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  // SPIM_TX_CFG register
  regs_->SPIM_TX_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->SPIM_TX_CFG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    reg.put(v);

    gen::spi_channel_regs::SPIM_TX_CFG_t tx_cfg{regs_->SPIM_TX_CFG.get()};
    if (tx_cfg.EN == 1) {
      tx_initiated_ = true;
    }

    if (cmd_initiated_ == true) {
      int status = handleCommands();
      if (!status) {
        return false;
      }

      cmd_initiated_ = false;
      tx_initiated_ = false;
    }

    return true;
  });
}

void udma::I2S::regs_cb() {
  regs_->I2S_RX_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_RX_SADDR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v >= 0x1c000000 && v < 0x1c080000) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  regs_->I2S_RX_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_RX_SIZE.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v <= 1048576) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  regs_->I2S_RX_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_RX_CFG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    reg.put(v);

    gen::i2s_channel_regs::I2S_RX_CFG_t rx_cfg{regs_->I2S_RX_CFG.get()};
    if (rx_cfg.EN == 1) {
      rx_eot_events_.notify(kRxEoTDelay);
    }

    return true;
  });

  regs_->I2S_TX_SADDR.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_TX_SADDR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v >= 0x1c000000 && v < 0x1c080000) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  regs_->I2S_TX_SIZE.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_TX_SIZE.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    if (v <= 1048576) {
      reg.put(v);
      return true;
    } else {
      return false;
    }
  });

  regs_->I2S_TX_CFG.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_TX_CFG.set_write_cb(vpvper::pulpissimo::simple_write);

  // I2S_CLKCFG_SETUP register
  // clock configuration for i2s-master, i2s-slave, i2s-pdm
  regs_->I2S_CLKCFG_SETUP.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_CLKCFG_SETUP.set_write_cb(vpvper::pulpissimo::simple_write);

  // I2S_SLV_SETUP register
  // configuration of I2S slave
  regs_->I2S_SLV_SETUP.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_SLV_SETUP.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    reg.put(v);

    return true;
  });

  // I2S_MST_SETUP register
  // configuration of I2S master
  regs_->I2S_MST_SETUP.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_MST_SETUP.set_write_cb(vpvper::pulpissimo::simple_write);

  // I2S_PDM_SETUP
  // configuration of PDM module
  regs_->I2S_PDM_SETUP.set_read_cb(vpvper::pulpissimo::simple_read);
  regs_->I2S_PDM_SETUP.set_write_cb(vpvper::pulpissimo::simple_write);
}

void udma::SPIM::printCMDCFG() {
  std::cout << "EN = " << current_cmd_cfg_.EN << "\t";
  std::cout << "PENDING = " << current_cmd_cfg_.PENDING << "\t";
  std::cout << "CLR = " << current_cmd_cfg_.CLR << "\t";
  std::cout << "DATASIZE = " << current_cmd_cfg_.DATASIZE << "\t";
  std::cout << "CONTINOUS = " << current_cmd_cfg_.CONTINOUS << "\n";
}

int udma::SPIM::handleCommands() {
  uint32_t cmd_buffer_baseaddr{regs_->SPIM_CMD_SADDR};
  auto cmd{std::make_unique<uint32_t[]>(regs_->SPIM_CMD_SIZE / 4)};

  // loading commands from memory
  soc_->readMemory(reinterpret_cast<unsigned char *>(cmd.get()), cmd_buffer_baseaddr - kL2MemBaseAddr,
                   regs_->SPIM_CMD_SIZE);

  // decoding the commands
  for (int i = 0; i < regs_->SPIM_CMD_SIZE / 4; ++i) {
    auto opcode{cmd[i] >> 28 & 0xf};

    switch (opcode) {
      case 0x0: {
        // SPI_CMD_CFG : sets the configuration for SPIM IP
        // not relevent for us as this is HW info
        break;
      }

      case 0x1: {
        // SPI_CMD_SOT : sets the chip select (CS)
        chip_select_ = cmd[i] & 0x3;

        // SPIM CG should have been enabled by this time
        if ((udma_regs_->CTRL_CFG_CG.get() >> 1) & 0x1) {
          transfer_started_ = true;
        }

        break;
      }

      case 0x7: {
        // SPI_CMD_RX_DATA : receive data (max 256 Kb)
        if (!transfer_started_) {
          return false;
        }

        auto qspi{(cmd[i] >> 27) & 0x1};
        auto lsb{cmd[i] >> 26 & 0x1};
        // auto words_per_transfer{cmd[i] >> 21 & 0x3};
        size_t words_per_transfer{0};
        switch (cmd[i] >> 21 & 0x3) {
          case 0: {
            words_per_transfer = 1;
            break;
          }
          case 1: {
            words_per_transfer = 2;
            break;
          }
          case 2: {
            words_per_transfer = 4;
            break;
          }
          case 3: {
            return false;
          }
        }
        // in bytes
        auto words_size{((cmd[i] >> 16 & 0x1f) + 1) / 8};
        auto words_num{(cmd[i] & 0xffff) + 1};
        auto l2mem_data{std::make_unique<unsigned char[]>(words_size * words_num)};

        auto num_transfers{words_num / words_per_transfer};
        for (int i = 0; i < num_transfers; ++i) {
          sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
          tlm::tlm_generic_payload gp{};
          gp.set_command(tlm::TLM_READ_COMMAND);
          gp.set_data_length(words_per_transfer * words_size);
          gp.set_data_ptr(l2mem_data.get() + i * words_per_transfer * words_size);

          soc_->transmitSPIMSocket(chip_select_, gp, delay);
          if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
            return false;
          }
        }

        // send data to l2mem
        // TODO: lsb stuff??
        soc_->writeMemory(l2mem_data.get(), regs_->SPIM_RX_SADDR.get() - kL2MemBaseAddr, words_num * words_size);
        rx_eot_events_.notify(kRxEoTDelay);

        break;
      }

      case 0x6: {
        // SPIM_CMD_TX_DATA : sends data (max 256 Kb)
        if (!transfer_started_) {
          return false;
        }

        auto qspi{(cmd[i] >> 27) & 0x1};
        auto lsb{cmd[i] >> 26 & 0x1};
        // auto words_per_transfer{cmd[i] >> 21 & 0x3};
        size_t words_per_transfer{0};
        switch (cmd[i] >> 21 & 0x3) {
          case 0: {
            words_per_transfer = 1;
            break;
          }
          case 1: {
            words_per_transfer = 2;
            break;
          }
          case 2: {
            words_per_transfer = 4;
            break;
          }
          case 3: {
            return false;
          }
        }
        // in bytes
        auto words_size{((cmd[i] >> 16 & 0x1f) + 1) / 8};
        auto words_num{(cmd[i] & 0xffff) + 1};

        // send the words from l2mem to external device
        auto l2mem_data{std::make_unique<unsigned char[]>(words_num * words_size)};

        soc_->readMemory(l2mem_data.get(), regs_->SPIM_TX_SADDR - kL2MemBaseAddr, words_num * words_size);

        auto num_transfers{words_num / words_per_transfer};
        for (int i = 0; i < num_transfers; ++i) {
          sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
          tlm::tlm_generic_payload gp{};
          gp.set_command(tlm::TLM_WRITE_COMMAND);
          gp.set_data_length(words_per_transfer * words_size);
          gp.set_data_ptr(l2mem_data.get() + i * words_per_transfer * words_size);

          soc_->transmitSPIMSocket(chip_select_, gp, delay);
          if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
            return false;
          }
        }

        tx_eot_events_.notify(kTxEoTDelay);

        break;
      }

      case 0x9: {
        // SPI_CMD_EOT : clears the chip select (CS)
        transfer_started_ = false;
        break;
      }
    }
  }

  return true;
}

void udma::SPIM::notifyRxEventGenerator() {
  while (1) {
    wait();
    soc_->setEvent(7);
  }
}

void udma::SPIM::notifyTxEventGenerator() {
  while (1) {
    wait();
    soc_->setEvent(7);
  }
}

udma::I2S::I2S(gen::udma_regs *udma_regs, SoC *soc)
    : udma::PeriphBase{sc_core::sc_module_name{"udma-i2s"}, udma_regs, soc} {
  SC_THREAD(notifyRxEventGenerator);
  sensitive << rx_eot_events_;
  SC_THREAD(notifyTxEventGenerator);
  sensitive << tx_eot_events_;

  regs_ = &udma_regs->i_i2s;
}

void udma::I2S::notifyRxEventGenerator() {
  while (1) {
    wait();

    // slave should have been enabled all this time
    gen::i2s_channel_regs::I2S_CLKCFG_SETUP_t clk_cfg{regs_->I2S_CLKCFG_SETUP.get()};
    gen::i2s_channel_regs::I2S_SLV_SETUP_t slv_cfg{regs_->I2S_SLV_SETUP.get()};
    if (clk_cfg.SLAVE_CLK_EN == 1 && slv_cfg.SLAVE_EN == 1) {
      auto l2mem_data{std::make_unique<unsigned char[]>(regs_->I2S_RX_SIZE.get())};
      size_t num_transfers{(regs_->I2S_RX_SIZE.get() * 8) / (slv_cfg.SLAVE_BITS + 1)};
      for (size_t i = 0; i < num_transfers; ++i) {
        sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
        tlm::tlm_generic_payload gp{};
        gp.set_command(tlm::TLM_READ_COMMAND);
        gp.set_data_length((slv_cfg.SLAVE_BITS + 1) / 8);
        gp.set_data_ptr(l2mem_data.get() + i * ((slv_cfg.SLAVE_BITS + 1) / 8));

        // TODO: i am not sure yet how to select particular i2s interface out of many as there is no chip-select
        soc_->transmitI2SSocket(0, gp, delay);
        if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
          std::cerr << "[udma-i2s] capture request not properly serviced\n";
          exit(1);
        }
      }

      // send data to l2mem
      soc_->writeMemory(l2mem_data.get(), regs_->I2S_RX_SADDR.get() - kL2MemBaseAddr, regs_->I2S_RX_SIZE.get());

      soc_->setEvent(20);
    }
  }
}

void udma::I2S::notifyTxEventGenerator() {
  while (1) {
    wait();
    soc_->setEvent(20);
  }
}
}  // namespace vpvper::pulpissimo
