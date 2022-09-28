/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>

namespace vpvper {
namespace minres {

class gpio_regs;

class gpio : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(gpio);// NOLINT

    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_vector<sc_core::sc_out<bool>> pins_o{"pins_o",32};

    sc_core::sc_vector<sc_core::sc_out<bool>> oe_o{"oe_o", 32};

    sc_core::sc_vector<sc_core::sc_in<bool>> pins_i{"pins_i", 32};

    gpio(sc_core::sc_module_name nm);

    virtual ~gpio() override; // need to keep it in source file because of fwd declaration of gpio_regs

    void set_clock_period(sc_core::sc_time period) {
        clk_period=period;
    }

protected:
    void reset_cb();
    sc_core::sc_time clk_period;
    std::unique_ptr<gpio_regs> regs;
};

using gpio_tl = scc::tickless_clock<gpio>;
using gpio_tc = scc::ticking_clock<gpio>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _GPIO_H_ */
