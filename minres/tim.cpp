#include "tim.h"
namespace vpvper {

namespace minres {

tim::tim(sc_core::sc_module_name nm, unsigned counter_width, unsigned tick_cnt, unsigned clear_cnt)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, tick_i{"tick_i", tick_cnt }
, clear_i{ "clear_i", clear_cnt }
, regs(scc::make_unique<tim_regs>("regs"))
, counter_width(counter_width)
{
    using this_class = tim;
    SC_HAS_PROCESS(this_class);
    regs->registerResources(*this);
    regs->limit.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t &data, sc_core::sc_time d) -> bool {
        if (d.value())
            wait(d);
        reg.put(data & (1ULL << this->counter_width) - 1);
        update_counter_evt.notify(sc_core::SC_ZERO_TIME);
        return true;
    }
    );
    regs->timer_value.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time d) -> bool {
        data = counter & ((1ULL << this->counter_width) - 1);
        return true;
    });
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(update_counter);
    sensitive << update_counter_evt;
    for (auto &i : tick_i)
        sensitive << i;
    dont_initialize();
}

tim::~tim() = default;

void tim::reset_cb() {
    tick_o.write(false);
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

void tim::update_counter() {
    auto now = sc_core::sc_time_stamp();
    if (now == sc_core::SC_ZERO_TIME) return;

    update_counter_evt.cancel();
    auto any_clear = false;
    auto mask = 1U;
    for (auto pin : clear_i) {
        any_clear |= (regs->r_clk_ctrl.clr_en & mask) && pin.read();
        mask <<= 1;
    }
    if (any_clear) {
        counter = 0;
        tick_o = false;
    } else {
        if (regs->r_clk_ctrl.clk_en) {
            if (reset_cnt) {
                counter = 0;
                reset_cnt = false;
            } else if (last_enable)
                counter += 1;
            auto next_tick = sc_core::SC_ZERO_TIME;
            auto next_trig = sc_core::SC_ZERO_TIME;
            for (auto pin : tick_i) {
                if(regs->r_clk_ctrl.clk_en & mask) {
                    auto t = pin.read();
                    next_tick = std::min(next_tick, t);
                    if(t>sc_core::sc_time_stamp())
                        next_trig = std::min(next_trig, t);
                }
                mask <<= 1;
            }
            if(next_tick == sc_core::sc_time_stamp())
                last_enable = true;
            update_counter_evt.notify(next_trig);
        } else
            last_enable = false;
    }
    last_cnt_update = now;
    last_clk_period = clk_period;
}

void tim::write_cmpgpio(size_t index, bool val) {
}

}
}
