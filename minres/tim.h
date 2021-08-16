/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIM_H_
#define _TIM_H_

#include "scc/tlm_target.h"
#include "tim_regs.h"

namespace vpvper {
namespace minres {

template<unsigned COUNTER_WIDTH=32, unsigned TICK_CNT=1, unsigned CLEAR_CNT=1>
class tim : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_vector<sc_core::sc_in<sc_core::sc_time>> tick_i{"tick_i", TICK_CNT};

    sc_core::sc_vector<sc_core::sc_in<bool>> clear_i{"clear_i", CLEAR_CNT};

    sc_core::sc_out<bool> tick_o{"tick_o"};

    tim(sc_core::sc_module_name nm);
    virtual ~tim() override;

protected:
    sc_core::sc_time clk, last_clk;
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk : 0.;
    }
    void update_counter();
    void write_cmpgpio(size_t, bool);
    std::unique_ptr<tim_regs<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>> regs;
    uint64_t counter{0};
    sc_core::sc_time last_cnt_update;
    double clk_remainder = 0.0;
    bool last_enable = false, reset_cnt = false;
    sc_core::sc_event update_counter_evt;
    std::array<bool, 4> pwmcmp_ip;
};

template<unsigned COUNTER_WIDTH, unsigned TICK_CNT, unsigned CLEAR_CNT>
tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>::tim(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, regs(scc::make_unique<tim_regs<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>>("regs"))
{
    using this_class = tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>;
    SC_HAS_PROCESS(this_class);
    regs->registerResources(*this);
    regs->limit.set_write_cb(
        [this](const scc::sc_register<uint32_t> &reg, const uint32_t &data, sc_core::sc_time d) -> bool {
            if (d.value()) wait(d);
            data &= (1ULL<<COUNTER_WIDTH)-1;
            update_counter_evt.notify(sc_core::SC_ZERO_TIME);
            return true;
        });
    regs->timer_value.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data,
                                      sc_core::sc_time d) -> bool {
        data = counter & ((1ULL<<COUNTER_WIDTH)-1);
        return true;
    });
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(update_counter);
    sensitive << update_counter_evt;
    for(auto& i:tick_i) sensitive << i;
    dont_initialize();
}

template<unsigned COUNTER_WIDTH, unsigned TICK_CNT, unsigned CLEAR_CNT>
tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>::~tim() = default;

template<unsigned COUNTER_WIDTH, unsigned TICK_CNT, unsigned CLEAR_CNT>
void tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>::reset_cb() {
    tick_o.write(false);
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

template<unsigned COUNTER_WIDTH, unsigned TICK_CNT, unsigned CLEAR_CNT>
void tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>::update_counter() {
    auto now = sc_core::sc_time_stamp();
    if (now == sc_core::SC_ZERO_TIME) return;
    update_counter_evt.cancel();
    auto any_clear = false;
    auto mask =1U;
    for(auto pin: clear_i) {
        any_clear |= (regs->clk_ctrl.clr_en & mask) && pin.read();
        mask<<=1;
    }
    if(any_clear) {
        counter=0;
        tick_o=false;
    } else if (regs->clk_ctrl.clk_en) {
        auto dpulses = get_pulses(sc_core::SC_ZERO_TIME);
        auto pulses = static_cast<int>(dpulses);
        clk_remainder += dpulses - pulses;
        if (clk_remainder > 1) {
            pulses++;
            clk_remainder -= 1.0;
        }
        if (reset_cnt) {
            current_cnt = 0;
            reset_cnt = false;
        } else if (last_enable)
            current_cnt += pulses;
        auto tims = (current_cnt >> regs->r_timcfg.timscale) & 0xffff;
        auto next_trigger_time =
            (0xffff - tims) * (1 << regs->r_timcfg.timscale) * clk; // next trigger based on wrap around
        if (tims == 0xffff) {                                       // wrap around calculation
            reset_cnt = true;
            next_trigger_time = clk;
            regs->r_timcfg.timenoneshot = 0;
        }
        auto tims0 = (regs->r_timcfg.timcmp0center && (tims & 0x8000) == 1) ? tims ^ 0xffff : tims;
        if (tims0 >= regs->r_timcmp0.timcmp0) {
            timcmp_new_ip[0] = true;
            regs->r_timcfg.timenoneshot = 0;
            if (regs->r_timcfg.timzerocmp) {
                reset_cnt = true;
                next_trigger_time = clk;
            }
        } else {
            timcmp_new_ip[0] = false;
            // TODO: add correct calculation for regs->r_timcfg.timcmpXcenter==1
            auto nt = (regs->r_timcmp0.timcmp0 - tims0) * (1 << regs->r_timcfg.timscale) * clk;
            next_trigger_time = nt < next_trigger_time ? nt : next_trigger_time;
        }
        auto tims1 = (regs->r_timcfg.timcmp0center && (tims & 0x8000) == 1) ? tims ^ 0xffff : tims;
        if (tims1 >= regs->r_timcmp1.timcmp0) {
            timcmp_new_ip[1] = true;
        } else {
            timcmp_new_ip[1] = false;
            // TODO: add correct calculation for regs->r_timcfg.timcmpXcenter==1
            auto nt = (regs->r_timcmp0.timcmp0 - tims0) * (1 << regs->r_timcfg.timscale) * clk;
            next_trigger_time = nt < next_trigger_time ? nt : next_trigger_time;
        }
        auto tims2 = (regs->r_timcfg.timcmp0center && (tims & 0x8000) == 1) ? tims ^ 0xffff : tims;
        if (tims2 >= regs->r_timcmp2.timcmp0) {
            timcmp_new_ip[2] = true;
        } else {
            timcmp_new_ip[2] = false;
            // TODO: add correct calculation for regs->r_timcfg.timcmpXcenter==1
            auto nt = (regs->r_timcmp0.timcmp0 - tims0) * regs->r_timcfg.timscale * clk;
            next_trigger_time = nt < next_trigger_time ? nt : next_trigger_time;
        }
        auto tims3 = (regs->r_timcfg.timcmp0center && (tims & 0x8000) == 1) ? tims ^ 0xffff : tims;
        if (tims3 >= regs->r_timcmp3.timcmp0) {
            timcmp_new_ip[3] = true;
        } else {
            timcmp_new_ip[3] = false;
            // TODO: add correct calculation for regs->r_timcfg.timcmpXcenter==1
            auto nt = (regs->r_timcmp0.timcmp0 - tims0) * (1 << regs->r_timcfg.timscale) * clk;
            next_trigger_time = nt < next_trigger_time ? nt : next_trigger_time;
        }
        for (size_t i = 0; i < 4; ++i) {
            // write gpio bits depending of gang bit
            if (regs->r_timcfg & (1 < (24 + i)))
                write_cmpgpio(i, timcmp_new_ip[i] && !timcmp_new_ip[(i + 1) % 4]);
            else
                write_cmpgpio(i, timcmp_new_ip[i]);
            // detect rising edge and set ip bit if found
            if (!timcmp_ip[i] && timcmp_new_ip[i]) regs->r_timcfg |= 1 << (28 + i);
            timcmp_ip[i] = timcmp_new_ip[i];
        }
        last_enable = true;
        update_counter_evt.notify(next_trigger_time);
    } else
        last_enable = false;
    cmpip_o[0].write(regs->r_timcfg.timcmp0ip != 0);
    cmpip_o[1].write(regs->r_timcfg.timcmp1ip != 0);
    cmpip_o[2].write(regs->r_timcfg.timcmp2ip != 0);
    cmpip_o[3].write(regs->r_timcfg.timcmp3ip != 0);
    last_cnt_update = now;
    last_clk = clk;
}

template<unsigned COUNTER_WIDTH, unsigned TICK_CNT, unsigned CLEAR_CNT>
void tim<COUNTER_WIDTH, TICK_CNT, CLEAR_CNT>::write_cmpgpio(size_t index, bool val) {
    if (cmpgpio_o[index].get_interface()) {
        tlm::tlm_phase phase(tlm::BEGIN_REQ);
        tlm::scc::tlm_signal_gp<> gp;
        sc_core::sc_time delay(SC_ZERO_TIME);
        gp.set_value(val);
        cmpgpio_o[index]->nb_transport_fw(gp, phase, delay);
    }
}

} /* namespace minres */
} /* namespace vpvper */

#endif /* _TIM_H_ */
