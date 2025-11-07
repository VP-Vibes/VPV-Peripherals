/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PULPISSIMO_INTERRUPT_H_
#define _PULPISSIMO_INTERRUPT_H_
#include "scc/tlm_target.h"

namespace pulpissimo {
namespace gen {
class interrupt_regs;
}

class interrupt : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    interrupt(sc_core::sc_module_name nm);
    virtual ~interrupt() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::interrupt_regs> regs;
};

} /* namespace pulpissimo */

#endif /* _PULPISSIMO_INTERRUPT_H_ */
