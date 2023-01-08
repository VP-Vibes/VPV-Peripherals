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

soc_event::soc_event(sc_core::sc_module_name nm, SoC *soc)
    : sc_core::sc_module(nm), soc_{soc}, scc::tlm_target<>(clk), NAMEDD(regs, gen::soc_event_regs) {
  regs->registerResources(*this);
  SC_METHOD(clock_cb);
  sensitive << clk_i;
  SC_METHOD(reset_cb);
  sensitive << rst_i;

  // SW_EVENT
  regs->SW_EVENT.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->SW_EVENT.set_write_cb(vpvper::pulpissimo::simple_write);

  // FC_MASKX
  regs->FC_MASK.set_read_cb(vpvper::pulpissimo::simple_read);
  regs->FC_MASK.set_write_cb(vpvper::pulpissimo::simple_write);

  // TODO: other regs
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

bool soc_event::push(size_t id) {
  // can i forward `id` event to FC?

  uint32_t data;
  regs->FC_MASK[id / 32].read(reinterpret_cast<uint8_t *>(&data), 4);
  if ((data >> id) & 0x1) {
    return false;
  }

  soc_->raiseInterrupt(26);

  return true;
}

}  // namespace vpvper::pulpissimo
