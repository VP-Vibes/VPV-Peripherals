/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _CLINT_H_
#define _CLINT_H_

#include "scc/tlm_target.h"

namespace iss {
namespace arch {
template <typename BASE> class riscv_hart_msu_vp;
}
}

namespace vpvper {
namespace sifive {
    
class clint_regs;
namespace SiFive {
class core_complex;
}

class clint : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(clint);// NOLINT
    sc_core::sc_in<sc_core::sc_time> tlclk_i;
    sc_core::sc_in<sc_core::sc_time> lfclk_i;
    sc_core::sc_in<bool> rst_i;
    sc_core::sc_out<bool> mtime_int_o;
    sc_core::sc_out<bool> msip_int_o;
    clint(sc_core::sc_module_name nm);
    virtual ~clint() override; // NOLINT // need to keep it in source file because of fwd declaration of clint_regs

protected:
    void clock_cb();
    void reset_cb();
    void update_mtime();
    sc_core::sc_time clk, last_updt;
    unsigned cnt_fraction;
    std::unique_ptr<clint_regs> regs;
    sc_core::sc_event mtime_evt;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _CLINT_H_ */
