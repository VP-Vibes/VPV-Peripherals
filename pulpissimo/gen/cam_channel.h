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
//             *      cam_channel.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_CAM_CHANNEL_H_
#define _PULPISSIMO_GEN_CAM_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {

class cam_channel_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_CAM_RX_SADDR;
    uint32_t r_CAM_RX_SIZE;
    BEGIN_BF_DECL(CAM_RX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_CAM_RX_CFG;
    BEGIN_BF_DECL(CAM_CFG_GLOB_t, uint32_t);
        BF_FIELD(CLR, 31, 1);
        BF_FIELD(SHIFT, 11, 4);
        BF_FIELD(FORMAT, 8, 3);
        BF_FIELD(FRAMESLICE_EN, 7, 1);
        BF_FIELD(FRAMEDROP_VAL, 1, 6);
        BF_FIELD(FRAMEDROP_EN, 0, 1);
    END_BF_DECL() r_CAM_CFG_GLOB;
    BEGIN_BF_DECL(CAM_CFG_LL_t, uint32_t);
        BF_FIELD(FRAMESLICE_LLY, 16, 16);
        BF_FIELD(FRAMESLICE_LLX, 0, 16);
    END_BF_DECL() r_CAM_CFG_LL;
    BEGIN_BF_DECL(CAM_CFG_UR_t, uint32_t);
        BF_FIELD(FRAMESLICE_URY, 16, 16);
        BF_FIELD(FRAMESLICE_URX, 0, 16);
    END_BF_DECL() r_CAM_CFG_UR;
    BEGIN_BF_DECL(CAM_CFG_SIZE_t, uint32_t);
        BF_FIELD(ROWLEN, 16, 16);
    END_BF_DECL() r_CAM_CFG_SIZE;
    BEGIN_BF_DECL(CAM_CFG_FILTER_t, uint32_t);
        BF_FIELD(R_COEFF, 16, 8);
        BF_FIELD(G_COEFF, 8, 8);
        BF_FIELD(B_COEFF, 0, 8);
    END_BF_DECL() r_CAM_CFG_FILTER;
    BEGIN_BF_DECL(CAM_VSYNC_POLARITY_t, uint32_t);
        BF_FIELD(VSYNC_POLARITY, 0, 1);
    END_BF_DECL() r_CAM_VSYNC_POLARITY;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> CAM_RX_SADDR;
    scc::sc_register<uint32_t> CAM_RX_SIZE;
    scc::sc_register<CAM_RX_CFG_t> CAM_RX_CFG;
    scc::sc_register<CAM_CFG_GLOB_t> CAM_CFG_GLOB;
    scc::sc_register<CAM_CFG_LL_t> CAM_CFG_LL;
    scc::sc_register<CAM_CFG_UR_t> CAM_CFG_UR;
    scc::sc_register<CAM_CFG_SIZE_t> CAM_CFG_SIZE;
    scc::sc_register<CAM_CFG_FILTER_t> CAM_CFG_FILTER;
    scc::sc_register<CAM_VSYNC_POLARITY_t> CAM_VSYNC_POLARITY;
    
    cam_channel_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::cam_channel_regs::cam_channel_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(CAM_RX_SADDR, r_CAM_RX_SADDR, 0, *this)
, NAMED(CAM_RX_SIZE, r_CAM_RX_SIZE, 0, *this)
, NAMED(CAM_RX_CFG, r_CAM_RX_CFG, 0, *this)
, NAMED(CAM_CFG_GLOB, r_CAM_CFG_GLOB, 0, *this)
, NAMED(CAM_CFG_LL, r_CAM_CFG_LL, 0, *this)
, NAMED(CAM_CFG_UR, r_CAM_CFG_UR, 0, *this)
, NAMED(CAM_CFG_SIZE, r_CAM_CFG_SIZE, 0, *this)
, NAMED(CAM_CFG_FILTER, r_CAM_CFG_FILTER, 0, *this)
, NAMED(CAM_VSYNC_POLARITY, r_CAM_VSYNC_POLARITY, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::cam_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(CAM_RX_SADDR, 0x0UL);
    target.addResource(CAM_RX_SIZE, 0x4UL);
    target.addResource(CAM_RX_CFG, 0x8UL);
    target.addResource(CAM_CFG_GLOB, 0x20UL);
    target.addResource(CAM_CFG_LL, 0x24UL);
    target.addResource(CAM_CFG_UR, 0xa8UL);
    target.addResource(CAM_CFG_SIZE, 0x2cUL);
    target.addResource(CAM_CFG_FILTER, 0x30UL);
    target.addResource(CAM_VSYNC_POLARITY, 0x34UL);
}

#endif // _PULPISSIMO_GEN_CAM_CHANNEL_H_
