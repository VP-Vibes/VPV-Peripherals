/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _IRQ_H_
#define _IRQ_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>

namespace vpvper {
namespace minres {

class Apb3IrqCtrl_regs;

class irq : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_out<bool> irq_o{"irq_o"};

    sc_core::sc_vector<sc_core::sc_in<bool>> pending_irq_i{"pending_irq_i"};

    irq(sc_core::sc_module_name nm, unsigned irq_width=32);

    virtual ~irq() override; // need to keep it in source file because of fwd declaration of irq_regs

    void set_clock_period(sc_core::sc_time period) {
        clk_period=period;
    }

protected:
    void reset_cb();
    void update_irqs();
    sc_core::sc_time clk_period;
    std::unique_ptr<Apb3IrqCtrl_regs> regs;
};

using irq_tl = scc::tickless_clock<irq>;
using irq_tc = scc::ticking_clock<irq>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _IRQ_H_ */
