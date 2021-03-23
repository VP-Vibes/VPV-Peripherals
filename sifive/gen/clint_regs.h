/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _CLINT_REGS_H_
#define _CLINT_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class clint_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(msip_t, uint32_t);
    BF_FIELD(msip, 0, 1);
    END_BF_DECL() r_msip;

    uint64_t r_mtimecmp;

    uint64_t r_mtime;

    // register declarations
    scc::sc_register<msip_t> msip;
    scc::sc_register<uint64_t> mtimecmp;
    scc::sc_register<uint64_t> mtime;

    clint_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::clint_regs::clint_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(msip, r_msip, 0, *this)
, NAMED(mtimecmp, r_mtimecmp, 0, *this)
, NAMED(mtime, r_mtime, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::clint_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(msip, 0x0UL);
    target.addResource(mtimecmp, 0x4000UL);
    target.addResource(mtime, 0xbff8UL);
}

#endif // _CLINT_REGS_H_
