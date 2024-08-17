/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      filter.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_FILTER_H_
#define _PULPISSIMO_GEN_FILTER_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace pulpissimo {
namespace gen {

class filter_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_REG_TX_CH0_ADD;
    BEGIN_BF_DECL(REG_TX_CH0_CFG_t, uint32_t);
    BF_FIELD(MODE, 8, 2);
    BF_FIELD(SIZE, 0, 2);
    END_BF_DECL() r_REG_TX_CH0_CFG;
    uint32_t r_REG_TX_CH0_LEN0;
    uint32_t r_REG_TX_CH0_LEN1;
    uint32_t r_REG_TX_CH0_LEN2;
    uint32_t r_REG_TX_CH1_ADD;
    BEGIN_BF_DECL(REG_TX_CH1_CFG_t, uint32_t);
    BF_FIELD(MODE, 8, 2);
    BF_FIELD(SIZE, 0, 2);
    END_BF_DECL() r_REG_TX_CH1_CFG;
    uint32_t r_REG_TX_CH1_LEN0;
    uint32_t r_REG_TX_CH1_LEN1;
    uint32_t r_REG_TX_CH1_LEN2;
    uint32_t r_REG_RX_CH_ADD;
    BEGIN_BF_DECL(REG_RX_CH_CFG_t, uint32_t);
    BF_FIELD(MODE, 8, 2);
    BF_FIELD(SIZE, 0, 2);
    END_BF_DECL() r_REG_RX_CH_CFG;
    uint32_t r_REG_RX_CH_LEN0;
    uint32_t r_REG_RX_CH_LEN1;
    uint32_t r_REG_RX_CH_LEN2;
    BEGIN_BF_DECL(REG_AU_CFG_t, uint32_t);
    BF_FIELD(SHIFT, 16, 5);
    BF_FIELD(MODE, 8, 4);
    BF_FIELD(BYPASS, 1, 1);
    BF_FIELD(SIGNED, 0, 1);
    END_BF_DECL() r_REG_AU_CFG;
    uint32_t r_REG_AU_REG0;
    uint32_t r_REG_AU_REG1;
    uint32_t r_REG_BINCU_TH;
    BEGIN_BF_DECL(REG_BINCU_CNT_t, uint32_t);
    BF_FIELD(EN, 31, 1);
    BF_FIELD(COUNT, 0, 20);
    END_BF_DECL() r_REG_BINCU_CNT;
    uint32_t r_REG_BINCU_SETUP;
    uint32_t r_REG_BINCU_VAL;
    uint32_t r_REG_FILT;
    BEGIN_BF_DECL(REG_FILT_CMD_t, uint32_t);
    BF_FIELD(START, 0, 1);
    END_BF_DECL() r_REG_FILT_CMD;
    BEGIN_BF_DECL(REG_STATUS_t, uint32_t);
    BF_FIELD(DONE, 0, 1);
    END_BF_DECL() r_REG_STATUS;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> REG_TX_CH0_ADD;
    scc::sc_register<REG_TX_CH0_CFG_t> REG_TX_CH0_CFG;
    scc::sc_register<uint32_t> REG_TX_CH0_LEN0;
    scc::sc_register<uint32_t> REG_TX_CH0_LEN1;
    scc::sc_register<uint32_t> REG_TX_CH0_LEN2;
    scc::sc_register<uint32_t> REG_TX_CH1_ADD;
    scc::sc_register<REG_TX_CH1_CFG_t> REG_TX_CH1_CFG;
    scc::sc_register<uint32_t> REG_TX_CH1_LEN0;
    scc::sc_register<uint32_t> REG_TX_CH1_LEN1;
    scc::sc_register<uint32_t> REG_TX_CH1_LEN2;
    scc::sc_register<uint32_t> REG_RX_CH_ADD;
    scc::sc_register<REG_RX_CH_CFG_t> REG_RX_CH_CFG;
    scc::sc_register<uint32_t> REG_RX_CH_LEN0;
    scc::sc_register<uint32_t> REG_RX_CH_LEN1;
    scc::sc_register<uint32_t> REG_RX_CH_LEN2;
    scc::sc_register<REG_AU_CFG_t> REG_AU_CFG;
    scc::sc_register<uint32_t> REG_AU_REG0;
    scc::sc_register<uint32_t> REG_AU_REG1;
    scc::sc_register<uint32_t> REG_BINCU_TH;
    scc::sc_register<REG_BINCU_CNT_t> REG_BINCU_CNT;
    scc::sc_register<uint32_t> REG_BINCU_SETUP;
    scc::sc_register<uint32_t> REG_BINCU_VAL;
    scc::sc_register<uint32_t> REG_FILT;
    scc::sc_register<REG_FILT_CMD_t> REG_FILT_CMD;
    scc::sc_register<REG_STATUS_t> REG_STATUS;

    filter_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset = 0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::filter_regs::filter_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(REG_TX_CH0_ADD, r_REG_TX_CH0_ADD, 0, *this)
, NAMED(REG_TX_CH0_CFG, r_REG_TX_CH0_CFG, 0, *this)
, NAMED(REG_TX_CH0_LEN0, r_REG_TX_CH0_LEN0, 0, *this)
, NAMED(REG_TX_CH0_LEN1, r_REG_TX_CH0_LEN1, 0, *this)
, NAMED(REG_TX_CH0_LEN2, r_REG_TX_CH0_LEN2, 0, *this)
, NAMED(REG_TX_CH1_ADD, r_REG_TX_CH1_ADD, 0, *this)
, NAMED(REG_TX_CH1_CFG, r_REG_TX_CH1_CFG, 0, *this)
, NAMED(REG_TX_CH1_LEN0, r_REG_TX_CH1_LEN0, 0, *this)
, NAMED(REG_TX_CH1_LEN1, r_REG_TX_CH1_LEN1, 0, *this)
, NAMED(REG_TX_CH1_LEN2, r_REG_TX_CH1_LEN2, 0, *this)
, NAMED(REG_RX_CH_ADD, r_REG_RX_CH_ADD, 0, *this)
, NAMED(REG_RX_CH_CFG, r_REG_RX_CH_CFG, 0, *this)
, NAMED(REG_RX_CH_LEN0, r_REG_RX_CH_LEN0, 0, *this)
, NAMED(REG_RX_CH_LEN1, r_REG_RX_CH_LEN1, 0, *this)
, NAMED(REG_RX_CH_LEN2, r_REG_RX_CH_LEN2, 0, *this)
, NAMED(REG_AU_CFG, r_REG_AU_CFG, 0, *this)
, NAMED(REG_AU_REG0, r_REG_AU_REG0, 0, *this)
, NAMED(REG_AU_REG1, r_REG_AU_REG1, 0, *this)
, NAMED(REG_BINCU_TH, r_REG_BINCU_TH, 0, *this)
, NAMED(REG_BINCU_CNT, r_REG_BINCU_CNT, 0, *this)
, NAMED(REG_BINCU_SETUP, r_REG_BINCU_SETUP, 0, *this)
, NAMED(REG_BINCU_VAL, r_REG_BINCU_VAL, 0, *this)
, NAMED(REG_FILT, r_REG_FILT, 0, *this)
, NAMED(REG_FILT_CMD, r_REG_FILT_CMD, 0, *this)
, NAMED(REG_STATUS, r_REG_STATUS, 0, *this) {}

template <unsigned BUSWIDTH>
inline void pulpissimo::gen::filter_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(REG_TX_CH0_ADD, 0x0UL);
    target.addResource(REG_TX_CH0_CFG, 0x4UL);
    target.addResource(REG_TX_CH0_LEN0, 0x8UL);
    target.addResource(REG_TX_CH0_LEN1, 0xcUL);
    target.addResource(REG_TX_CH0_LEN2, 0x10UL);
    target.addResource(REG_TX_CH1_ADD, 0x14UL);
    target.addResource(REG_TX_CH1_CFG, 0x18UL);
    target.addResource(REG_TX_CH1_LEN0, 0x1cUL);
    target.addResource(REG_TX_CH1_LEN1, 0x20UL);
    target.addResource(REG_TX_CH1_LEN2, 0x24UL);
    target.addResource(REG_RX_CH_ADD, 0x28UL);
    target.addResource(REG_RX_CH_CFG, 0x2cUL);
    target.addResource(REG_RX_CH_LEN0, 0x30UL);
    target.addResource(REG_RX_CH_LEN1, 0x34UL);
    target.addResource(REG_RX_CH_LEN2, 0x38UL);
    target.addResource(REG_AU_CFG, 0x3cUL);
    target.addResource(REG_AU_REG0, 0x40UL);
    target.addResource(REG_AU_REG1, 0x44UL);
    target.addResource(REG_BINCU_TH, 0x48UL);
    target.addResource(REG_BINCU_CNT, 0x4cUL);
    target.addResource(REG_BINCU_SETUP, 0x50UL);
    target.addResource(REG_BINCU_VAL, 0x54UL);
    target.addResource(REG_FILT, 0x58UL);
    target.addResource(REG_FILT_CMD, 0x5cUL);
    target.addResource(REG_STATUS, 0x60UL);
}

#endif // _PULPISSIMO_GEN_FILTER_H_
