/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _TIM_REGS_H_
#define _TIM_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

template<unsigned COUNTER_WIDTH=32, unsigned TICK_CNT=1, unsigned CLEAR_CNT=1>
class tim_regs : public sc_core::sc_module, public scc::resetable {
public:
    static_assert(COUNTER_WIDTH<32);
    static_assert(TICK_CNT<16);
    static_assert(CLEAR_CNT<16);

    // storage declarations
    BEGIN_BF_DECL(clk_ctrl_t, uint32_t);
    BF_FIELD(clk_en, 0, TICK_CNT);
    BF_FIELD(clr_en, 16, CLEAR_CNT);
    END_BF_DECL() r_clk_ctrl;

    uint32_t r_limit;

    uint32_t r_timer_value;

    // register declarations
    scc::sc_register<clk_ctrl_t> clk_ctrl;
    scc::sc_register<uint32_t> limit;
    scc::sc_register<uint32_t> timer_value;

    tim_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);

    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace minres */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
inline vpvper::minres::tim_regs<COUNTER_WIDTH, EXT_CTRL_CNT>::tim_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(clk_ctrl, r_clk_ctrl, 0, *this)
, NAMED(limit, r_limit, 0, *this)
, NAMED(timer_value, r_timer_value, 0, *this)
 {}

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
template <unsigned BUSWIDTH> inline void vpvper::minres::tim_regs<COUNTER_WIDTH, EXT_CTRL_CNT>::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(clk_ctrl, 0x0UL);
    target.addResource(limit, 0x4UL);
    target.addResource(timer_value, 0x8UL);
}

template<unsigned COUNTER_WIDTH, unsigned EXT_CTRL_CNT>
inline void vpvper::minres::tim_regs<COUNTER_WIDTH, EXT_CTRL_CNT>::trace(sc_core::sc_trace_file *tf) const {
    clk_ctrl.trace(tf);
    limit.trace(tf);
    timer_value.trace(tf);
}

#endif // _TIM_REGS_H_
