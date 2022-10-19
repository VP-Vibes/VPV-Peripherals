/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      spi_channel.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_SPI_CHANNEL_H_
#define _PULPISSIMO_GEN_SPI_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace vpvper::pulpissimo {
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

inline vpvper::pulpissimo::gen::spi_channel_regs::spi_channel_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(SPIM_RX_SADDR, r_SPIM_RX_SADDR, 0, *this)
, NAMED(SPIM_RX_SIZE, r_SPIM_RX_SIZE, 0, *this)
, NAMED(SPIM_RX_CFG, r_SPIM_RX_CFG, 0x4, *this, 0xffffffbf, 0xffffffdf)
, NAMED(SPIM_TX_SADDR, r_SPIM_TX_SADDR, 0, *this)
, NAMED(SPIM_TX_SIZE, r_SPIM_TX_SIZE, 0, *this)
, NAMED(SPIM_TX_CFG, r_SPIM_TX_CFG, 0, *this)
, NAMED(SPIM_CMD_SADDR, r_SPIM_CMD_SADDR, 0, *this)
, NAMED(SPIM_CMD_SIZE, r_SPIM_CMD_SIZE, 0, *this)
, NAMED(SPIM_CMD_CFG, r_SPIM_CMD_CFG, 0, *this, 0xffffffbf, 0xffffffdf)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::spi_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(SPIM_RX_SADDR, 0x0UL + offset);
    target.addResource(SPIM_RX_SIZE, 0x4UL + offset);
    target.addResource(SPIM_RX_CFG, 0x8UL + offset);
    target.addResource(SPIM_TX_SADDR, 0x10UL + offset);
    target.addResource(SPIM_TX_SIZE, 0x14UL + offset);
    target.addResource(SPIM_TX_CFG, 0x18UL + offset);
    target.addResource(SPIM_CMD_SADDR, 0x20UL + offset);
    target.addResource(SPIM_CMD_SIZE, 0x24UL + offset);
    target.addResource(SPIM_CMD_CFG, 0x28UL + offset);
}

#endif // _PULPISSIMO_GEN_SPI_CHANNEL_H_
