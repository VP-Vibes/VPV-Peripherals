////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017-2022, MINRES Technologies GmbH
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Created on: Sun Feb 13 08:59:06 CET 2022
//             *      adv_timer.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_ADV_TIMER_H_
#define _PULPISSIMO_GEN_ADV_TIMER_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

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
    BEGIN_BF_DECL(T0_CMD_t, uint32_t);
    END_BF_DECL() r_T0_CMD;
    BEGIN_BF_DECL(T0_CONFIG_t, uint32_t);
    END_BF_DECL() r_T0_CONFIG;
    BEGIN_BF_DECL(T0_THRESHOLD_t, uint32_t);
    END_BF_DECL() r_T0_THRESHOLD;
    BEGIN_BF_DECL(T0_TH_CHANNEL0_t, uint32_t);
    END_BF_DECL() r_T0_TH_CHANNEL0;
    BEGIN_BF_DECL(T0_TH_CHANNEL1_t, uint32_t);
    END_BF_DECL() r_T0_TH_CHANNEL1;
    BEGIN_BF_DECL(T0_TH_CHANNEL2_t, uint32_t);
    END_BF_DECL() r_T0_TH_CHANNEL2;
    BEGIN_BF_DECL(T0_TH_CHANNEL3_t, uint32_t);
    END_BF_DECL() r_T0_TH_CHANNEL3;
    BEGIN_BF_DECL(T0_COUNTER_t, uint32_t);
    END_BF_DECL() r_T0_COUNTER;
    BEGIN_BF_DECL(T1_CMD_t, uint32_t);
    END_BF_DECL() r_T1_CMD;
    BEGIN_BF_DECL(T1_CONFIG_t, uint32_t);
    END_BF_DECL() r_T1_CONFIG;
    BEGIN_BF_DECL(T1_THRESHOLD_t, uint32_t);
    END_BF_DECL() r_T1_THRESHOLD;
    BEGIN_BF_DECL(T1_TH_CHANNEL0_t, uint32_t);
    END_BF_DECL() r_T1_TH_CHANNEL0;
    BEGIN_BF_DECL(T1_TH_CHANNEL1_t, uint32_t);
    END_BF_DECL() r_T1_TH_CHANNEL1;
    BEGIN_BF_DECL(T1_TH_CHANNEL2_t, uint32_t);
    END_BF_DECL() r_T1_TH_CHANNEL2;
    BEGIN_BF_DECL(T1_TH_CHANNEL3_t, uint32_t);
    END_BF_DECL() r_T1_TH_CHANNEL3;
    BEGIN_BF_DECL(T1_COUNTER_t, uint32_t);
    END_BF_DECL() r_T1_COUNTER;
    BEGIN_BF_DECL(T2_CMD_t, uint32_t);
    END_BF_DECL() r_T2_CMD;
    BEGIN_BF_DECL(T2_CONFIG_t, uint32_t);
    END_BF_DECL() r_T2_CONFIG;
    BEGIN_BF_DECL(T2_THRESHOLD_t, uint32_t);
    END_BF_DECL() r_T2_THRESHOLD;
    BEGIN_BF_DECL(T2_TH_CHANNEL0_t, uint32_t);
    END_BF_DECL() r_T2_TH_CHANNEL0;
    BEGIN_BF_DECL(T2_TH_CHANNEL1_t, uint32_t);
    END_BF_DECL() r_T2_TH_CHANNEL1;
    BEGIN_BF_DECL(T2_TH_CHANNEL2_t, uint32_t);
    END_BF_DECL() r_T2_TH_CHANNEL2;
    BEGIN_BF_DECL(T2_TH_CHANNEL3_t, uint32_t);
    END_BF_DECL() r_T2_TH_CHANNEL3;
    BEGIN_BF_DECL(T2_COUNTER_t, uint32_t);
    END_BF_DECL() r_T2_COUNTER;
    BEGIN_BF_DECL(T3_CMD_t, uint32_t);
    END_BF_DECL() r_T3_CMD;
    BEGIN_BF_DECL(T3_CONFIG_t, uint32_t);
    END_BF_DECL() r_T3_CONFIG;
    BEGIN_BF_DECL(T3_THRESHOLD_t, uint32_t);
    END_BF_DECL() r_T3_THRESHOLD;
    BEGIN_BF_DECL(T3_TH_CHANNEL0_t, uint32_t);
    END_BF_DECL() r_T3_TH_CHANNEL0;
    BEGIN_BF_DECL(T3_TH_CHANNEL1_t, uint32_t);
    END_BF_DECL() r_T3_TH_CHANNEL1;
    BEGIN_BF_DECL(T3_TH_CHANNEL2_t, uint32_t);
    END_BF_DECL() r_T3_TH_CHANNEL2;
    BEGIN_BF_DECL(T3_TH_CHANNEL3_t, uint32_t);
    END_BF_DECL() r_T3_TH_CHANNEL3;
    BEGIN_BF_DECL(T3_COUNTER_t, uint32_t);
    END_BF_DECL() r_T3_COUNTER;
    BEGIN_BF_DECL(EVENT_CFG_t, uint32_t);
    END_BF_DECL() r_EVENT_CFG;
    BEGIN_BF_DECL(CG_t, uint32_t);
    END_BF_DECL() r_CG;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<T0_CMD_t> T0_CMD;
    scc::sc_register<T0_CONFIG_t> T0_CONFIG;
    scc::sc_register<T0_THRESHOLD_t> T0_THRESHOLD;
    scc::sc_register<T0_TH_CHANNEL0_t> T0_TH_CHANNEL0;
    scc::sc_register<T0_TH_CHANNEL1_t> T0_TH_CHANNEL1;
    scc::sc_register<T0_TH_CHANNEL2_t> T0_TH_CHANNEL2;
    scc::sc_register<T0_TH_CHANNEL3_t> T0_TH_CHANNEL3;
    scc::sc_register<T0_COUNTER_t> T0_COUNTER;
    scc::sc_register<T1_CMD_t> T1_CMD;
    scc::sc_register<T1_CONFIG_t> T1_CONFIG;
    scc::sc_register<T1_THRESHOLD_t> T1_THRESHOLD;
    scc::sc_register<T1_TH_CHANNEL0_t> T1_TH_CHANNEL0;
    scc::sc_register<T1_TH_CHANNEL1_t> T1_TH_CHANNEL1;
    scc::sc_register<T1_TH_CHANNEL2_t> T1_TH_CHANNEL2;
    scc::sc_register<T1_TH_CHANNEL3_t> T1_TH_CHANNEL3;
    scc::sc_register<T1_COUNTER_t> T1_COUNTER;
    scc::sc_register<T2_CMD_t> T2_CMD;
    scc::sc_register<T2_CONFIG_t> T2_CONFIG;
    scc::sc_register<T2_THRESHOLD_t> T2_THRESHOLD;
    scc::sc_register<T2_TH_CHANNEL0_t> T2_TH_CHANNEL0;
    scc::sc_register<T2_TH_CHANNEL1_t> T2_TH_CHANNEL1;
    scc::sc_register<T2_TH_CHANNEL2_t> T2_TH_CHANNEL2;
    scc::sc_register<T2_TH_CHANNEL3_t> T2_TH_CHANNEL3;
    scc::sc_register<T2_COUNTER_t> T2_COUNTER;
    scc::sc_register<T3_CMD_t> T3_CMD;
    scc::sc_register<T3_CONFIG_t> T3_CONFIG;
    scc::sc_register<T3_THRESHOLD_t> T3_THRESHOLD;
    scc::sc_register<T3_TH_CHANNEL0_t> T3_TH_CHANNEL0;
    scc::sc_register<T3_TH_CHANNEL1_t> T3_TH_CHANNEL1;
    scc::sc_register<T3_TH_CHANNEL2_t> T3_TH_CHANNEL2;
    scc::sc_register<T3_TH_CHANNEL3_t> T3_TH_CHANNEL3;
    scc::sc_register<T3_COUNTER_t> T3_COUNTER;
    scc::sc_register<EVENT_CFG_t> EVENT_CFG;
    scc::sc_register<CG_t> CG;
    
    adv_timer_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
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
, NAMED(CG, r_CG, 0, *this)
{
}

template<unsigned BUSWIDTH>
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
