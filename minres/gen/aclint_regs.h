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

    std::vector<uint32_t> r_mtimecmplo;

    std::vector<uint32_t> r_mtimecmphi;

    uint32_t r_mtime_lo;

    uint32_t r_mtime_hi;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    sc_core::sc_vector<scc::sc_register<msip0_t>> msip;
    sc_core::sc_vector<scc::sc_register<uint32_t>> mtimecmplo;
    sc_core::sc_vector<scc::sc_register<uint32_t>> mtimecmphi;
    scc::sc_register<uint32_t> mtime_lo;
    scc::sc_register<uint32_t> mtime_hi;

    aclint_regs(sc_core::sc_module_name nm, size_t num_cpus);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline aclint_regs::aclint_regs(sc_core::sc_module_name nm, size_t num_cpus)
: sc_core::sc_module(nm)
, r_msip(num_cpus)
, r_mtimecmplo(num_cpus)
, r_mtimecmphi(num_cpus)
, msip("msip", num_cpus, [this](char const* name, int idx) { return new scc::sc_register<msip0_t>(name, r_msip[idx], 0, *this); })
, mtimecmplo("mtimecmplo", num_cpus,
             [this](char const* name, int idx) { return new scc::sc_register<msip0_t>(name, r_mtimecmplo[idx], 0, *this); })
, mtimecmphi("mtimecmphi", num_cpus,
             [this](char const* name, int idx) { return new scc::sc_register<msip0_t>(name, r_mtimecmphi[idx], 0, *this); })
, NAMED(mtime_lo, r_mtime_lo, 0, *this)
, NAMED(mtime_hi, r_mtime_hi, 0, *this) {}

template <unsigned BUSWIDTH> inline void aclint_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    const size_t msie_sz = sizeof(decltype(r_msip)::value_type::backing);
    const size_t mtimecmplo_sz = sizeof(decltype(r_mtimecmplo)::value_type);
    const size_t mtimecmphi_sz = sizeof(decltype(r_mtimecmphi)::value_type);
    for(auto i = 0u; i < mtimecmplo.size(); ++i) {
        target.addResource(msip[i], 0x0UL + msie_sz * i);
        target.addResource(mtimecmplo[i], 0x4000UL + (mtimecmplo_sz + mtimecmphi_sz) * i);
        target.addResource(mtimecmphi[i], 0x4000UL + mtimecmplo_sz + (mtimecmplo_sz + mtimecmphi_sz) * i);
    }
    target.addResource(mtime_lo, 0xbff8UL);
    target.addResource(mtime_hi, 0xbffcUL);
}
} // namespace minres
} // namespace vpvper
#endif // _ACLINT_H_