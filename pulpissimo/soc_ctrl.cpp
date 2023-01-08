/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc_ctrl.h"

#include <scc/utilities.h>

#include "gen/soc_ctrl.h"

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(soc_ctrl);  // NOLINT

soc_ctrl::soc_ctrl(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::soc_ctrl_regs) {
  regs->registerResources(*this);
  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  // Info register
  regs->INFO.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time t) -> bool {
    // this is a read-only register hence write to it should signal error
    return false;
  });
  regs->INFO.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time t) -> bool {
    // this sets up the 32b content to 0x1_0000 i.e. NumOfCluster = 0, NumOfCores = 1
    // this is not expected to change for PulpissimoSoC
    static bool reg_written{false};
    if (!reg_written) {
      regs->INFO.put(0x00010000);
      reg_written = true;
    }

    // for returning we read the current value of INFO and send it
    v = reg.get();
    return true;
  });

  // Boot Address register
  regs->BOOT_ADR.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time t) -> bool {
    reg.put(v);
    return true;
  });
  regs->BOOT_ADR.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time t) -> bool {
    v = reg.get();
    return true;
  });

  // Fetch Enable register
  // ...

  // PAD Mux register
  regs->PAD_MUX.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time t) -> bool {
    reg.put(v);
    return true;
  });
  regs->PAD_MUX.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time t) -> bool {
    v = reg.get();
    return true;
  });

  // JTAG register
  regs->JTAG_REG.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time t) -> bool {
    reg.put(v);

    std::cout << "[JTAG_REG]\t" << std::hex << v << std::dec << "\n";

    return true;
  });
  regs->JTAG_REG.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time t) -> bool {
    v = reg.get();
    return true;
  });

  // CORE STATUS register
}

soc_ctrl::~soc_ctrl() {}  // NOLINT

void soc_ctrl::clock_cb() { this->clk = clk_i.read(); }

void soc_ctrl::reset_cb() {
  if (rst_i.read()) {
    regs->reset_start();
  } else {
    regs->reset_stop();
  }
}

}  // namespace vpvper::pulpissimo
