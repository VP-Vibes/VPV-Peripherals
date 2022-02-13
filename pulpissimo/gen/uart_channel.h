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
//             *      uart_channel.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_UART_CHANNEL_H_
#define _PULPISSIMO_GEN_UART_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {

class uart_channel_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(RX_SADDR_t, uint32_t);
        BF_FIELD(RX_SADDR, 0, 16);
    END_BF_DECL() r_RX_SADDR;
    BEGIN_BF_DECL(RX_SIZE_t, uint32_t);
        BF_FIELD(RX_SIZE, 0, 17);
    END_BF_DECL() r_RX_SIZE;
    BEGIN_BF_DECL(RX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_RX_CFG;
    BEGIN_BF_DECL(TX_SADDR_t, uint32_t);
        BF_FIELD(TX_SADDR, 0, 16);
    END_BF_DECL() r_TX_SADDR;
    BEGIN_BF_DECL(TX_SIZE_t, uint32_t);
        BF_FIELD(TX_SIZE, 0, 17);
    END_BF_DECL() r_TX_SIZE;
    BEGIN_BF_DECL(TX_CFG_t, uint32_t);
        BF_FIELD(CLR, 6, 1);
        BF_FIELD(PENDING, 5, 1);
        BF_FIELD(EN, 4, 1);
        BF_FIELD(CONTINOUS, 0, 1);
    END_BF_DECL() r_TX_CFG;
    BEGIN_BF_DECL(STATUS_t, uint32_t);
        BF_FIELD(RX_BUSY, 1, 1);
        BF_FIELD(TX_BUSY, 0, 1);
    END_BF_DECL() r_STATUS;
    BEGIN_BF_DECL(SETUP_t, uint32_t);
        BF_FIELD(CLKDIV, 16, 16);
        BF_FIELD(RX_ENA, 9, 1);
        BF_FIELD(TX_ENA, 8, 1);
        BF_FIELD(CLEAN_FIFO, 5, 1);
        BF_FIELD(POLLING_EN, 4, 1);
        BF_FIELD(STOP_BITS, 3, 1);
        BF_FIELD(BIT_LENGHT, 1, 2);
        BF_FIELD(TX_BUSY, 0, 1);
    END_BF_DECL() r_SETUP;
    BEGIN_BF_DECL(ERROR_t, uint32_t);
        BF_FIELD(RX_ERR_PARITY, 1, 1);
        BF_FIELD(RX_ERR_OVERFLOW, 0, 1);
    END_BF_DECL() r_ERROR;
    BEGIN_BF_DECL(IRQ_EN_t, uint32_t);
        BF_FIELD(ERROR, 1, 1);
        BF_FIELD(RX, 0, 1);
    END_BF_DECL() r_IRQ_EN;
    BEGIN_BF_DECL(VALID_t, uint32_t);
        BF_FIELD(READY, 0, 1);
    END_BF_DECL() r_VALID;
    BEGIN_BF_DECL(DATA_t, uint32_t);
        BF_FIELD(READY, 0, 8);
    END_BF_DECL() r_DATA;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<RX_SADDR_t> RX_SADDR;
    scc::sc_register<RX_SIZE_t> RX_SIZE;
    scc::sc_register<RX_CFG_t> RX_CFG;
    scc::sc_register<TX_SADDR_t> TX_SADDR;
    scc::sc_register<TX_SIZE_t> TX_SIZE;
    scc::sc_register<TX_CFG_t> TX_CFG;
    scc::sc_register<STATUS_t> STATUS;
    scc::sc_register<SETUP_t> SETUP;
    scc::sc_register<ERROR_t> ERROR;
    scc::sc_register<IRQ_EN_t> IRQ_EN;
    scc::sc_register<VALID_t> VALID;
    scc::sc_register<DATA_t> DATA;
    
    uart_channel_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::uart_channel_regs::uart_channel_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(RX_SADDR, r_RX_SADDR, 0, *this)
, NAMED(RX_SIZE, r_RX_SIZE, 0, *this)
, NAMED(RX_CFG, r_RX_CFG, 0, *this)
, NAMED(TX_SADDR, r_TX_SADDR, 0, *this)
, NAMED(TX_SIZE, r_TX_SIZE, 0, *this)
, NAMED(TX_CFG, r_TX_CFG, 0, *this)
, NAMED(STATUS, r_STATUS, 0, *this)
, NAMED(SETUP, r_SETUP, 0, *this)
, NAMED(ERROR, r_ERROR, 0, *this)
, NAMED(IRQ_EN, r_IRQ_EN, 0, *this)
, NAMED(VALID, r_VALID, 0, *this)
, NAMED(DATA, r_DATA, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::uart_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(RX_SADDR, 0x0UL);
    target.addResource(RX_SIZE, 0x4UL);
    target.addResource(RX_CFG, 0x8UL);
    target.addResource(TX_SADDR, 0x10UL);
    target.addResource(TX_SIZE, 0x14UL);
    target.addResource(TX_CFG, 0x18UL);
    target.addResource(STATUS, 0x20UL);
    target.addResource(SETUP, 0x24UL);
    target.addResource(ERROR, 0x28UL);
    target.addResource(IRQ_EN, 0x2cUL);
    target.addResource(VALID, 0x30UL);
    target.addResource(DATA, 0x34UL);
}

#endif // _PULPISSIMO_GEN_UART_CHANNEL_H_
