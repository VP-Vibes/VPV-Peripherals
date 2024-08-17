/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "timer.h"
#include "gen/Apb3Timer_regs.h"
#include <sysc/kernel/sc_module.h>
#include <sysc/kernel/sc_time.h>

namespace vpvper {

namespace minres {

timer::timer(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, regs(scc::make_unique<Apb3Timer_regs>("regs")) {
    using this_class = timer;
    SC_HAS_PROCESS(this_class);
    regs->registerResources(*this);
    regs->t0_overflow.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
            if(d.value())
                wait(d);
            reg.put(data & (1ULL << COUNTER_WIDTH) - 1);
            update_counter_evts[0].notify(sc_core::SC_ZERO_TIME);
            return true;
        });
    regs->t1_overflow.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
            if(d.value())
                wait(d);
            reg.put(data & (1ULL << COUNTER_WIDTH) - 1);
            update_counter_evts[1].notify(sc_core::SC_ZERO_TIME);
            return true;
        });
    regs->t0_value.set_read_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
            data = counters[0] & ((1ULL << COUNTER_WIDTH) - 1);
            return true;
        });
    regs->t1_value.set_read_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
            data = counters[1] & ((1ULL << COUNTER_WIDTH) - 1);
            return true;
        });
    regs->t0_ctrl.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
            if(d.value())
                wait(d);
            reg.put(data);
            update_counter_evts[0].notify(sc_core::SC_ZERO_TIME);
            return true;
        });
    regs->t1_ctrl.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
            if(d.value())
                wait(d);
            reg.put(data);
            update_counter_evts[1].notify(sc_core::SC_ZERO_TIME);
            return true;
        });
    regs->prescaler.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
            reg.put(data);
            update_prescaler_evt.notify(sc_core::SC_ZERO_TIME);
            return true;
        });

    SC_METHOD(update_prescaler);
    sensitive << update_prescaler_evt;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(update_counter0);
    sensitive << update_counter_evts[0] << prescaler_tick_evt;
    for(auto& p : clear_i)
        sensitive << p.pos();
    for(auto& p : tick_i)
        sensitive << p.pos();
    SC_METHOD(update_counter1);
    sensitive << update_counter_evts[1] << prescaler_tick_evt;
    for(auto& p : clear_i)
        sensitive << p.pos();
    for(auto& p : tick_i)
        sensitive << p.pos();

    dont_initialize();
}

timer::~timer() = default;

void timer::reset_cb() {
    for(auto& p : interrupt_o)
        p.write(false);
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}
void timer::update_prescaler() {
    auto now = sc_core::sc_time_stamp();
    if(now == sc_core::SC_ZERO_TIME)
        return;

    update_prescaler_evt.cancel();
    if(regs->r_prescaler.limit > 0) {
        auto dpulses = get_pulses(sc_core::SC_ZERO_TIME);
        auto pulses = static_cast<int>(dpulses);
        clk_remainder += dpulses - pulses;
        if(clk_remainder > 1) {
            pulses++;
            clk_remainder -= 1.0;
        }
        if(reset_cnt) {
            presc_counter = 0;
            reset_cnt = false;
        } else if(last_enable)
            presc_counter += pulses;

        auto next_trigger_time =
            (regs->r_prescaler.limit - presc_counter) * clk_period; // next trigger based on wrap around
        if(presc_counter == regs->r_prescaler.limit) {
            // wrap around calculation
            reset_cnt = true;
            next_trigger_time = clk_period;
        } else {
            prescaler_tick_evt.notify(sc_core::sc_time_stamp() + next_trigger_time);
        }
        last_enable = true;
        update_prescaler_evt.notify(next_trigger_time);
    } else {
        prescaler_tick_evt.notify(sc_core::SC_ZERO_TIME);
        last_enable = false;
    }
    last_cnt_update = now;
    last_clk_period = clk_period;
}

void timer::update_counter(unsigned idx) {
    auto now = sc_core::sc_time_stamp();
    if(now == sc_core::SC_ZERO_TIME)
        return;

    update_counter_evts[idx].cancel();
    auto clear = idx == 0 ? regs->r_t0_ctrl.clear : regs->r_t1_ctrl.clear;
    auto enable = idx == 0 ? regs->r_t0_ctrl.enable : regs->r_t1_ctrl.enable;
    auto counter = counters[idx];
    auto any_clear = false;
    auto any_tick = (enable & 1) && prescaler_tick_evt.triggered();

    auto mask = 1U;
    for(auto& pin : clear_i) {
        any_clear |= (clear & mask) && pin->posedge();
        mask <<= 1;
    }
    mask = 2U;
    for(auto& pin : tick_i) {
        any_tick |= (clear & mask) && pin->posedge();
        mask <<= 1;
    }
    interrupt_o[idx] = false;
    if(any_clear) {
        counter = 0;
    } else if(any_tick) {
        counter++;
        auto overflow = idx == 0 ? regs->r_t0_overflow : regs->r_t1_overflow;
        if(counter >= overflow) {
            interrupt_o[idx] = true;
            counter = 0;
        }
    }
}
} // namespace minres
} // namespace vpvper
