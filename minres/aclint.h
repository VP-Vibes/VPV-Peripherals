/*
 * Copyright (c) 2019 -2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ACLINT_H_
#define _ACLINT_H_

#include <scc/tlm_target.h>

namespace vpvper {
namespace minres {

template <unsigned MTIMECMP_COUNT = 1, unsigned MSIP_COUNT = 1, unsigned SSWI_COUNT = 0>
class aclint_regs;

class aclint : public sc_core::sc_module{
public:
    sc_core::sc_in<sc_core::sc_time> lfclk_i{"lfclk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_out<bool> mtime_int_o{"mtime_int_o"};
    sc_core::sc_out<bool> msip_int_o{"msip_int_o"};
    sc_core::sc_out<bool> sswi_int_o{"sswi_int_o"};
    aclint(sc_core::sc_module_name nm);
    virtual ~aclint() override; // NOLINT // need to keep it in source file because of fwd declaration of clint_regs

protected:
    void clock_cb();
    void reset_cb();
    void update_mtime();
    sc_core::sc_time clk, last_updt;
    std::unique_ptr<aclint_regs<>> regs;
    std::unique_ptr<scc::tlm_target<>> mtime_target;
    std::unique_ptr<scc::tlm_target<>> msip_target;
    std::unique_ptr<scc::tlm_target<>> sswi_target;
    sc_core::sc_event mtime_evt;
    std::array<uint64_t, 1> mtimecmps; //TODO harcoded 1 entfernen und mit MTIMECMP_COUNT wechseln
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _ACLINT_H_ */
