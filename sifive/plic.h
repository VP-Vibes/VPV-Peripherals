/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PLIC_H_
#define _PLIC_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_initiator_mixin.h>
#endif

namespace vpvper {
namespace sifive {

class plic_regs;

class plic : public sc_core::sc_module, public scc::tlm_target<> {
public:
    enum { NUM_IRQ = 64 };
    SC_HAS_PROCESS(plic); // NOLINT
    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_vector<sc_core::sc_in<bool>> global_interrupts_i{"global_interrupts_i", NUM_IRQ};
#ifdef SC_SIGNAL_IF
    sc_core::sc_out<bool> core_interrupt_o{"core_interrupt_o"};
#else
    tlm::scc::tlm_signal_bool_out core_interrupt_o{"core_interrupt_o"};
#endif
    sc_core::sc_event raise_int_ev;
    sc_core::sc_event clear_int_ev;
    plic(sc_core::sc_module_name nm);
    ~plic() override;

protected:
    void clock_cb();
    void reset_cb();

    void global_int_port_cb();
    void handle_pending_int();
    void reset_pending_int(uint32_t irq);
    void write_irq(bool irq);

    void raise_core_interrupt();
    void clear_core_interrupt();
    sc_core::sc_time clk;
    std::unique_ptr<plic_regs> regs;
    std::function<bool(scc::sc_register<uint32_t>, uint32_t)> m_claim_complete_write_cb;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _PLIC_H_ */
