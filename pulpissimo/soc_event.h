/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PULPISSIMO_SOC_EVENT_H_
#define _PULPISSIMO_SOC_EVENT_H_
#include <scc/tlm_target.h>

#include "util.h"

namespace vpvper::pulpissimo {
namespace gen {
class soc_event_regs;
}

class soc_event final : public sc_core::sc_module, public scc::tlm_target<> {
 public:
  sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
  sc_core::sc_in<bool> rst_i{"rst_i"};

  soc_event(sc_core::sc_module_name, SoC *);
  virtual ~soc_event() override;

  bool push(size_t);

 private:
  sc_core::sc_time clk;
  std::unique_ptr<gen::soc_event_regs> regs;
  SoC *soc_{nullptr};

  void clock_cb();
  void reset_cb();
};

}  // namespace vpvper::pulpissimo

#endif /* _PULPISSIMO_SOC_EVENT_H_ */
