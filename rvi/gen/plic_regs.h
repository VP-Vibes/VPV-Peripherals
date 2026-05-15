/*
 * Copyright (c) MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _RVI_PLIC_REGS_H_
#define _RVI_PLIC_REGS_H_

#include <stdint.h>

#include <array>
#include <vector>

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace rvi {

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
class plic_regs : public sc_core::sc_module, public scc::resetable {
   public:
    static constexpr uint32_t NUM_PENDING = (NUM_SOURCES + 31) / 32;
    static constexpr uint32_t NUM_ENABLED = NUM_PENDING;

    using Reg32 = scc::sc_register<uint32_t>;
    using EnReg = scc::sc_register_indexed<uint32_t, NUM_ENABLED>;

    // storage declarations
    std::array<uint32_t, NUM_SOURCES> r_priority;
    std::array<uint32_t, NUM_PENDING> r_pending;
    std::array<std::array<uint32_t, NUM_ENABLED>, NUM_CONTEXTS> r_enabled;
    std::array<uint32_t, NUM_CONTEXTS> r_threshold;
    std::array<uint32_t, NUM_CONTEXTS> r_claim_complete;

    // register declarations
    scc::sc_register_indexed<uint32_t, NUM_SOURCES> priority;
    scc::sc_register_indexed<uint32_t, NUM_PENDING> pending;
    std::vector<std::unique_ptr<EnReg>> enabled;
    std::vector<std::unique_ptr<Reg32>> threshold;
    std::vector<std::unique_ptr<Reg32>> claim_complete;

    plic_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
plic_regs<NUM_SOURCES, NUM_CONTEXTS>::plic_regs(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm),
      NAMED(priority, r_priority, 0, *this),
      NAMED(pending, r_pending, 0, *this, UINT32_MAX, 0) {
    for (uint32_t i = 0; i < NUM_CONTEXTS; ++i) {
        auto nm_str = "enabled" + std::to_string(i);
        enabled.emplace_back(std::make_unique<EnReg>(nm_str.c_str(), r_enabled[i], 0, *this));
        nm_str = "threshold" + std::to_string(i);
        threshold.emplace_back(std::make_unique<Reg32>(nm_str.c_str(), r_threshold[i], 0, *this));
        nm_str = "claim_complete" + std::to_string(i);
        claim_complete.emplace_back(std::make_unique<Reg32>(nm_str.c_str(), r_claim_complete[i], 0, *this));
    }
}

template <uint32_t NUM_SOURCES, uint32_t NUM_CONTEXTS>
template <unsigned BUSWIDTH>
inline void plic_regs<NUM_SOURCES, NUM_CONTEXTS>::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(priority, 0x0UL);
    target.addResource(pending, 0x1000UL);
    for (uint32_t i = 0; i < NUM_CONTEXTS; ++i) {
        target.addResource(*enabled[i], 0x2000UL + (0x80UL * i));
        target.addResource(*threshold[i], 0x200000UL + (0x1000UL * i));
        target.addResource(*claim_complete[i], 0x200004UL + (0x1000UL * i));
    }
}

}  // namespace rvi
}  // namespace vpvper

#endif  // _RVI_PLIC_REGS_H_
