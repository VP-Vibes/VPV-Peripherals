/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PULPISSIMO_GPIO_H_
#define _PULPISSIMO_GPIO_H_
#include "scc/tlm_target.h"

namespace pulpissimo {
namespace gen {
class gpio_regs;
}

class gpio : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    sc_core::sc_vector<sc_core::sc_in<bool>>  in{"in", 64};
    sc_core::sc_vector<sc_core::sc_out<bool>> out{"out", 64};
    sc_core::sc_vector<sc_core::sc_out<bool>> dir{"dir", 64};
    sc_core::sc_out<bool> irq_o{"irq_o"};

    gpio(sc_core::sc_module_name nm);
    virtual ~gpio() override;

protected:
    void clock_cb();
    void reset_cb();
    void in_31_0_cb();
    void in_63_32_cb();
    void check_irq(unsigned bit, bool new_val);
    unsigned get_int_type(unsigned bit);
    sc_core::sc_time clk;
    std::unique_ptr<gen::gpio_regs> regs;
};

} /* namespace pulpissimo */

#endif /* _PULPISSIMO_GPIO_H_ */
