#include "presc.h"
namespace vpvper {

namespace minres {

presc::presc(sc_core::sc_module_name nm, unsigned counter_width)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, regs(scc::make_unique<presc_regs>("regs"))
, counter_width(counter_width)
{
    using this_class = presc;
    SC_HAS_PROCESS(this_class);
    regs->registerResources(*this);
    regs->limit.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t &data, sc_core::sc_time d) -> bool {
        if (d.value()) wait(d);
        reg.put(data & (1ULL << this->counter_width) - 1);
        update_counter_evt.notify(sc_core::SC_ZERO_TIME);
        return true;
    }
    );
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(update_counter);
    sensitive << update_counter_evt;
    dont_initialize();
}

presc::~presc() = default;

void presc::reset_cb() {
    overflow_o.write(sc_core::SC_ZERO_TIME);
    if (rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

void presc::update_counter() {
    auto now = sc_core::sc_time_stamp();
    if (now == sc_core::SC_ZERO_TIME) return;

    update_counter_evt.cancel();
    if (regs->limit > 0) {
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
        } else
            if (last_enable) counter += pulses;

        auto next_trigger_time = (regs->limit - counter) * clk_period; // next trigger based on wrap around
        if (counter == regs->limit) {
            // wrap around calculation
            reset_cnt = true;
            next_trigger_time = clk_period;
        } else {
            overflow_o.write(sc_core::sc_time_stamp() + next_trigger_time);
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

}

}
