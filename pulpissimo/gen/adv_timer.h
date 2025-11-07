/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      adv_timer.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_ADV_TIMER_H_
#define _PULPISSIMO_GEN_ADV_TIMER_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace pulpissimo {
namespace gen {

class adv_timer_regs : public sc_core::sc_module, public scc::resetable {
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

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset = 0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::adv_timer_regs::adv_timer_regs(sc_core::sc_module_name nm)
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
, NAMED(CG, r_CG, 0, *this) {}

template <unsigned BUSWIDTH>
inline void pulpissimo::gen::adv_timer_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(T0_CMD, 0x0UL);
    target.addResource(T0_CONFIG, 0x4UL);
    target.addResource(T0_THRESHOLD, 0x8UL);
    target.addResource(T0_TH_CHANNEL0, 0xcUL);
    target.addResource(T0_TH_CHANNEL1, 0x10UL);
    target.addResource(T0_TH_CHANNEL2, 0x14UL);
    target.addResource(T0_TH_CHANNEL3, 0x18UL);
    target.addResource(T0_COUNTER, 0x2cUL);
    target.addResource(T1_CMD, 0x40UL);
    target.addResource(T1_CONFIG, 0x44UL);
    target.addResource(T1_THRESHOLD, 0x48UL);
    target.addResource(T1_TH_CHANNEL0, 0x4cUL);
    target.addResource(T1_TH_CHANNEL1, 0x50UL);
    target.addResource(T1_TH_CHANNEL2, 0x54UL);
    target.addResource(T1_TH_CHANNEL3, 0x58UL);
    target.addResource(T1_COUNTER, 0x6cUL);
    target.addResource(T2_CMD, 0x80UL);
    target.addResource(T2_CONFIG, 0x84UL);
    target.addResource(T2_THRESHOLD, 0x88UL);
    target.addResource(T2_TH_CHANNEL0, 0x8cUL);
    target.addResource(T2_TH_CHANNEL1, 0x90UL);
    target.addResource(T2_TH_CHANNEL2, 0x94UL);
    target.addResource(T2_TH_CHANNEL3, 0x98UL);
    target.addResource(T2_COUNTER, 0xacUL);
    target.addResource(T3_CMD, 0xc0UL);
    target.addResource(T3_CONFIG, 0xc4UL);
    target.addResource(T3_THRESHOLD, 0xc8UL);
    target.addResource(T3_TH_CHANNEL0, 0xccUL);
    target.addResource(T3_TH_CHANNEL1, 0xd0UL);
    target.addResource(T3_TH_CHANNEL2, 0xd4UL);
    target.addResource(T3_TH_CHANNEL3, 0xd8UL);
    target.addResource(T3_COUNTER, 0xecUL);
    target.addResource(EVENT_CFG, 0x100UL);
    target.addResource(CG, 0x104UL);
}

#endif // _PULPISSIMO_GEN_ADV_TIMER_H_
