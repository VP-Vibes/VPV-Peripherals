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

class tim_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    uint64_t r_mtime;

    uint64_t r_mtimecmp;

    // register declarations
    scc::sc_register<uint64_t> mtime;
    scc::sc_register<uint64_t> mtimecmp;

    tim_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);

    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace minres */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::minres::tim_regs::tim_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(mtime, r_mtime, 0, *this)
, NAMED(mtimecmp, r_mtimecmp, 0, *this)
 {}

template <unsigned BUSWIDTH> inline void vpvper::minres::tim_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(mtime, 0x0UL);
    target.addResource(mtimecmp, 0x8UL);
}

inline void vpvper::minres::tim_regs::trace(sc_core::sc_trace_file *tf) const {
    mtime.trace(tf);
    mtimecmp.trace(tf);
}

#endif // _GPIO_REGS_H_
