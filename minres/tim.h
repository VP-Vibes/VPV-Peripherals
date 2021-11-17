/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIM_H_
#define _TIM_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>
#include "tim_regs.h"

namespace vpvper {
namespace minres {

class tim : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_vector<sc_core::sc_in<sc_core::sc_time>> tick_i{"tick_i"};

    sc_core::sc_vector<sc_core::sc_in<bool>> clear_i{"clear_i"};

    sc_core::sc_out<bool> tick_o{"tick_o"};

    tim(sc_core::sc_module_name nm, unsigned counter_width=32, unsigned tick_cnt=1, unsigned clear_cnt=0);

    virtual ~tim() override;

    void set_clock_period(sc_core::sc_time period) {
        clk_period=period;
    }

protected:
    sc_core::sc_time clk_period, last_clk_period;
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk_period > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk_period : 0.;
    }
    void update_counter();
    void write_cmpgpio(size_t, bool);
    std::unique_ptr<tim_regs> regs;
    uint64_t counter{0};
    sc_core::sc_time last_cnt_update;
    double clk_remainder = 0.0;
    bool last_enable = false, reset_cnt = false;
    sc_core::sc_event update_counter_evt;
    std::array<bool, 4> pwmcmp_ip;
    const unsigned counter_width;
};

using tim_tc =scc::ticking_clock<tim>;
using tim_tl =scc::tickless_clock<tim>;
} /* namespace minres */
} /* namespace vpvper */

#endif /* _TIM_H_ */
