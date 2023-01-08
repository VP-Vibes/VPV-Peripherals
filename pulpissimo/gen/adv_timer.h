/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:33 CET 2023
 *             *      adv_timer.h Author: <RDL Generator>
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

class adv_timer_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_T0_CMD;
    uint32_t r_T0_CONFIG;
    uint32_t r_T0_THRESHOLD;
    uint32_t r_T0_TH_CHANNEL0;
    uint32_t r_T0_TH_CHANNEL1;
    uint32_t r_T0_TH_CHANNEL2;
    uint32_t r_T0_TH_CHANNEL3;
    uint32_t r_T0_COUNTER;
    uint32_t r_T1_CMD;
    uint32_t r_T1_CONFIG;
    uint32_t r_T1_THRESHOLD;
    uint32_t r_T1_TH_CHANNEL0;
    uint32_t r_T1_TH_CHANNEL1;
    uint32_t r_T1_TH_CHANNEL2;
    uint32_t r_T1_TH_CHANNEL3;
    uint32_t r_T1_COUNTER;
    uint32_t r_T2_CMD;
    uint32_t r_T2_CONFIG;
    uint32_t r_T2_THRESHOLD;
    uint32_t r_T2_TH_CHANNEL0;
    uint32_t r_T2_TH_CHANNEL1;
    uint32_t r_T2_TH_CHANNEL2;
    uint32_t r_T2_TH_CHANNEL3;
    uint32_t r_T2_COUNTER;
    uint32_t r_T3_CMD;
    uint32_t r_T3_CONFIG;
    uint32_t r_T3_THRESHOLD;
    uint32_t r_T3_TH_CHANNEL0;
    uint32_t r_T3_TH_CHANNEL1;
    uint32_t r_T3_TH_CHANNEL2;
    uint32_t r_T3_TH_CHANNEL3;
    uint32_t r_T3_COUNTER;
    uint32_t r_EVENT_CFG;
    uint32_t r_CG;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> T0_CMD;
    scc::sc_register<uint32_t> T0_CONFIG;
    scc::sc_register<uint32_t> T0_THRESHOLD;
    scc::sc_register<uint32_t> T0_TH_CHANNEL0;
    scc::sc_register<uint32_t> T0_TH_CHANNEL1;
    scc::sc_register<uint32_t> T0_TH_CHANNEL2;
    scc::sc_register<uint32_t> T0_TH_CHANNEL3;
    scc::sc_register<uint32_t> T0_COUNTER;
    scc::sc_register<uint32_t> T1_CMD;
    scc::sc_register<uint32_t> T1_CONFIG;
    scc::sc_register<uint32_t> T1_THRESHOLD;
    scc::sc_register<uint32_t> T1_TH_CHANNEL0;
    scc::sc_register<uint32_t> T1_TH_CHANNEL1;
    scc::sc_register<uint32_t> T1_TH_CHANNEL2;
    scc::sc_register<uint32_t> T1_TH_CHANNEL3;
    scc::sc_register<uint32_t> T1_COUNTER;
    scc::sc_register<uint32_t> T2_CMD;
    scc::sc_register<uint32_t> T2_CONFIG;
    scc::sc_register<uint32_t> T2_THRESHOLD;
    scc::sc_register<uint32_t> T2_TH_CHANNEL0;
    scc::sc_register<uint32_t> T2_TH_CHANNEL1;
    scc::sc_register<uint32_t> T2_TH_CHANNEL2;
    scc::sc_register<uint32_t> T2_TH_CHANNEL3;
    scc::sc_register<uint32_t> T2_COUNTER;
    scc::sc_register<uint32_t> T3_CMD;
    scc::sc_register<uint32_t> T3_CONFIG;
    scc::sc_register<uint32_t> T3_THRESHOLD;
    scc::sc_register<uint32_t> T3_TH_CHANNEL0;
    scc::sc_register<uint32_t> T3_TH_CHANNEL1;
    scc::sc_register<uint32_t> T3_TH_CHANNEL2;
    scc::sc_register<uint32_t> T3_TH_CHANNEL3;
    scc::sc_register<uint32_t> T3_COUNTER;
    scc::sc_register<uint32_t> EVENT_CFG;
    scc::sc_register<uint32_t> CG;
    
    adv_timer_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
} // namespace vpvper
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::adv_timer_regs::adv_timer_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(T0_CMD, r_T0_CMD, 0, *this)
, NAMED(T0_CONFIG, r_T0_CONFIG, 0, *this)
, NAMED(T0_THRESHOLD, r_T0_THRESHOLD, 0, *this)
, NAMED(T0_TH_CHANNEL0, r_T0_TH_CHANNEL0, 0, *this)
, NAMED(T0_TH_CHANNEL1, r_T0_TH_CHANNEL1, 0, *this)
, NAMED(T0_TH_CHANNEL2, r_T0_TH_CHANNEL2, 0, *this)
, NAMED(T0_TH_CHANNEL3, r_T0_TH_CHANNEL3, 0, *this)
, NAMED(T0_COUNTER, r_T0_COUNTER, 0, *this)
, NAMED(T1_CMD, r_T1_CMD, 0, *this)
, NAMED(T1_CONFIG, r_T1_CONFIG, 0, *this)
, NAMED(T1_THRESHOLD, r_T1_THRESHOLD, 0, *this)
, NAMED(T1_TH_CHANNEL0, r_T1_TH_CHANNEL0, 0, *this)
, NAMED(T1_TH_CHANNEL1, r_T1_TH_CHANNEL1, 0, *this)
, NAMED(T1_TH_CHANNEL2, r_T1_TH_CHANNEL2, 0, *this)
, NAMED(T1_TH_CHANNEL3, r_T1_TH_CHANNEL3, 0, *this)
, NAMED(T1_COUNTER, r_T1_COUNTER, 0, *this)
, NAMED(T2_CMD, r_T2_CMD, 0, *this)
, NAMED(T2_CONFIG, r_T2_CONFIG, 0, *this)
, NAMED(T2_THRESHOLD, r_T2_THRESHOLD, 0, *this)
, NAMED(T2_TH_CHANNEL0, r_T2_TH_CHANNEL0, 0, *this)
, NAMED(T2_TH_CHANNEL1, r_T2_TH_CHANNEL1, 0, *this)
, NAMED(T2_TH_CHANNEL2, r_T2_TH_CHANNEL2, 0, *this)
, NAMED(T2_TH_CHANNEL3, r_T2_TH_CHANNEL3, 0, *this)
, NAMED(T2_COUNTER, r_T2_COUNTER, 0, *this)
, NAMED(T3_CMD, r_T3_CMD, 0, *this)
, NAMED(T3_CONFIG, r_T3_CONFIG, 0, *this)
, NAMED(T3_THRESHOLD, r_T3_THRESHOLD, 0, *this)
, NAMED(T3_TH_CHANNEL0, r_T3_TH_CHANNEL0, 0, *this)
, NAMED(T3_TH_CHANNEL1, r_T3_TH_CHANNEL1, 0, *this)
, NAMED(T3_TH_CHANNEL2, r_T3_TH_CHANNEL2, 0, *this)
, NAMED(T3_TH_CHANNEL3, r_T3_TH_CHANNEL3, 0, *this)
, NAMED(T3_COUNTER, r_T3_COUNTER, 0, *this)
, NAMED(EVENT_CFG, r_EVENT_CFG, 0, *this)
, NAMED(CG, r_CG, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::adv_timer_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(T0_CMD, 0x0UL+offset);
    target.addResource(T0_CONFIG, 0x4UL+offset);
    target.addResource(T0_THRESHOLD, 0x8UL+offset);
    target.addResource(T0_TH_CHANNEL0, 0xcUL+offset);
    target.addResource(T0_TH_CHANNEL1, 0x10UL+offset);
    target.addResource(T0_TH_CHANNEL2, 0x14UL+offset);
    target.addResource(T0_TH_CHANNEL3, 0x18UL+offset);
    target.addResource(T0_COUNTER, 0x2cUL+offset);
    target.addResource(T1_CMD, 0x40UL+offset);
    target.addResource(T1_CONFIG, 0x44UL+offset);
    target.addResource(T1_THRESHOLD, 0x48UL+offset);
    target.addResource(T1_TH_CHANNEL0, 0x4cUL+offset);
    target.addResource(T1_TH_CHANNEL1, 0x50UL+offset);
    target.addResource(T1_TH_CHANNEL2, 0x54UL+offset);
    target.addResource(T1_TH_CHANNEL3, 0x58UL+offset);
    target.addResource(T1_COUNTER, 0x6cUL+offset);
    target.addResource(T2_CMD, 0x80UL+offset);
    target.addResource(T2_CONFIG, 0x84UL+offset);
    target.addResource(T2_THRESHOLD, 0x88UL+offset);
    target.addResource(T2_TH_CHANNEL0, 0x8cUL+offset);
    target.addResource(T2_TH_CHANNEL1, 0x90UL+offset);
    target.addResource(T2_TH_CHANNEL2, 0x94UL+offset);
    target.addResource(T2_TH_CHANNEL3, 0x98UL+offset);
    target.addResource(T2_COUNTER, 0xacUL+offset);
    target.addResource(T3_CMD, 0xc0UL+offset);
    target.addResource(T3_CONFIG, 0xc4UL+offset);
    target.addResource(T3_THRESHOLD, 0xc8UL+offset);
    target.addResource(T3_TH_CHANNEL0, 0xccUL+offset);
    target.addResource(T3_TH_CHANNEL1, 0xd0UL+offset);
    target.addResource(T3_TH_CHANNEL2, 0xd4UL+offset);
    target.addResource(T3_TH_CHANNEL3, 0xd8UL+offset);
    target.addResource(T3_COUNTER, 0xecUL+offset);
    target.addResource(EVENT_CFG, 0x100UL+offset);
    target.addResource(CG, 0x104UL+offset);
}

