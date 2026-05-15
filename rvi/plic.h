/*
 * Copyright (c) MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _RVI_PLIC_H_
#define _RVI_PLIC_H_

#include <algorithm>

#include <scc/register.h>
#include <scc/tlm_target.h>
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#endif

#include "gen/plic_regs.h"

namespace vpvper {
namespace rvi {

/**
 * @brief RISC-V Platform-Level Interrupt Controller (PLIC) SystemC/TLM model.
 *
 * Implements the RISC-V PLIC specification as a synthesizable SystemC module.
 * Accepts up to NUM_SOURCES interrupt inputs and drives one interrupt
 * output per context (hart). Interrupt arbitration follows priority ordering
 * with per-context threshold filtering; the lowest source ID wins ties.
 *
 * The standard claim/complete handshake is modeled via register callbacks:
 * reading the claim/complete register returns and atomically clears the
 * highest-priority pending interrupt; writing it signals completion and
 * allows the source to fire again.
 *
 * @tparam NUM_SOURCES  Number of interrupt sources representng interrupt IDs 1..NUM_SOURCES.
 *                      Source 0 is reserved and unused per the PLIC specification.
 * @tparam NUM_CONTEXTS Number of interrupt contexts (typically one per hart).
 */
template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
class plic : public sc_core::sc_module, public scc::tlm_target<> {
   public:
    SC_HAS_PROCESS(plic);  // NOLINT

    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};

#ifdef SC_SIGNAL_IF
    sc_core::sc_vector<sc_core::sc_in<bool>> interrupts_i;
    sc_core::sc_vector<sc_core::sc_out<bool>> interrupts_o;
#else
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_in> interrupts_i;
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_out> interrupts_o;
#endif

    plic(sc_core::sc_module_name nm);
    ~plic() override;

   protected:
    /**
     * @brief SC_METHOD sensitive to clk_i; captures the current clock period.
     *
     * Stores the value read from clk_i into the internal @ref clk member so
     * that the TLM target base class always has an up-to-date clock reference.
     */
    void clock_cb();

    /**
     * @brief SC_METHOD sensitive to rst_i; drives the register block reset.
     *
     * Calls regs->reset_start() while rst_i is asserted and regs->reset_stop()
     * once it is de-asserted.
     */
    void reset_cb();

#ifdef SC_SIGNAL_IF
    /**
     * @brief SC_METHOD triggered on any rising edge of interrupts_i (sc_signal build).
     *
     * Walks all interrupt sources, sets the pending bit for every source that
     * has transitioned high and has been cleared, then calls handle_pending_irq()
     * for every context.
     */
    void source_irq_cb();
#else
    /**
     * @brief TLM nb_transport callback invoked when a source interrupt changes (TLM build).
     *
     * Sets the pending bit for the specified source when it asserts and has
     * been cleared, then calls handle_pending_irq() for every context.
     *
     * @param irq_idx Source interrupt index (0 corresponds to source ID 1).
     * @param value   New value driven by the source (non-zero = asserted).
     */
    void source_irq_cb(uint32_t irq_idx, uint32_t value);
#endif

    /**
     * @brief Evaluate pending interrupts for a context and assert the core IRQ if needed.
     *
     * Checks whether any enabled, pending source interrupt above the context
     * threshold exists. If one is found, the interrupt output is asserted.
     * If not, the interrupt output is de-asserted.
     *
     * @param ctx Context index to evaluate (0-based).
     */
    void handle_pending_irq(uint32_t ctx);

    /**
     * @brief Read callback for the claim/complete register of a context.
     *
     * Finds the highest-priority pending interrupt ID for @p ctx and clears
     * its pending bit, implementing the atomic claim operation defined by the
     * PLIC specification.
     *
     * @param ctx         Context index performing the claim (0-based).
     * @param[out] src_id Filled with the claimed interrupt source ID, or 0 if none.
     */
    void claim_complete_read_cb(uint32_t ctx, uint32_t& src_id);

    /**
     * @brief Write callback for the claim/complete register of a context.
     *
     * Signals that the interrupt identified by @p src_id has been serviced and
     * marks the source as cleared so it can fire again. Writes for source ID 0
     * (reserved) and for disabled sources are silently ignored.
     *
     * @param ctx Context index completing the interrupt (0-based).
     * @param src_id Interrupt source ID being completed.
     */
    void claim_complete_write_cb(uint32_t ctx, uint32_t src_id);

    /**
     * @brief Drive the core interrupt output for a context.
     *
     * Abstracts the sc_signal / TLM signal interface difference and writes
     * @p value to interrupts_o[ctx].
     *
     * @param ctx   Context index whose interrupt output is to be driven (0-based).
     * @param value Value to drive: true = assert, false = de-assert.
     */
    void write_irq(uint32_t ctx, bool value);

    /**
     * @brief Find a candidate pending interrupt for a context.
     *
     * Scans all interrupt sources for one that is pending, enabled for @p ctx,
     * and has priority strictly above the context threshold.
     *
     * When @p find_top_priority is true the full source list is scanned and
     * the highest-priority (lowest source ID on a tie) interrupt ID is returned.
     * When false the scan stops at the first qualifying source, which is
     * sufficient for deciding whether an interrupt should be raised.
     *
     * @param ctx               Context index to evaluate (0-based).
     * @param find_top_priority True to return the highest-priority source ID;
     *                          false to return any qualifying source ID.
     * @param ignore_threshold  True to ignore the threshold value during the search;
     *                          false to take threshold into account.
     * @return Source interrupt ID of the selected interrupt, or 0 if none qualifies.
     */
    uint32_t get_source_irq(uint32_t ctx, bool find_top_priority, bool ignore_threshold) const;

    /**
     * @brief Return a reference to the pending register value that contains @p src_id.
     *
     * The PLIC pending state is stored as a flat array of 32-bit words; this
     * helper returns the word for the given interrupt source ID so callers can
     * test or modify individual bits.
     *
     * @param src_id Interrupt source ID whose pending word is required.
     * @return       Reference to the 32-bit pending register containing the bit for @p src_id.
     */
    uint32_t& get_pending(uint32_t src_id) const;

    /**
     * @brief Convert a zero-based source interrupt vector index to a PLIC source ID.
     *
     * The PLIC reserves source ID 0 as "no interrupt", so the first real interrupt
     * source (index 0 in the interrupt vector) maps to PLIC source ID 1.  This
     * helper encapsulates that offset so callers never hard-code the adjustment.
     *
     * @param src_idx Zero-based index into the source interrupt vector.
     * @return        PLIC source ID corresponding to @p src_idx (always >= 1).
     */
    uint32_t get_source_id(uint32_t src_idx) const;

    sc_core::sc_time clk;
    std::unique_ptr<plic_regs<NUM_SOURCES, NUM_CONTEXTS>> regs;
    std::array<bool, NUM_SOURCES> src_irq_cleared;
};

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
plic<NUM_SOURCES, NUM_CONTEXTS>::plic(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm),
      tlm_target<>(clk),
      interrupts_i("interrupts_i", NUM_SOURCES),
      interrupts_o("interrupts_o", NUM_CONTEXTS)

{
    src_irq_cleared.fill(true);

    regs = std::make_unique<plic_regs<NUM_SOURCES, NUM_CONTEXTS>>("regs");
    regs->registerResources(*this);
    // register per-context claim/complete write callbacks
    for (uint32_t i = 0; i < NUM_CONTEXTS; ++i) {
        regs->claim_complete[i]->set_read_cb(
            [this, i](const scc::sc_register<uint32_t>& reg, uint32_t& v, sc_core::sc_time& d) -> bool {
                claim_complete_read_cb(i, v);
                return true;
            });
        regs->claim_complete[i]->set_write_cb(
            [this, i](scc::sc_register<uint32_t>& reg, const uint32_t& v, sc_core::sc_time& d) -> bool {
                claim_complete_write_cb(i, v);
                return true;
            });
    }

    // port callbacks
#ifdef SC_SIGNAL_IF
    SC_METHOD(source_irq_cb);
    for (auto& irq : interrupts_i) sensitive << irq.pos();
    dont_initialize();
#else
    for (uint32_t i = 0; i < interrupts_i.size(); ++i) {
        interrupts_i[i].register_nb_transport(
            [this, i](tlm::scc::tlm_signal_gp<bool>& gp, tlm::tlm_phase& p, sc_core::sc_time& t) {
                this->source_irq_cb(i, gp.get_value());
                return tlm::TLM_COMPLETED;
            });
    }
#endif

    // register event callbacks
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
plic<NUM_SOURCES, NUM_CONTEXTS>::~plic() {}  // NOLINT

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::clock_cb() {
    this->clk = clk_i.read();
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::reset_cb() {
    if (rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

#ifdef SC_SIGNAL_IF
template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::source_irq_cb() {
    auto handle_pending = false;
    for (auto i = 0UL; i < interrupts_i.size(); ++i) {
        if (src_irq_cleared[i] && (interrupts_i[i].read() == 1)) {
            const auto src_id = get_source_id(i);
            const auto bit_ofs = src_id & 0x1F;
            get_pending(src_id) |= (0x1UL << bit_ofs);
            src_irq_cleared[i] = false;
            handle_pending = true;
            SCCDEBUG(this->name()) << "Setting interrupt " << src_id << " as pending";
        }
    }

    if (handle_pending) {
        for (auto i = 0UL; i < NUM_CONTEXTS; ++i) {
            handle_pending_irq(i);
        }
    }
}
#else
template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::source_irq_cb(uint32_t irq_idx, uint32_t value) {
    if (!src_irq_cleared[irq_idx] || !value) {
        // If the source interrupt has not been cleared or the interrupt isn't high, do nothing
        return;
    }

    // Set the source's pending interrupt bit
    const auto src_id = get_source_id(irq_idx);
    const auto bit_ofs = src_id & 0x1F;
    get_pending(src_id) |= (0x1UL << bit_ofs);
    src_irq_cleared[irq_idx] = false;
    SCCDEBUG(this->name()) << "Setting interrupt " << src_id << " as pending";

    // Check if any contexts need interrupts
    for (auto i = 0UL; i < NUM_CONTEXTS; ++i) {
        handle_pending_irq(i);
    }
}
#endif

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::handle_pending_irq(uint32_t ctx) {
    if (get_source_irq(ctx, false, false)) {
        // If any pending interrupt passes the threshold test and an interrupt is not already active, trigger one
        SCCDEBUG(this->name()) << "Context " << ctx << " sending interrupt";
        write_irq(ctx, true);
    } else {
        // If there are no pending source interrupts that pass the threshold test, the interrupt can be deactivated
        write_irq(ctx, false);
        SCCDEBUG(this->name()) << "Context " << ctx << " has no further pending interrupts";
    }
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::claim_complete_read_cb(uint32_t ctx, uint32_t& src_id) {
    // Reading the claim/complete register returns the ID of the highest priority pending interrupt and clears that
    // source's interrupt pending bit. The claim operation is not affected by the threshold setting.
    src_id = get_source_irq(ctx, true, false);
    regs->r_claim_complete[ctx] = src_id;
    const auto bit_ofs = src_id & 0x1F;
    get_pending(src_id) &= ~(0x1u << bit_ofs);
    SCCTRACE(this->name()) << "Context " << ctx << " claimed interrupt " << src_id;

    // After clearing the highest priority interrupt's pending bit, handle any remaining pending interrupts.
    // This might cause the output interrupt to de-assert if there are no pending interrupts that are serviceable,
    // or the output interrupt might remain asserted if there are.
    handle_pending_irq(ctx);
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::claim_complete_write_cb(uint32_t ctx, uint32_t src_id) {
    if (src_id == 0) {
        // Interrupt 0 is unused/reserved
        SCCTRACE(this->name()) << "Context " << ctx << " ignoring claim completion for interrupt 0";
        return;
    }

    // If the source interrupt is disabled, ignore the claim completion
    const auto reg_idx = src_id >> 5;
    const auto bit_ofs = src_id & 0x1F;
    const bool enabled = (regs->r_enabled[ctx][reg_idx] >> bit_ofs) & 1u;
    if (!enabled) {
        SCCTRACE(this->name()) << "Context " << ctx << " ignoring claim completion for disabled source " << src_id;
        return;
    }

    // Writing the claim/complete register signals completion of the interrupt ID written.
    // The source interrupt can now be triggered again.
    SCCTRACE(this->name()) << "Context " << ctx << " claim complete for interrupt " << src_id;
    src_irq_cleared[src_id - 1] = true;  // Source cleared array does not include an entry for unused source 0
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
void plic<NUM_SOURCES, NUM_CONTEXTS>::write_irq(uint32_t ctx, bool value) {
#ifdef SC_SIGNAL_IF
    interrupts_o[ctx].write(value);
#else
    sc_core::sc_time t;
    tlm::tlm_phase p{tlm::BEGIN_REQ};
    tlm::scc::tlm_signal_gp<bool> gp;
    gp.set_value(value);
    interrupts_o[ctx]->nb_transport_fw(gp, p, t);
#endif
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
uint32_t plic<NUM_SOURCES, NUM_CONTEXTS>::get_source_irq(uint32_t ctx, bool find_top_priority,
                                                         bool ignore_threshold) const {
    auto prio_irq = 0U;
    auto top_prio = 0U;
    const auto thold = regs->r_threshold[ctx];

    // Walk through each interrupt source and check for pending interrupts
    for (auto i = 0UL; i < interrupts_i.size(); ++i) {
        const auto src_id = get_source_id(i);
        const auto reg_idx = src_id >> 5;
        const auto bit_ofs = src_id & 0x1F;
        const bool pending = (get_pending(src_id) >> bit_ofs) & 1u;
        const bool enabled = (regs->r_enabled[ctx][reg_idx] >> bit_ofs) & 1u;
        const auto prio = regs->r_priority[src_id];

        if (pending && enabled && (ignore_threshold || (prio > thold))) {
            // Lowest source ID wins in case of equal priority
            if (prio > top_prio) {
                top_prio = prio;
                prio_irq = src_id;
                if (!find_top_priority) {
                    // If we don't need the top priority interrupt, we can stop here
                    SCCDEBUG(this->name()) << "Context " << ctx << " found pending interrupt " << src_id;
                    break;
                }
            }
        }
    }

    if (find_top_priority) {
        SCCDEBUG(this->name()) << "Context " << ctx << " top priority pending interrupt is " << prio_irq;
    }

    return prio_irq;
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
uint32_t& plic<NUM_SOURCES, NUM_CONTEXTS>::get_pending(uint32_t src_id) const {
    const auto reg_idx = src_id >> 5;
    return regs->r_pending[reg_idx];
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
uint32_t plic<NUM_SOURCES, NUM_CONTEXTS>::get_source_id(uint32_t src_idx) const {
    // Source interrupt 0 is not valid on the PLIC, so index 0 of the source interrupt vector corresponds to ID 1
    return src_idx + 1;
}

}  // namespace rvi
}  // namespace vpvper

#endif  // _RVI_PLIC_H_
