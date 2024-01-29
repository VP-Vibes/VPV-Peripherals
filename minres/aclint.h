/*
 * Copyright {c} 2019 -2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ACLINT_H_
#define _ACLINT_H_

#include <scc/tlm_target.h>

namespace vpvper {
namespace minres {

class Apb3AClint;

class aclint : public sc_core::sc_module{
public:
    sc_core::sc_in<sc_core::sc_time> tlclk_i{"tlclk_i"};
    sc_core::sc_in<sc_core::sc_time> lfclk_i{"lfclk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_out<bool> mtime_int_o{"mtime_int_o"};
    tlm::tlm_target_socket<> mtime{"mtime"};
    aclint(sc_core::sc_module_name nm);
    virtual ~aclint(); // NOLINT // need to keep it in source file because of fwd declaration of clint_regs

protected:
    void tl_clock_cb();
    void clock_reset_cb();
    void update_mtime();
    sc_core::sc_time clk, last_updt, tlclk;
    scc::tlm_target_mod<> mtime_target{"mtime_target", tlclk};
    std::unique_ptr<Apb3AClint> regs;
    sc_core::sc_event mtime_evt;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _ACLINT_H_ */
