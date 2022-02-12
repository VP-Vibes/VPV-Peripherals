/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PLIC_REGS_H_
#define _PLIC_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class plic_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(priority_t, uint32_t);
    BF_FIELD(priority, 0, 3);
    END_BF_DECL();
    std::array<priority_t, 256> r_priority;

    std::array<uint32_t, 8> r_pending;

    std::array<uint32_t, 8> r_enabled;

    BEGIN_BF_DECL(threshold_t, uint32_t);
    BF_FIELD(threshold, 0, 3);
    END_BF_DECL() r_threshold;

    uint32_t r_claim_complete;

    // register declarations
    scc::sc_register_indexed<priority_t, 256> priority;
    scc::sc_register_indexed<uint32_t, 8> pending;
    scc::sc_register_indexed<uint32_t, 8> enabled;
    scc::sc_register<threshold_t> threshold;
    scc::sc_register<uint32_t> claim_complete;

    plic_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::plic_regs::plic_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(priority, r_priority, 0, *this)
, NAMED(pending, r_pending, 0, *this)
, NAMED(enabled, r_enabled, 0, *this)
, NAMED(threshold, r_threshold, 0, *this)
, NAMED(claim_complete, r_claim_complete, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::plic_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(priority, 0x0UL);
    target.addResource(pending, 0x1000UL);
    target.addResource(enabled, 0x2000UL);
    target.addResource(threshold, 0x200000UL);
    target.addResource(claim_complete, 0x200004UL);
}

#endif // _PLIC_REGS_H_
