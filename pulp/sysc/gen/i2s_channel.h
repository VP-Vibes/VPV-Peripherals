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
//             *      i2s_channel.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSC_GEN_I2S_CHANNEL_H_
#define _SYSC_GEN_I2S_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace sysc {
namespace gen {

class i2s_channel_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_I2S_RX_SADDR;
    uint32_t r_I2S_RX_SIZE;
    BEGIN_BF_DECL(I2S_RX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_I2S_RX_CFG;
    uint32_t r_I2S_TX_SADDR;
    uint32_t r_I2S_TX_SIZE;
    BEGIN_BF_DECL(I2S_TX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_I2S_TX_CFG;
    BEGIN_BF_DECL(I2S_CLKCFG_SETUP_t, uint32_t);
        BF_FIELD(MASTER_NUM, 31, 1);
        BF_FIELD(MASTER_EXT, 30, 1);
        BF_FIELD(SLAVE_NUM, 29, 1);
        BF_FIELD(SLAVE_EXT, 28, 1);
        BF_FIELD(PDM_CLK_EN, 26, 1);
        BF_FIELD(MASTER_CLK_EN, 25, 1);
        BF_FIELD(SLAVE_CLK_EN, 24, 1);
        BF_FIELD(COMMON_CLK_DIV, 16, 8);
        BF_FIELD(SLAVE_CLK_DIV, 8, 8);
        BF_FIELD(MASTER_CLK_DIV, 0, 8);
    END_BF_DECL() r_I2S_CLKCFG_SETUP;
    BEGIN_BF_DECL(I2S_SLV_SETUP_t, uint32_t);
        BF_FIELD(SLAVE_EN, 31, 1);
        BF_FIELD(SLAVE_2CH, 17, 1);
        BF_FIELD(SLAVE_LSB, 16, 1);
        BF_FIELD(SLAVE_BITS, 8, 5);
        BF_FIELD(SLAVE_WORDS, 0, 3);
    END_BF_DECL() r_I2S_SLV_SETUP;
    BEGIN_BF_DECL(I2S_MST_SETUP_t, uint32_t);
        BF_FIELD(MASTER_EN, 31, 1);
        BF_FIELD(MASTER_2CH, 17, 1);
        BF_FIELD(MASTER_LSB, 16, 1);
        BF_FIELD(MASTER_BITS, 8, 5);
        BF_FIELD(MASTER_WORDS, 0, 3);
    END_BF_DECL() r_I2S_MST_SETUP;
    BEGIN_BF_DECL(I2S_PDM_SETUP_t, uint32_t);
        BF_FIELD(PDM_DECIMATION, 3, 10);
        BF_FIELD(PDM_SHIFT, 0, 3);
    END_BF_DECL() r_I2S_PDM_SETUP;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> I2S_RX_SADDR;
    scc::sc_register<uint32_t> I2S_RX_SIZE;
    scc::sc_register<I2S_RX_CFG_t> I2S_RX_CFG;
    scc::sc_register<uint32_t> I2S_TX_SADDR;
    scc::sc_register<uint32_t> I2S_TX_SIZE;
    scc::sc_register<I2S_TX_CFG_t> I2S_TX_CFG;
    scc::sc_register<I2S_CLKCFG_SETUP_t> I2S_CLKCFG_SETUP;
    scc::sc_register<I2S_SLV_SETUP_t> I2S_SLV_SETUP;
    scc::sc_register<I2S_MST_SETUP_t> I2S_MST_SETUP;
    scc::sc_register<I2S_PDM_SETUP_t> I2S_PDM_SETUP;
    
    i2s_channel_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace sysc
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline sysc::gen::i2s_channel_regs::i2s_channel_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(I2S_RX_SADDR, r_I2S_RX_SADDR, 0, *this)
, NAMED(I2S_RX_SIZE, r_I2S_RX_SIZE, 0, *this)
, NAMED(I2S_RX_CFG, r_I2S_RX_CFG, 0, *this)
, NAMED(I2S_TX_SADDR, r_I2S_TX_SADDR, 0, *this)
, NAMED(I2S_TX_SIZE, r_I2S_TX_SIZE, 0, *this)
, NAMED(I2S_TX_CFG, r_I2S_TX_CFG, 0, *this)
, NAMED(I2S_CLKCFG_SETUP, r_I2S_CLKCFG_SETUP, 0, *this)
, NAMED(I2S_SLV_SETUP, r_I2S_SLV_SETUP, 0, *this)
, NAMED(I2S_MST_SETUP, r_I2S_MST_SETUP, 0, *this)
, NAMED(I2S_PDM_SETUP, r_I2S_PDM_SETUP, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void sysc::gen::i2s_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(I2S_RX_SADDR, 0x0UL);
    target.addResource(I2S_RX_SIZE, 0x4UL);
    target.addResource(I2S_RX_CFG, 0x8UL);
    target.addResource(I2S_TX_SADDR, 0x10UL);
    target.addResource(I2S_TX_SIZE, 0x14UL);
    target.addResource(I2S_TX_CFG, 0x18UL);
    target.addResource(I2S_CLKCFG_SETUP, 0x20UL);
    target.addResource(I2S_SLV_SETUP, 0x24UL);
    target.addResource(I2S_MST_SETUP, 0x28UL);
    target.addResource(I2S_PDM_SETUP, 0x2cUL);
}

#endif // _SYSC_GEN_I2S_CHANNEL_H_
