/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      i2s_channel.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_I2S_CHANNEL_H_
#define _PULPISSIMO_GEN_I2S_CHANNEL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace vpvper::pulpissimo {
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
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::i2s_channel_regs::i2s_channel_regs(sc_core::sc_module_name nm)
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
inline void vpvper::pulpissimo::gen::i2s_channel_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(I2S_RX_SADDR, 0x0UL + offset);
    target.addResource(I2S_RX_SIZE, 0x4UL + offset);
    target.addResource(I2S_RX_CFG, 0x8UL + offset);
    target.addResource(I2S_TX_SADDR, 0x10UL + offset);
    target.addResource(I2S_TX_SIZE, 0x14UL + offset);
    target.addResource(I2S_TX_CFG, 0x18UL + offset);
    target.addResource(I2S_CLKCFG_SETUP, 0x20UL + offset);
    target.addResource(I2S_SLV_SETUP, 0x24UL + offset);
    target.addResource(I2S_MST_SETUP, 0x28UL + offset);
    target.addResource(I2S_PDM_SETUP, 0x2cUL + offset);
}

#endif // _PULPISSIMO_GEN_I2S_CHANNEL_H_
