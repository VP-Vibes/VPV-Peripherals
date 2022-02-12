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
// Created on: Sun Feb 13 08:28:03 CET 2022
//             *      timer.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSC_GEN_TIMER_H_
#define _SYSC_GEN_TIMER_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace sysc {
namespace gen {

class timer_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(CFG_LO_t, uint32_t);
    END_BF_DECL() r_CFG_LO;
    BEGIN_BF_DECL(CFG_HI_t, uint32_t);
    END_BF_DECL() r_CFG_HI;
    BEGIN_BF_DECL(CNT_LO_t, uint32_t);
    END_BF_DECL() r_CNT_LO;
    BEGIN_BF_DECL(CNT_HI_t, uint32_t);
    END_BF_DECL() r_CNT_HI;
    BEGIN_BF_DECL(CMP_LO_t, uint32_t);
    END_BF_DECL() r_CMP_LO;
    BEGIN_BF_DECL(CMP_HI_t, uint32_t);
    END_BF_DECL() r_CMP_HI;
    BEGIN_BF_DECL(START_LO_t, uint32_t);
    END_BF_DECL() r_START_LO;
    BEGIN_BF_DECL(START_HI_t, uint32_t);
    END_BF_DECL() r_START_HI;
    BEGIN_BF_DECL(RESET_LO_t, uint32_t);
    END_BF_DECL() r_RESET_LO;
    BEGIN_BF_DECL(RESET_HI_t, uint32_t);
    END_BF_DECL() r_RESET_HI;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<CFG_LO_t> CFG_LO;
    scc::sc_register<CFG_HI_t> CFG_HI;
    scc::sc_register<CNT_LO_t> CNT_LO;
    scc::sc_register<CNT_HI_t> CNT_HI;
    scc::sc_register<CMP_LO_t> CMP_LO;
    scc::sc_register<CMP_HI_t> CMP_HI;
    scc::sc_register<START_LO_t> START_LO;
    scc::sc_register<START_HI_t> START_HI;
    scc::sc_register<RESET_LO_t> RESET_LO;
    scc::sc_register<RESET_HI_t> RESET_HI;
    
    timer_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace sysc
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline sysc::gen::timer_regs::timer_regs(sc_core::sc_module_name nm)
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
inline void sysc::gen::timer_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(CFG_LO, 0x0UL);
    target.addResource(CFG_HI, 0x4UL);
    target.addResource(CNT_LO, 0x8UL);
    target.addResource(CNT_HI, 0xcUL);
    target.addResource(CMP_LO, 0x10UL);
    target.addResource(CMP_HI, 0x14UL);
    target.addResource(START_LO, 0x18UL);
    target.addResource(START_HI, 0x1cUL);
    target.addResource(RESET_LO, 0x20UL);
    target.addResource(RESET_HI, 0x24UL);
}

#endif // _SYSC_GEN_TIMER_H_
