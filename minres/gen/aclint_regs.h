/*
 * Copyright (c) 2023 - 2025 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2025-01-21 12:45:36 UTC
 * by peakrdl_mnrs version 1.2.9
 */

#ifndef _ACLINT_H_
#define _ACLINT_H_

#include <limits>
#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <sysc/utils/sc_vector.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class aclint_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(msip0_t, uint32_t);
    BF_FIELD(msip, 0, 1);
    END_BF_DECL();
    std::vector<msip0_t> r_msip;

    std::vector<uint64_t> r_mtimecmp;

    uint64_t r_mtime;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    sc_core::sc_vector<scc::sc_register<msip0_t>> msip;
    sc_core::sc_vector<scc::sc_register<uint64_t>> mtimecmp;
    scc::sc_register<uint64_t> mtime;

    aclint_regs(sc_core::sc_module_name nm, size_t num_cpus);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline aclint_regs::aclint_regs(sc_core::sc_module_name nm, size_t num_cpus)
: sc_core::sc_module(nm)
, r_msip(num_cpus)
, r_mtimecmp(num_cpus)
, msip("msip", num_cpus, [this](char const* name, int idx) { return new scc::sc_register<msip0_t>(name, r_msip[idx], 0, *this); })
, mtimecmp("mtimecmp", num_cpus,
           [this](char const* name, int idx) {
               return new scc::sc_register<uint64_t>(name, r_mtimecmp[idx], std::numeric_limits<uint64_t>::max(), *this);
           })
, NAMED(mtime, r_mtime, 0, *this) {}

template <unsigned BUSWIDTH> inline void aclint_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    constexpr size_t msie_sz = sizeof(decltype(r_msip)::value_type::backing);
    constexpr size_t mtimecmp_sz = sizeof(decltype(r_mtimecmp)::value_type);
    for(auto i = 0u; i < mtimecmp.size(); ++i) {
        target.addResource(msip[i], 0x0UL + msie_sz * i);
        target.addResource(mtimecmp[i], 0x4000UL + mtimecmp_sz * i);
    }
    target.addResource(mtime, 0xbff8UL);
}
} // namespace minres
} // namespace vpvper
#endif // _ACLINT_H_