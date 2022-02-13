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
// Created on: Sun Feb 13 12:33:59 CET 2022
//             *      soc_ctrl.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_SOC_CTRL_H_
#define _PULPISSIMO_GEN_SOC_CTRL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {

class soc_ctrl_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(INFO_t, uint32_t);
        BF_FIELD(NumOfCores, 16, 16);
        BF_FIELD(NumOfCluster, 0, 16);
    END_BF_DECL() r_INFO;
    uint32_t r_BOOT_ADR;
    BEGIN_BF_DECL(FETCH_ENABLE_t, uint32_t);
        BF_FIELD(E, 0, 1);
    END_BF_DECL() r_FETCH_ENABLE;
    std::array<uint32_t, 16> r_PAD_MUX;
    BEGIN_BF_DECL(JTAG_REG_t, uint32_t);
        BF_FIELD(JTAGRegIn, 8, 8);
        BF_FIELD(JTAGRegOut, 0, 8);
    END_BF_DECL() r_JTAG_REG;
    uint32_t r_CORE_STATUS;
    BEGIN_BF_DECL(FLL_CLOCK_SELECT_t, uint32_t);
        BF_FIELD(S, 0, 1);
    END_BF_DECL() r_FLL_CLOCK_SELECT;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<INFO_t> INFO;
    scc::sc_register<uint32_t> BOOT_ADR;
    scc::sc_register<FETCH_ENABLE_t> FETCH_ENABLE;
    scc::sc_register_indexed<uint32_t, 16> PAD_MUX;
    scc::sc_register<JTAG_REG_t> JTAG_REG;
    scc::sc_register<uint32_t> CORE_STATUS;
    scc::sc_register<FLL_CLOCK_SELECT_t> FLL_CLOCK_SELECT;
    
    soc_ctrl_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::soc_ctrl_regs::soc_ctrl_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(INFO, r_INFO, 0, *this)
, NAMED(BOOT_ADR, r_BOOT_ADR, 0, *this)
, NAMED(FETCH_ENABLE, r_FETCH_ENABLE, 0, *this)
, NAMED(PAD_MUX, r_PAD_MUX, 0, *this)
, NAMED(JTAG_REG, r_JTAG_REG, 0, *this)
, NAMED(CORE_STATUS, r_CORE_STATUS, 0, *this)
, NAMED(FLL_CLOCK_SELECT, r_FLL_CLOCK_SELECT, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::soc_ctrl_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(INFO, 0x0UL);
    target.addResource(BOOT_ADR, 0x4UL);
    target.addResource(FETCH_ENABLE, 0x8UL);
    target.addResource(PAD_MUX, 0x20UL);
    target.addResource(JTAG_REG, 0x74UL);
    target.addResource(CORE_STATUS, 0xc0UL);
    target.addResource(FLL_CLOCK_SELECT, 0xc8UL);
}

#endif // _PULPISSIMO_GEN_SOC_CTRL_H_
