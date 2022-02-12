/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _AON_H_
#define _AON_H_

#include "scc/tlm_target.h"

namespace vpvper {
namespace sifive {

class aon_regs;

class aon : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(aon);// NOLINT
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> erst_n_i;
    sc_core::sc_out<sc_core::sc_time> lfclkc_o;
    sc_core::sc_out<bool> rst_o;
    aon(sc_core::sc_module_name nm);
    virtual ~aon() override; // need to keep it in source file because of fwd declaration of aon_regs

protected:
    void start_of_simulation() override;
    void clock_cb();
    void reset_cb();
    void reset_internal_cb();
    sc_core::sc_time clk;
    std::unique_ptr<aon_regs> regs;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _GPIO_H_ */
