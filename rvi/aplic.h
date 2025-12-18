/*
 * aplic.h - RISC-V aplic model
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

#ifndef _APLIC_RISCV_H_
#define _APLIC_RISCV_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#endif

namespace vpvper {
namespace rvi {

class aplic_regs;

class aplic : public sc_core::sc_module, public scc::tlm_target<> {
public:
    enum SourceMode : uint8_t { INACTIVE = 0, DETACHED = 1, EDGERISING = 4, EDGEFALLING = 5, LEVELHIGH = 6, LEVELLOW = 7 };
    SC_HAS_PROCESS(aplic);

    // the maximum number of interrupt sources an APLIC may support is 1023.
    cci::cci_param<uint32_t> num_irq {"num_irq", 1023, "Aplic Number of Interrupt"};
    cci::cci_param<uint32_t> num_irq_prio {"num_irq_prio", 255, "Aplic Number of Interrupt Priority"};

    sc_core::sc_in<bool> rst_i{"rst_i"};
#ifdef SC_SIGNAL_IF
    sc_core::sc_out<bool> core_interrupt_o{"core_interrupt_o"};
    sc_core::sc_vector<sc_core::sc_in<bool>> external_global_interrupts_i;
#else
    tlm::scc::tlm_signal_bool_out core_interrupt_o{"core_interrupt_o"};
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_in> external_global_interrupts_i;
#endif


    aplic(sc_core::sc_module_name nm);
    virtual ~aplic() override;

    void set_clock_period(sc_core::sc_time period) { clk_period = period; }

protected:
    void reset_cb();

#ifdef SC_SIGNAL_IF
    void external_global_interrupts_cb();
#else
    void external_global_interrupts_cb(uint32_t irq, uint32_t value);
#endif
    bool external_global_interrupts_handling(uint32_t irq, uint32_t value, bool &handle_pending);
    void write_irq(bool irq);
    void handle_pending_int();
    void reset_pending_int(uint32_t irq);
    bool is_source_active(uint32_t index);
    uint8_t get_source_mode(uint32_t irq);
    uint32_t get_interrupt_priority(uint32_t irq);
    uint32_t get_reg_index (uint32_t id);
    uint32_t get_bit_pos(uint32_t id);
    bool register_pending_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data);
    bool register_clrpending_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data);
    bool register_enabled_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data);
    bool register_clrenabled_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data);

    sc_core::sc_time clk_period;
    std::unique_ptr<aplic_regs> regs;
    std::function<bool(scc::sc_register<uint32_t>, uint32_t)> m_domaincfg_write_cb;

    static constexpr uint32_t REG_SIZE = 32;
    static constexpr uint32_t REG_INDEX_SHIFT = 5;
    static constexpr uint32_t BIT_POS_MASK = 0x1F;
    static constexpr uint32_t INTERRUPT_PRIO_MASk = 0xFF;

    using aplic_tc = scc::ticking_clock<aplic>;
    using aplic_tl = scc::tickless_clock<aplic>;
};

} /* namespace rvi */
} /* namespace vpvper */

#endif /* _APLIC_RISCV_H_ */
