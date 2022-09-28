/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIM_H_
#define _TIM_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>

namespace vpvper {
namespace minres {

class presc_regs;

class presc : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_in<bool> clear_i{"clear_i"};

    sc_core::sc_out<sc_core::sc_time> overflow_o{"overflow_o"};

    presc(sc_core::sc_module_name nm, unsigned counter_width=32);

    virtual ~presc() override;

    void set_clock_period(sc_core::sc_time period) {
        clk_period=period;
    }

protected:
    sc_core::sc_time clk_period, last_clk_period;
    sc_core::sc_time last_cnt_update;
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk_period > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk_period : 0.;
    }
    void update_counter();
    std::unique_ptr<presc_regs> regs;
    uint64_t counter{0};
    double clk_remainder = 0.0;
    bool last_enable{false}, reset_cnt{false};
    sc_core::sc_event update_counter_evt;
    const unsigned counter_width;
};

using presc_tc =scc::ticking_clock<presc>;
using presc_tl =scc::tickless_clock<presc>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _TIM_H_ */
