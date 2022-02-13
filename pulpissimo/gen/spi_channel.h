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
//             *      spi_channel.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_SPI_CHANNEL_H_
#define _PULPISSIMO_GEN_SPI_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {

class spi_channel_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_SPIM_RX_SADDR;
    uint32_t r_SPIM_RX_SIZE;
    BEGIN_BF_DECL(SPIM_RX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_SPIM_RX_CFG;
    uint32_t r_SPIM_TX_SADDR;
    uint32_t r_SPIM_TX_SIZE;
    BEGIN_BF_DECL(SPIM_TX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_SPIM_TX_CFG;
    uint32_t r_SPIM_CMD_SADDR;
    uint32_t r_SPIM_CMD_SIZE;
    BEGIN_BF_DECL(SPIM_CMD_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(DATASIZE, 1, 2);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_SPIM_CMD_CFG;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> SPIM_RX_SADDR;
    scc::sc_register<uint32_t> SPIM_RX_SIZE;
    scc::sc_register<SPIM_RX_CFG_t> SPIM_RX_CFG;
    scc::sc_register<uint32_t> SPIM_TX_SADDR;
    scc::sc_register<uint32_t> SPIM_TX_SIZE;
    scc::sc_register<SPIM_TX_CFG_t> SPIM_TX_CFG;
    scc::sc_register<uint32_t> SPIM_CMD_SADDR;
    scc::sc_register<uint32_t> SPIM_CMD_SIZE;
    scc::sc_register<SPIM_CMD_CFG_t> SPIM_CMD_CFG;
    
    spi_channel_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::spi_channel_regs::spi_channel_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(SPIM_RX_SADDR, r_SPIM_RX_SADDR, 0, *this)
, NAMED(SPIM_RX_SIZE, r_SPIM_RX_SIZE, 0, *this)
, NAMED(SPIM_RX_CFG, r_SPIM_RX_CFG, 0, *this)
, NAMED(SPIM_TX_SADDR, r_SPIM_TX_SADDR, 0, *this)
, NAMED(SPIM_TX_SIZE, r_SPIM_TX_SIZE, 0, *this)
, NAMED(SPIM_TX_CFG, r_SPIM_TX_CFG, 0, *this)
, NAMED(SPIM_CMD_SADDR, r_SPIM_CMD_SADDR, 0, *this)
, NAMED(SPIM_CMD_SIZE, r_SPIM_CMD_SIZE, 0, *this)
, NAMED(SPIM_CMD_CFG, r_SPIM_CMD_CFG, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::spi_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(SPIM_RX_SADDR, 0x100UL);
    target.addResource(SPIM_RX_SIZE, 0x104UL);
    target.addResource(SPIM_RX_CFG, 0x108UL);
    target.addResource(SPIM_TX_SADDR, 0x110UL);
    target.addResource(SPIM_TX_SIZE, 0x114UL);
    target.addResource(SPIM_TX_CFG, 0x118UL);
    target.addResource(SPIM_CMD_SADDR, 0x120UL);
    target.addResource(SPIM_CMD_SIZE, 0x124UL);
    target.addResource(SPIM_CMD_CFG, 0x128UL);
}

#endif // _PULPISSIMO_GEN_SPI_CHANNEL_H_
