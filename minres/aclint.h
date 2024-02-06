/*
 * Copyright {c} 2019 -2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ACLINT_H_
#define _ACLINT_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>
#include <sysc/communication/sc_signal_ports.h>

namespace vpvper {
namespace minres {

class Apb3AClint_regs;

class aclint : public sc_core::sc_module, public scc::tlm_target<>{
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_out<bool> mtime_int_o{"mtime_int_o"};
    sc_core::sc_out<bool> msip_int_o{"msip_int_o"};
    aclint(sc_core::sc_module_name nm);
    virtual ~aclint(); 
    void set_clock_period(sc_core::sc_time period) {
        update_mtime();
        clk_period=period;
        update_mtime();
    }
protected:
    void tl_clock_cb();
    void reset_cb();
    void update_mtime();
    sc_core::sc_time clk_period, last_updt;
    std::unique_ptr<Apb3AClint_regs> regs;
    sc_core::sc_event mtime_evt;
};
using aclint_tl = scc::tickless_clock<aclint>;
using aclint_tc = scc::ticking_clock<aclint>;
} /* namespace sifive */
} /* namespace vpvper */

#endif /* _ACLINT_H_ */
