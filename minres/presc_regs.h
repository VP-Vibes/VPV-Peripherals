/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _GPIO_REGS_H_
#define _GPIO_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

template<unsigned COUNTER_WIDTH=32>
class presc_regs : public sc_core::sc_module, public scc::resetable {
public:
    static_assert(COUNTER_WIDTH<32);
    // storage declarations
    uint32_t r_limit;

    // register declarations
    scc::sc_register<uint32_t> limit;

    presc_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);

    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace minres */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::minres::presc_regs::presc_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(limit, r_limit, 0, *this)
 {}

template <unsigned BUSWIDTH> inline void vpvper::minres::presc_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(limit, 0x0UL);
}

inline void vpvper::minres::presc_regs::trace(sc_core::sc_trace_file *tf) const {
    limit.trace(tf);
}

#endif // _GPIO_REGS_H_
