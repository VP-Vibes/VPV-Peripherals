/*
 * aplic.cpp - RISC-V APLIC model
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 */

 /* This model supports only the Direct delivery mode.
  * It has been tested for a single hart (index:0) implementing only M-mode, with a single machine-level interrupt domain for that hart,
  * and for level-triggered interrupts using source and target TLM signals.
  * It also supports edge-triggered interrupts and SystemC signals, however, these features have not yet been validated.
  * Additionally, functionality for the pendingnum, clripnum registers is not implemented.
 */

#include "aplic.h"
#include "gen/aplic_regs.h"
#include "tlm/scc/tlm_signal_gp.h"

#include <scc/report.h>
#include <scc/utilities.h>
#include <tlm_core/tlm_2/tlm_generic_payload/tlm_phase.h>

namespace vpvper {
namespace rvi {

aplic::aplic(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, aplic_regs)
, external_global_interrupts_i("external_global_interrupts_i", num_irq.get_value()){

    SCCDEBUG(this->name()) << "Initialize Aplic Model with " << external_global_interrupts_i.size() << " interrupts";

    regs->registerResources(*this);

    // register callbacks

    regs->sourcecfg.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        uint32_t irq = index + 1;
        if(irq > external_global_interrupts_i.size()) {
            SCCDEBUG(this->name()) << "sourcecfg: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size()<< " supported by the current configuration";
        }else {
            regs->r_sourcecfg[index] = data;
        }
        return true;
    });

    regs->targetcfg.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        uint32_t irq = index + 1;
        if(irq > external_global_interrupts_i.size()) {
            SCCDEBUG(this->name()) << "target: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
        }else {
            uint32_t irq_priority = data & INTERRUPT_PRIO_MASk;
            if(irq_priority > num_irq_prio.get_value()) {
                SCCDEBUG(this->name()) << "target: The interrupt priority " << irq_priority << " exceeds the maximum value " << num_irq_prio.get_value() << " supported by the current configuration";
            }else {
                // only values 1 through Max are allowed for Interrupt Priority, not zero.
                if(irq_priority != 0) {
                    regs->r_targetcfg[index] = data;
                }
            }
        }
        return true;
    });

    regs->enablednum.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {

        auto irq = data;

        if(irq > external_global_interrupts_i.size()) {
            SCCDEBUG(this->name()) << "setie: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
        }else {
            if(is_source_active(irq)) {
                SCCDEBUG(this->name()) << "enabled interrupt: " << irq;

                auto reg_idx = get_reg_index(irq);
                auto bit_ofs = get_bit_pos(irq);
                regs->r_enabled[reg_idx] |= (0x1 << bit_ofs);;
            }
        }

        return true;
    });

    // if the source mode is Level1 or Level0 and the interrupt domain is configured in direct delivery mode
    // (domaincfg.DM = 0): The pending bit is not cleared by a claim of the interrupt at the APLIC.
    regs->claim.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        data = reg.get();
        uint8_t source_mode = get_source_mode(regs->r_claim.interruptidentity);
        if(regs->r_claim > 0) {
            if(!((source_mode == SourceMode::LEVELHIGH) || (source_mode == SourceMode::LEVELLOW))) {
                reset_pending_int(regs->r_claim.interruptidentity);
            }
        }
        return true;
    });

    // on a write to a setie register, for each bit that is one in the 32-bit value written, if that bit position
    // corresponds to an active interrupt source, the interrupt-enable bit for that source is set to one.
    regs->enabled.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        return register_enabled_callback(index, reg, data);
    });

    // if the source mode is Level1 or Level0, The pending bit cannot be set by a write to a setip or setipnum register.
    regs->pending.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        return register_pending_callback(index, reg, data);
    });

    // if the source mode is Level1 or Level0, The pending bit is not cleared by a write to an in_clrip register or to clripnum.
    regs->clrpending.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        return register_clrpending_callback(index, reg, data);
    });

    // on a write to a clrie register, for each bit that is one in the 32-bit value written, the interrupt-enable
    // bit for that source is cleared
    regs->clrenabled.set_write_cb([this](size_t index, scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        return register_clrenabled_callback(index, reg, data);
    });

    // a read of a clrie register always returns zero.
    regs->clrenabled.set_read_cb([this](size_t index, const scc::sc_register<uint32_t>& reg, uint32_t& data) -> bool {
        data = 0;
        return true;
    });
#ifdef SC_SIGNAL_IF
    SC_METHOD(external_global_interrupts_cb);
    for(auto& irq : external_global_interrupts_i)
        sensitive << irq;
    dont_initialize();
#else
    // the number zero is not a valid interrupt identity number at an APLIC.
    for(auto i = 1; i < external_global_interrupts_i.size(); i++) {
        external_global_interrupts_i[i].register_nb_transport([this, i](tlm::scc::tlm_signal_gp<bool>& gp, tlm::tlm_phase& p, sc_core::sc_time& t) {
            this->external_global_interrupts_cb(i, gp.get_value());
            return tlm::TLM_COMPLETED;
        });
    }
#endif

    // register event callbacks
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
}

aplic::~aplic() = default;

void aplic::reset_cb() {
    if(rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

bool aplic::register_pending_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data)
{
    for(uint8_t bit_pos =0 ; bit_pos < REG_SIZE; bit_pos++) {
            bool value = (data >> bit_pos) & 0x1;
            if(value) {
                uint32_t irq = index * REG_SIZE + bit_pos;
                if(irq > external_global_interrupts_i.size()) {
                    SCCDEBUG(this->name()) << "setip: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
                    continue;
                }
                uint8_t source_mode = get_source_mode(irq);
                if(!((source_mode == SourceMode::LEVELHIGH) || (source_mode == SourceMode::LEVELLOW))) {
                    if(is_source_active(irq)) {
                        SCCDEBUG(this->name()) << "set interrupt " << irq << " as pending.";
                        regs->r_pending[index] |= (0x1 << bit_pos);
                        handle_pending_int();
                    }
                }
            }
        }
    return true;
}

bool aplic::register_clrpending_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data)
{
    for(uint8_t bit_pos =0 ; bit_pos < REG_SIZE; bit_pos++) {
            bool value = (data >> bit_pos) & 0x1;
            if(value) {
                uint32_t irq = index * REG_SIZE + bit_pos;
                if(irq > external_global_interrupts_i.size()) {
                    SCCDEBUG(this->name()) << "clrip: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
                    continue;
                }
                uint8_t source_mode = get_source_mode(irq);
                if(!((source_mode == SourceMode::LEVELHIGH) || (source_mode == SourceMode::LEVELLOW))) {
                     bool pending = (regs->r_pending[index] & (0x1 << bit_pos)) ? true : false;
                    if(is_source_active(irq) && pending) {
                        SCCDEBUG(this->name()) << "clr pending interrupt: " << irq;
                        reset_pending_int(irq);
                    }
                }
            }
        }
    return true;
}

bool aplic::register_enabled_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data)
{
    for(uint8_t bit_pos =0 ; bit_pos < REG_SIZE; bit_pos++) {
        bool value = (data >> bit_pos) & 0x1;
        if(value) {
            uint32_t irq = index * REG_SIZE + bit_pos;
            if(irq > external_global_interrupts_i.size()) {
                SCCDEBUG(this->name()) << "setie: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
                continue;
            }

            if(is_source_active(irq)) {
                SCCDEBUG(this->name()) << "enabled interrupt: " << irq;
                regs->r_enabled[index] |= (0x1 << bit_pos);;
            }
        }
    }
    return true;
}

bool aplic::register_clrenabled_callback(size_t index, scc::sc_register<uint32_t>& reg, uint32_t data)
{
    for(uint8_t bit_pos =0 ; bit_pos < REG_SIZE; bit_pos++) {
        bool value = (data >> bit_pos) & 0x1;
        if(value) {
            uint32_t irq = index * REG_SIZE + bit_pos;
            if(irq > external_global_interrupts_i.size()) {
                SCCDEBUG(this->name()) << "clrenabled: The interrupt number " << irq << " exceeds the maximum value " << external_global_interrupts_i.size() << " supported by the current configuration";
                continue;
            }
            SCCDEBUG(this->name()) << "Clear the enable bit for interrupt " << irq;
            regs->r_enabled[index] &= ~(0x1 << bit_pos);
        }
    }
    return true;
}

bool aplic::is_source_active(uint32_t index) {
    uint8_t source_mode = get_source_mode(index);

    if(source_mode == SourceMode::DETACHED || source_mode == SourceMode::EDGERISING || source_mode == SourceMode::EDGEFALLING ||
    source_mode == SourceMode::LEVELHIGH || source_mode == SourceMode::LEVELLOW) {
        return true;
    }else {
        return false;
    }
}

uint8_t aplic::get_source_mode(uint32_t irq)
{
    if(irq == 0) {
        return 0;
    }
    return regs->r_sourcecfg[irq - 1];
}

uint32_t aplic::get_interrupt_priority(uint32_t irq)
{
    if(irq == 0) {
        return 0;
    }

    return regs->r_targetcfg[irq - 1].interruptpriority;
}

uint32_t aplic::get_reg_index (uint32_t id) {
    return (id >> REG_INDEX_SHIFT);
}

uint32_t aplic::get_bit_pos(uint32_t id) {
    return (id & BIT_POS_MASK);
}

#ifdef SC_SIGNAL_IF
void aplic::external_global_interrupts_cb() {
    auto handle_pending = false;
    // set related pending bit if enable is set for incoming global_interrupt
    for(uint32_t irq = 1; irq < external_global_interrupts_i.size(); irq++) {
        if(external_global_interrupts_i[irq].event()) {
            uint32_t value = external_global_interrupts_i[irq].read();
            external_global_interrupts_handling(irq, value, handle_pending);
        }
    }

    if(handle_pending){
        handle_pending_int();
    }
}
#else
void aplic::external_global_interrupts_cb(uint32_t irq, uint32_t value) {

    auto handle_pending = false;
    external_global_interrupts_handling(irq, value, handle_pending);

    if(handle_pending) {
        handle_pending_int();
    }
}
#endif

bool aplic::external_global_interrupts_handling(uint32_t irq, uint32_t value, bool &handle_pending) {

    SCCDEBUG(this->name()) << "triggered the source interrupt callback for interrupt " << irq << " with value " << value;
    uint8_t source_mode = get_source_mode(irq);
    if(source_mode == SourceMode::INACTIVE) {
        return false;
    }

    auto reg_idx = get_reg_index(irq);
    auto bit_ofs = get_bit_pos(irq);
    bool wire = (value ? true : false);

    uint8_t source_inverted = 0;

    // when configured for a falling edge or low level (mode Level0)
    // the source is said to be inverted.
    if((source_mode == SourceMode::EDGEFALLING) || (source_mode == SourceMode::LEVELLOW)) {
        source_inverted = 1;
    }

    bool rectified = wire ^ source_inverted;

    SCCDEBUG(this->name()) << "rectified value " << rectified << " using source mode " << static_cast<uint32_t>(source_mode);

    // the pending bit is set to one whenever the rectified input value is high.
    if(rectified) {
        regs->r_pending[reg_idx] |= (0x1 << bit_ofs);
        handle_pending = true;
    }


    // if the source mode is Level1 or Level0 and the interrupt domain is configured in direct delivery mode
    //(domaincfg.DM = 0): The pending bit is cleared whenever the rectified input value is low.
    if((source_mode == SourceMode::LEVELHIGH) || (source_mode == SourceMode::LEVELLOW)) {
        bool pending = (regs->r_pending[reg_idx] & (0x1 << bit_ofs)) ? true : false;
        if((!rectified) && pending) { //rectified input value is low
            reset_pending_int(irq);
        }
    }

    return handle_pending;
}

void aplic::handle_pending_int() {
    auto claim_int = 0U;  // claim interrupt
    auto claim_prio = 0U; // related priority (highest prio interrupt wins the race)
    auto raise_int = false;
    auto thold = regs->r_threshold; // threshold value

    for(size_t irq = 1; irq < external_global_interrupts_i.size(); irq++) {
        auto reg_idx = get_reg_index(irq);
        auto bit_ofs = get_bit_pos(irq);
        auto pending = (regs->r_pending[reg_idx] & (0x1 << bit_ofs)) ? true : false;
        auto prio = get_interrupt_priority(irq); // read priority value
        auto enable = (regs->r_enabled[reg_idx] & (0x1 << bit_ofs)) ? true : false;

        // when ITHRESHOLD is a nonzero value P, interrupt sources with priority numbers P and higher do not contribute to signaling 
        // interrupts to the hart, regardless of the settings of their interrupt-enable bits. When ITHRESHOLD is
        // zero, all enabled interrupt sources can contribute to signaling interrupts to the hart
        if(enable && pending && (thold == 0 || prio < thold)) {
            if(prio > claim_prio) {
                claim_prio = prio;
                claim_int = irq;
                raise_int = true;
                SCCDEBUG(this->name()) << "pending interrupt " << claim_int << " detected for assert";
            }
        }
    }

    if(raise_int) {
        // topi is a read-only register whose value indicates the current highest-priority pending-and-enabled
        // interrupt targeted to this hart that also exceeds the priority threshold specified by ithreshold, if not zero.
        regs->r_top.interruptpriority = claim_prio;
        regs->r_top.interruptidentity = claim_int;
        regs->r_claim.interruptpriority = claim_prio;
        regs->r_claim.interruptidentity = claim_int;
        write_irq(true);
    } else {
        regs->r_top = 0;
        regs->r_claim = 0;
        SCCDEBUG(this->name()) << "no pending interrupt detected for assert.";
    }
}

void aplic::reset_pending_int(uint32_t irq) {
    SCCDEBUG(this->name()) << "triggered the reset pending interrupt for " << irq;
    // reset related pending bit
    auto reg_idx = get_reg_index(irq);
    auto bit_ofs = get_bit_pos(irq);
    regs->r_pending[reg_idx] &= ~(0x1 << bit_ofs);
    write_irq(false);
    // evaluate next pending interrupt
    handle_pending_int();
}

void aplic::write_irq(bool irq) {
    if((regs->r_delivery == 1) && (regs->r_domaincfg.interruptenable == 1)) { //1 = interrupt delivery is enabled
        if(irq) {
            SCCDEBUG(this->name()) << "assert aplic interrupt\n";
        }else{
            SCCDEBUG(this->name()) << "deassert aplic interrupt\n";
        }
    #ifdef SC_SIGNAL_IF
        core_interrupt_o.write(irq);
    #else
        sc_core::sc_time t;
        tlm::tlm_phase p{tlm::BEGIN_REQ};
        tlm::scc::tlm_signal_gp<bool> gp;
        gp.set_value(irq);
        core_interrupt_o->nb_transport_fw(gp, p, t);
    #endif

    } else {
        SCCDEBUG(this->name()) << "Interrupt delivery is disabled because either domaincfg.IE is set to 0, or the delivery register has disabled interrupt delivery to the hart";
    }
}

} /* namespace rvi */
} /* namespace vpvper */
