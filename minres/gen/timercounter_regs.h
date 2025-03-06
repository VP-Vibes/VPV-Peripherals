/*
 * Copyright (c) 2023 - 2025 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2025-01-21 12:45:36 UTC
 * by peakrdl_mnrs version 1.2.9
 */

#ifndef _TIMERCOUNTER_H_
#define _TIMERCOUNTER_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class timercounter_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(prescaler_t, uint32_t);
    BF_FIELD(limit, 0, 16);
    END_BF_DECL() r_prescaler;

    BEGIN_BF_DECL(t0_ctrl_t, uint32_t);
    BF_FIELD(enable, 0, 3);
    BF_FIELD(clear, 3, 2);
    END_BF_DECL() r_t0_ctrl;

    uint32_t r_t0_overflow;

    uint32_t r_t0_counter;

    BEGIN_BF_DECL(t1_ctrl_t, uint32_t);
    BF_FIELD(enable, 0, 3);
    BF_FIELD(clear, 3, 2);
    END_BF_DECL() r_t1_ctrl;

    uint32_t r_t1_overflow;

    uint32_t r_t1_counter;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    scc::sc_register<prescaler_t> prescaler;
    scc::sc_register<t0_ctrl_t> t0_ctrl;
    scc::sc_register<uint32_t> t0_overflow;
    scc::sc_register<uint32_t> t0_counter;
    scc::sc_register<t1_ctrl_t> t1_ctrl;
    scc::sc_register<uint32_t> t1_overflow;
    scc::sc_register<uint32_t> t1_counter;

    timercounter_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline timercounter_regs::timercounter_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(prescaler, r_prescaler, 0, *this)
, NAMED(t0_ctrl, r_t0_ctrl, 0, *this)
, NAMED(t0_overflow, r_t0_overflow, 0, *this)
, NAMED(t0_counter, r_t0_counter, 0, *this)
, NAMED(t1_ctrl, r_t1_ctrl, 0, *this)
, NAMED(t1_overflow, r_t1_overflow, 0, *this)
, NAMED(t1_counter, r_t1_counter, 0, *this) {}

template <unsigned BUSWIDTH> inline void timercounter_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(prescaler, 0x0UL);
    target.addResource(t0_ctrl, 0x4UL);
    target.addResource(t0_overflow, 0x8UL);
    target.addResource(t0_counter, 0xcUL);
    target.addResource(t1_ctrl, 0x10UL);
    target.addResource(t1_overflow, 0x14UL);
    target.addResource(t1_counter, 0x18UL);
}
} // namespace minres
} // namespace vpvper
#endif // _TIMERCOUNTER_H_