/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:41 CET 2023
 *             *      timer.h Author: <RDL Generator>
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

class timer_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_CFG_LO;
    uint32_t r_CFG_HI;
    uint32_t r_CNT_LO;
    uint32_t r_CNT_HI;
    uint32_t r_CMP_LO;
    uint32_t r_CMP_HI;
    uint32_t r_START_LO;
    uint32_t r_START_HI;
    uint32_t r_RESET_LO;
    uint32_t r_RESET_HI;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> CFG_LO;
    scc::sc_register<uint32_t> CFG_HI;
    scc::sc_register<uint32_t> CNT_LO;
    scc::sc_register<uint32_t> CNT_HI;
    scc::sc_register<uint32_t> CMP_LO;
    scc::sc_register<uint32_t> CMP_HI;
    scc::sc_register<uint32_t> START_LO;
    scc::sc_register<uint32_t> START_HI;
    scc::sc_register<uint32_t> RESET_LO;
    scc::sc_register<uint32_t> RESET_HI;
    
    timer_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
} // namespace vpvper
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::timer_regs::timer_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(CFG_LO, r_CFG_LO, 0, *this)
, NAMED(CFG_HI, r_CFG_HI, 0, *this)
, NAMED(CNT_LO, r_CNT_LO, 0, *this)
, NAMED(CNT_HI, r_CNT_HI, 0, *this)
, NAMED(CMP_LO, r_CMP_LO, 0, *this)
, NAMED(CMP_HI, r_CMP_HI, 0, *this)
, NAMED(START_LO, r_START_LO, 0, *this)
, NAMED(START_HI, r_START_HI, 0, *this)
, NAMED(RESET_LO, r_RESET_LO, 0, *this)
, NAMED(RESET_HI, r_RESET_HI, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::timer_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(CFG_LO, 0x0UL+offset);
    target.addResource(CFG_HI, 0x4UL+offset);
    target.addResource(CNT_LO, 0x8UL+offset);
    target.addResource(CNT_HI, 0xcUL+offset);
    target.addResource(CMP_LO, 0x10UL+offset);
    target.addResource(CMP_HI, 0x14UL+offset);
    target.addResource(START_LO, 0x18UL+offset);
    target.addResource(START_HI, 0x1cUL+offset);
    target.addResource(RESET_LO, 0x20UL+offset);
    target.addResource(RESET_HI, 0x24UL+offset);
}

