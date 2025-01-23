/*
 * Copyright (c) 2019 -2024 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include "minres/gen/timercounter_regs.h"
#include <array>
#include <cstdint>
#include <scc/clock_if_mixins.h>
#include <scc/tlm_target.h>
#include <sysc/kernel/sc_event.h>
#include <sysc/kernel/sc_time.h>

namespace vpvper {
namespace minres {

class timercounter_regs;

class timer : public sc_core::sc_module, public scc::tlm_target<> {
public:
    enum { COUNTER_WIDTH = 32, TICK_CNT = 3, CLEAR_CNT = 2, NUM_COUNTER = 2 };

    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_vector<sc_core::sc_in<bool>> tick_i{"tick_i", TICK_CNT - 1};

    sc_core::sc_vector<sc_core::sc_in<bool>> clear_i{"clear_i", CLEAR_CNT};

    sc_core::sc_vector<sc_core::sc_out<bool>> interrupt_o{"interrupt_o", NUM_COUNTER};

    timer(sc_core::sc_module_name nm);

    virtual ~timer() override;

    void set_clock_period(sc_core::sc_time period) {
        clk_period = period;
        update_prescaler_evt.notify(sc_core::SC_ZERO_TIME);
    }

protected:
    sc_core::sc_time clk_period, last_clk_period;
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk_period > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk_period : 0.;
    }
    void update_counter0() { update_counter(0); };
    void update_counter1() { update_counter(1); };
    void update_counter(unsigned idx);
    void update_prescaler();
    std::unique_ptr<timercounter_regs> regs;
    uint64_t presc_counter{0};
    std::array<uint64_t, NUM_COUNTER> counters;
    sc_core::sc_time last_cnt_update;
    double clk_remainder = 0.0;
    bool last_enable = false, reset_cnt = false;
    sc_core::sc_event update_prescaler_evt, prescaler_tick_evt;
    std::array<sc_core::sc_event, NUM_COUNTER> update_counter_evts;
    std::array<bool, 4> pwmcmp_ip;
};

using timer_tc = scc::ticking_clock<timer>;
using timer_tl = scc::tickless_clock<timer>;
} /* namespace minres */
} /* namespace vpvper */

#endif /* _TIMER_H_ */
