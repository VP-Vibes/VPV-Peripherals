/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _CLINT_H_
#define _CLINT_H_

#include "scc/tlm_target.h"
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/tlm_signal.h>
#endif

namespace iss {
namespace arch {
template <typename BASE> class riscv_hart_msu_vp;
}
} // namespace iss

namespace vpvper {
namespace sifive {

class clint_regs;
namespace SiFive {
class core_complex;
}

class clint : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(clint); // NOLINT
    sc_core::sc_in<sc_core::sc_time> tlclk_i;
    sc_core::sc_in<sc_core::sc_time> lfclk_i;
    sc_core::sc_in<bool> rst_i;
#ifdef SC_SIGNAL_IF
    sc_core::sc_out<bool> mtime_int_o;
#else
    tlm::scc::tlm_signal_bool_out mtime_int_o{"mtime_int_o"};
#endif
#ifdef SC_SIGNAL_IF
    sc_core::sc_out<bool> msip_int_o;
#else
    tlm::scc::tlm_signal_bool_out msip_int_o{"msip_int_o"};
#endif
    clint(sc_core::sc_module_name nm);
    virtual ~clint() override; // NOLINT // need to keep it in source file because of fwd declaration of clint_regs

protected:
    void clock_cb();
    void reset_cb();
    void update_mtime();
    void write_mtime_irq(bool irq);
    void write_msip_irq(bool irq);
    sc_core::sc_time clk, last_updt;
    std::unique_ptr<clint_regs> regs;
    sc_core::sc_event mtime_evt;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _CLINT_H_ */
