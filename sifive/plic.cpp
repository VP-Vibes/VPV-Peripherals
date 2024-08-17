/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "plic.h"
#include "gen/plic_regs.h"

#include <scc/report.h>
#include <scc/utilities.h>

namespace vpvper {
namespace sifive {

plic::plic(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, NAMEDD(regs, plic_regs)

{
    regs->registerResources(*this);
    // register callbacks
    regs->claim_complete.set_write_cb([this](scc::sc_register<uint32_t> reg, uint32_t v, sc_core::sc_time d) -> bool {
        reg.put(v);
        reset_pending_int(v);
        // std::cout << "Value of register: 0x" << std::hex << reg << std::endl;
        // todo: reset related interrupt and find next high-prio interrupt
        return true;
    });

    // port callbacks
    SC_METHOD(global_int_port_cb);
    for(auto& irq : global_interrupts_i)
        sensitive << irq.pos();
    dont_initialize();

    // register event callbacks
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
}

plic::~plic() {} // NOLINT

void plic::clock_cb() { this->clk = clk_i.read(); }

void plic::reset_cb() {
    if(rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

// Functional handling of interrupts:
// - global_int_port_cb()
//   - set pending register bits
//   - called by: incoming global_int
// - handle_pending_int()
//   - update claim register content
//   - generate core-interrupt pulse
//   - called by:
//     - incoming global_int
//     - complete-register write access
// - reset_pending_int(int-id)
//   - reset pending bit
//   - call next handle_pending_int()
//   - called by:
//     - complete-reg write register content

void plic::global_int_port_cb() {
    auto handle_pending = false;
    // set related pending bit if enable is set for incoming global_interrupt
    for(uint32_t i = 1; i < 256; i++) {
        auto reg_idx = i >> 5;
        auto bit_ofs = i & 0x1F;
        bool enable = regs->r_enabled[reg_idx] & (0x1 << bit_ofs); // read enable bit

        if(enable && global_interrupts_i[i].read() == 1) {
            regs->r_pending[reg_idx] = regs->r_pending[reg_idx] | (0x1 << bit_ofs);
            handle_pending = true;
            SCCDEBUG(this->name()) << "pending interrupt identified: " << i;
        }
    }

    if(handle_pending)
        handle_pending_int();
}

void plic::handle_pending_int() {
    // identify high-prio pending interrupt and raise a core-interrupt
    auto claim_int = 0U;  // claim interrupt
    auto claim_prio = 0U; // related priority (highest prio interrupt wins the race)
    auto raise_int = false;
    auto thold = regs->r_threshold.threshold; // threshold value

    for(size_t i = 1; i < 255; i++) {
        auto reg_idx = i >> 5;
        auto bit_ofs = i & 0x1F;
        bool pending = (regs->r_pending[reg_idx] & (0x1 << bit_ofs)) ? true : false;
        auto prio = regs->r_priority[i].priority; // read priority value

        if(pending && thold < prio) {
            // below condition ensures implicitly that lowest id is selected in case of multiple identical
            // priority-interrupts
            if(prio > claim_prio) {
                claim_prio = prio;
                claim_int = i;
                raise_int = true;
                SCCDEBUG(this->name()) << "pending interrupt activated: " << i;
            }
        }
    }

    if(raise_int) {
        regs->r_claim_complete = claim_int;
        core_interrupt_o.write(true);
        // todo: evluate clock period
    } else {
        regs->r_claim_complete = 0;
        SCCDEBUG(this->name()) << "no further pending interrupt.";
    }
}

void plic::reset_pending_int(uint32_t irq) {
    // todo: evaluate enable register (see spec)
    // todo: make sure that pending is set, otherwise don't reset irq ... read spec.
    SCCTRACE(this->name()) << "reset pending interrupt: " << irq;
    // reset related pending bit
    auto reg_idx = irq >> 5;
    auto bit_ofs = irq & 0x1F;
    regs->r_pending[reg_idx] &= ~(0x1 << bit_ofs);
    core_interrupt_o.write(false);

    // evaluate next pending interrupt
    handle_pending_int();
}

} /* namespace sifive */
} /* namespace vpvper */
