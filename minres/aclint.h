/*
 * Copyright {c} 2019 -2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <scc/clock_if_mixins.h>
#include <scc/signal_opt_ports.h>
#include <scc/tlm_target.h>
#include <sysc/communication/sc_signal_ports.h>

namespace vpvper {
namespace minres {

class aclint_regs;

class aclint : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> mtime_clk_i{"mtime_clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_vector<sc_core::sc_out<bool>> mtime_int_o;
    sc_core::sc_vector<sc_core::sc_out<bool>> msip_int_o;
    scc::sc_out_opt<uint64_t> mtime_o;
    aclint(sc_core::sc_module_name nm, size_t num_cpus = 1);
    virtual ~aclint();
    void set_clock_period(sc_core::sc_time period) {
        update_mtime();
        clk_period = period;
        update_mtime();
    }

protected:
    void tl_clock_cb();
    void reset_cb();
    void update_mtime();
    void update_mtime_clk() { mtime_clk_period = mtime_clk_i->read(); }
    sc_core::sc_time clk_period, last_updt, mtime_clk_period;
    std::unique_ptr<aclint_regs> regs;
    sc_core::sc_event mtime_evt;
};
using aclint_tl = scc::tickless_clock<aclint>;
using aclint_tc = scc::ticking_clock<aclint>;
} // namespace minres
} /* namespace vpvper */
