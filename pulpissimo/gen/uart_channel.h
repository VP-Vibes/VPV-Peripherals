/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      uart_channel.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_UART_CHANNEL_H_
#define _PULPISSIMO_GEN_UART_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace vpvper::pulpissimo {
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

inline vpvper::pulpissimo::gen::uart_channel_regs::uart_channel_regs(sc_core::sc_module_name nm)
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
inline void vpvper::pulpissimo::gen::uart_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(RX_SADDR, 0x0UL + offset);
    target.addResource(RX_SIZE, 0x4UL + offset);
    target.addResource(RX_CFG, 0x8UL + offset);
    target.addResource(TX_SADDR, 0x10UL + offset);
    target.addResource(TX_SIZE, 0x14UL + offset);
    target.addResource(TX_CFG, 0x18UL + offset);
    target.addResource(STATUS, 0x20UL + offset);
    target.addResource(SETUP, 0x24UL + offset);
    target.addResource(ERROR, 0x28UL + offset);
    target.addResource(IRQ_EN, 0x2cUL + offset);
    target.addResource(VALID, 0x30UL + offset);
    target.addResource(DATA, 0x34UL + offset);
}

#endif // _PULPISSIMO_GEN_UART_CHANNEL_H_
