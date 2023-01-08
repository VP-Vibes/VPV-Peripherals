/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "gen/udma.h"
#include "util.h"
//
#include "scc/memory.h"
#include "scc/tlm_target.h"

namespace vpvper::pulpissimo {
namespace gen {
class udma_regs;
}

class udma final : public sc_core::sc_module, public scc::tlm_target<> {
  SC_HAS_PROCESS(udma);

  class PeriphBase : public sc_core::sc_module {
   public:
    PeriphBase(sc_core::sc_module_name, gen::udma_regs *, SoC *);
    virtual ~PeriphBase() = default;
    // this is an abstract class hence slicing problem can't occur so no need of explicit suppressing of
    // copy/move constructor

    virtual void regs_cb() = 0;

   protected:
    const sc_core::sc_time kRxEoTDelay{10, SC_US};

    gen::udma_regs *udma_regs_{nullptr};
    SoC *soc_{nullptr};
    sc_core::sc_event_queue rx_eot_events_{};
    sc_core::sc_event_queue tx_eot_events_{};
  };

  class SPIM final : public PeriphBase {
    SC_HAS_PROCESS(SPIM);

   public:
    // passing raw pointer is dangerous but rest assured I am not going to manage this memory
    // rather the guy who owns it ( allocates it ) is responsible for releasing it
    SPIM(gen::udma_regs *, SoC *);
    // inheritance is disabled so no need to provide a virtual destructor
    // further slicing problem can not occur, hence no need to suppress copy/move stuff and defaults are ok to use

    void regs_cb() override;

   private:
    const unsigned kTX{0};
    const unsigned kRX{1};
    const unsigned kCMD{2};
    const sc_core::sc_time kTxEoTDelay{10, SC_US};

    gen::spi_channel_regs *regs_{nullptr};
    gen::spi_channel_regs::SPIM_CMD_CFG_t current_cmd_cfg_{};
    bool transfer_started_{false};
    size_t chip_select_{0};
    bool tx_initiated_{false};
    bool rx_initiated_{false};
    bool cmd_initiated_{false};

    void printCMDCFG();
    int handleCommands();
    void notifyRxEventGenerator();
    void notifyTxEventGenerator();
  };

  class I2S final : public PeriphBase {
    SC_HAS_PROCESS(I2S);

   public:
    // passing raw pointer is dangerous but rest assured I am not going to manage this memory
    // rather the guy who owns it ( allocates it ) is responsible for releasing it
    I2S(gen::udma_regs *, SoC *);
    // inheritance is disabled so no need to provide a virtual destructor
    // further slicing problem can not occur, hence no need to suppress copy/move stuff and defaults are ok to use

    void regs_cb() override;

   private:
    gen::i2s_channel_regs *regs_{nullptr};

    void notifyRxEventGenerator();
    void notifyTxEventGenerator();
  };

 public:
  sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
  sc_core::sc_in<bool> rst_i{"rst_i"};

  // passing raw pointer is dangerous but rest assured I am not going to manage this memory
  // rather the guy who owns it ( allocates it ) is responsible for releasing it
  udma(sc_core::sc_module_name, SoC *);
  // inheritance is disabled so no need to provide a virtual destructor
  // further slicing problem can not occur, hence no need to suppress copy/move stuff and defaults are ok to use

 private:
  sc_core::sc_time clk;
  std::unique_ptr<gen::udma_regs> regs;
  SoC *soc_{nullptr};
  SPIM spim_{regs.get(), soc_};
  I2S i2s_{regs.get(), soc_};

  void clock_cb();
  void reset_cb();
};

}  // namespace vpvper::pulpissimo
