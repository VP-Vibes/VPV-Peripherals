/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIM_H_
#define _TIM_H_

#include "scc/tlm_target.h"
#include <scc/clock_if_mixins.h>
#include "presc_regs.h"

namespace vpvper {
namespace minres {

template<unsigned COUNTER_WIDTH=32>
class presc : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_in<bool> clear_i{"clear_i"};

    sc_core::sc_out<sc_core::sc_time> overflow_o{"overflow_o"};

    presc(sc_core::sc_module_name nm);

    virtual ~presc() override;

protected:
    sc_core::sc_time clk_period, last_clk_period;
    sc_core::sc_time last_cnt_update;
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk_period > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk_period : 0.;
    }
    void update_counter();
    std::unique_ptr<presc_regs<COUNTER_WIDTH>> regs;
    uint64_t counter{0};
    double clk_remainder = 0.0;
    bool last_enable{false}, reset_cnt{false};
    sc_core::sc_event update_counter_evt;
};

template<unsigned COUNTER_WIDTH>
presc<COUNTER_WIDTH>::presc(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, regs(scc::make_unique<presc_regs<COUNTER_WIDTH>>("regs"))
{
    using this_class = presc<COUNTER_WIDTH>;
    SC_HAS_PROCESS(this_class);
    regs->registerResources(*this);
    regs->limit.set_write_cb(
            [this](const scc::sc_register<uint32_t> &reg, const uint32_t &data, sc_core::sc_time d) -> bool {
        if (d.value()) wait(d);
        data&=(1ULL<<COUNTER_WIDTH)-1;
        update_counter_evt.notify(sc_core::SC_ZERO_TIME);
        return true;
    });
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(update_counter);
    sensitive << update_counter_evt;
    dont_initialize();
}

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
presc<COUNTER_WIDTH>::~presc() = default;

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
void presc<COUNTER_WIDTH>::reset_cb() {
    overflow_o.write(sc_core::SC_ZERO_TIME);
    if (rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
void presc<COUNTER_WIDTH>::update_counter() {
    auto now = sc_core::sc_time_stamp();
    if (now == sc_core::SC_ZERO_TIME) return;
    update_counter_evt.cancel();
    if(regs->limit>0){
        auto dpulses = get_pulses(sc_core::SC_ZERO_TIME);
        auto pulses = static_cast<int>(dpulses);
        clk_remainder += dpulses - pulses;
        if (clk_remainder > 1) {
            pulses++;
            clk_remainder -= 1.0;
        }
        if (reset_cnt) {
            counter = 0;
            reset_cnt = false;
        } else if (last_enable)
            counter += pulses;
        auto next_trigger_time = regs->limit-counter * clk_period; // next trigger based on wrap around
        if (counter == regs->limit) {                                       // wrap around calculation
            reset_cnt = true;
            next_trigger_time = clk_period;
        } else {
            overflow_o.write(sc_core::sc_time_stamp()+next_trigger_time);
        }
        last_enable = true;
        update_counter_evt.notify(next_trigger_time);
    } else {
        overflow_o.write(sc_core::SC_ZERO_TIME);
        last_enable = false;
    }
    last_cnt_update = now;
    last_clk_period = clk_period;
}

template<unsigned COUNTER_WIDTH=32>
using presc_tc =scc::ticking_clock<presc<COUNTER_WIDTH>>;
template<unsigned COUNTER_WIDTH=32>
using presc_tl =scc::tickless_clock<presc<COUNTER_WIDTH>>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _TIM_H_ */
