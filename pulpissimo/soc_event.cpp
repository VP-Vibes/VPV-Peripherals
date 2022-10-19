/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc_event.h"

#include <scc/utilities.h>

#include "gen/soc_event.h"

#include "util.h"

namespace vpvper::pulpissimo {
SC_HAS_PROCESS(soc_event);  // NOLINT

soc_event::soc_event(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm), scc::tlm_target<>(clk), NAMEDD(regs, gen::soc_event_regs) {
  regs->registerResources(*this);
  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

/*
  regs->SW_EVENT.set_read_cb(
      [this](scc::sc_register<uint32_t> const&, uint32_t& v, sc_core::sc_time t) -> bool { return true; });
  regs->ERR.set_write_cb(
      [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time t) -> bool { return true; });

  // FC_MASKX
//  regs->FC_MASK.set_write_cb([this](scc::sc_register<uint32_t>& regs, uint32_t v, sc_core::sc_time t) -> bool {
//  });
*/

  // SW_EVENT
  regs->SW_EVENT.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->SW_EVENT.set_write_cb(vpvper::pulpissimo::simple_write);

  // FC_MASKX
  regs->FC_MASK.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->FC_MASK.set_write_cb(vpvper::pulpissimo::simple_write);

}

soc_event::~soc_event() {}  // NOLINT

void soc_event::clock_cb() { this->clk = clk_i.read(); }

void soc_event::reset_cb() {
  if (rst_i.read()) {
    regs->reset_start();
  } else {
    regs->reset_stop();
  }
}

}  // namespace vpvper::pulpissimo
