/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:38 CET 2023
 *             *      soc_event.h Author: <RDL Generator>
 *
 */

#pragma once

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace vpvper {
namespace pulpissimo {
namespace gen {

class soc_event_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(SW_EVENT_t, uint32_t);
        BF_FIELD(EVENT, 0, 8);
    END_BF_DECL() r_SW_EVENT;
    std::array<uint32_t, 8> r_FC_MASK;
    std::array<uint32_t, 8> r_PR_MASK;
    std::array<uint32_t, 8> r_ERR;
    BEGIN_BF_DECL(TIMER_LO_t, uint32_t);
        BF_FIELD(TIMER_LO_EVENT, 0, 8);
    END_BF_DECL() r_TIMER_LO;
    BEGIN_BF_DECL(TIMER_HI_t, uint32_t);
        BF_FIELD(TIMER_HI_EVENT, 0, 8);
    END_BF_DECL() r_TIMER_HI;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<SW_EVENT_t> SW_EVENT;
    scc::sc_register_indexed<uint32_t, 8> FC_MASK;
    scc::sc_register_indexed<uint32_t, 8> PR_MASK;
    scc::sc_register_indexed<uint32_t, 8> ERR;
    scc::sc_register<TIMER_LO_t> TIMER_LO;
    scc::sc_register<TIMER_HI_t> TIMER_HI;
    
    soc_event_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
} // namespace vpvper
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::soc_event_regs::soc_event_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(SW_EVENT, r_SW_EVENT, 0, *this)
, NAMED(FC_MASK, r_FC_MASK, 0, *this)
, NAMED(PR_MASK, r_PR_MASK, 0, *this)
, NAMED(ERR, r_ERR, 0, *this)
, NAMED(TIMER_LO, r_TIMER_LO, 0, *this)
, NAMED(TIMER_HI, r_TIMER_HI, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::soc_event_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(SW_EVENT, 0x0UL+offset);
    target.addResource(FC_MASK, 0x4UL+offset);
    target.addResource(PR_MASK, 0x44UL+offset);
    target.addResource(ERR, 0x64UL+offset);
    target.addResource(TIMER_LO, 0x84UL+offset);
    target.addResource(TIMER_HI, 0x88UL+offset);
}

