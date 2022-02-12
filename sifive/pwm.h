/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PWM_H_
#define _PWM_H_

#include "cci_configuration"
#include "tlm/scc/signal_initiator_mixin.h"
#include "tlm/scc/signal_target_mixin.h"
#include "scc/tlm_target.h"
#include <tlm/scc/tlm_signal.h>

namespace vpvper {
namespace sifive {

class pwm_regs;

class pwm : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(pwm);// NOLINT
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;

    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_out> cmpgpio_o;
    sc_core::sc_vector<sc_core::sc_out<bool>> cmpip_o;

    pwm(sc_core::sc_module_name nm);
    virtual ~pwm() override; // need to keep it in source file because of fwd declaration of gpio_regs

protected:
    sc_core::sc_time clk, last_clk;
    void clock_cb();
    void reset_cb();
    inline double get_pulses(sc_core::sc_time d) {
        auto t = sc_core::sc_time_stamp() + d;
        return last_clk > sc_core::SC_ZERO_TIME ? (t - last_cnt_update) / last_clk : 0.;
    }
    void update_counter();
    void write_cmpgpio(size_t, bool);
    std::unique_ptr<pwm_regs> regs;
    uint64_t current_cnt;
    sc_core::sc_time last_cnt_update;
    double clk_remainder = 0.0;
    bool last_enable = false, reset_cnt = false;
    sc_core::sc_event update_counter_evt;
    std::array<bool, 4> pwmcmp_ip;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _GPIO_H_ */
