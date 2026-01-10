/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:41 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#pragma once

#include <scc/tlm_target.h>

namespace vpvper {
namespace minres {

namespace gen {
class ethmac_regs;
}

class ethmac : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    ethmac(sc_core::sc_module_name nm);
    virtual ~ethmac() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::ethmac_regs> regs;
};

} // namespace minres
} // namespace vpvper