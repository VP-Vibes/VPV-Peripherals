/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SPI_REGS_H_
#define _SPI_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class qspi_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(DATA_t, uint32_t);
        BF_FIELD(data, 0, 8);
        BF_FIELD(write, 8, 1);
        BF_FIELD(read, 9, 1);
        BF_FIELD(data_ctrl, 11, 1);
        BF_FIELD(rxvalid, 31, 1);
    END_BF_DECL() r_DATA;
    BEGIN_BF_DECL(STATUS_t, uint32_t);
        BF_FIELD(txfree, 0, 1);
        BF_FIELD(rxavail, 16, 1);
    END_BF_DECL() r_STATUS;
    BEGIN_BF_DECL(CONFIG_t, uint32_t);
        BF_FIELD(cpol, 0, 1);
        BF_FIELD(cpha, 1, 1);
        BF_FIELD(rxavail, 16, 1);
        BF_FIELD(ifmode, 4, 2);
    END_BF_DECL() r_CONFIG;
    BEGIN_BF_DECL(INTR_t, uint32_t);
        BF_FIELD(txien, 0, 1);
        BF_FIELD(rxien, 1, 1);
        BF_FIELD(txip, 8, 1);
        BF_FIELD(rxip, 9, 1);
    END_BF_DECL() r_INTR;
    BEGIN_BF_DECL(CLOCK_DIV_t, uint32_t);
        BF_FIELD(toggle, 0, 12);
    END_BF_DECL() r_CLOCK_DIV;
    BEGIN_BF_DECL(SS_SETUP_t, uint32_t);
        BF_FIELD(toggle, 0, 12);
    END_BF_DECL() r_SS_SETUP;
    BEGIN_BF_DECL(SS_HOLD_t, uint32_t);
        BF_FIELD(toggle, 0, 12);
    END_BF_DECL() r_SS_HOLD;
    BEGIN_BF_DECL(SS_DISABLE_t, uint32_t);
        BF_FIELD(toggle, 0, 12);
    END_BF_DECL() r_SS_DISABLE;
    BEGIN_BF_DECL(SS_ACTIVEHIGH_t, uint32_t);
        BF_FIELD(toggle, 0, 12);
    END_BF_DECL() r_SS_ACTIVEHIGH;
    BEGIN_BF_DECL(XIP_ENABLE_t, uint32_t);
        BF_FIELD(ena, 0, 1);
    END_BF_DECL() r_XIP_ENABLE;
    BEGIN_BF_DECL(XIP_INST_t, uint32_t);
        BF_FIELD(data, 0, 1);
        BF_FIELD(ena, 1, 1);
        BF_FIELD(dummy_data, 16, 8);
        BF_FIELD(dummy_count, 24, 4);
    END_BF_DECL() r_XIP_INST;
    BEGIN_BF_DECL(XIP_MODES_t, uint32_t);
        BF_FIELD(instr, 0, 2);
        BF_FIELD(addr, 8, 2);
        BF_FIELD(dummy, 16, 2);
        BF_FIELD(data, 24, 2);
    END_BF_DECL() r_XIP_MODES;
    uint32_t r_XIP_WRITE32;
    uint32_t r_XIP_READWRITE32;
    uint32_t r_XIP_READ32;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<DATA_t> DATA;
    scc::sc_register<STATUS_t> STATUS;
    scc::sc_register<CONFIG_t> CONFIG;
    scc::sc_register<INTR_t> INTR;
    scc::sc_register<CLOCK_DIV_t> CLOCK_DIV;
    scc::sc_register<SS_SETUP_t> SS_SETUP;
    scc::sc_register<SS_HOLD_t> SS_HOLD;
    scc::sc_register<SS_DISABLE_t> SS_DISABLE;
    scc::sc_register<SS_ACTIVEHIGH_t> SS_ACTIVEHIGH;
    scc::sc_register<XIP_ENABLE_t> XIP_ENABLE;
    scc::sc_register<XIP_INST_t> XIP_INST;
    scc::sc_register<XIP_MODES_t> XIP_MODES;
    scc::sc_register<uint32_t> XIP_WRITE32;
    scc::sc_register<uint32_t> XIP_READWRITE32;
    scc::sc_register<uint32_t> XIP_READ32;
    
    qspi_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} /* namespace minres */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::minres::qspi_regs::qspi_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(DATA, r_DATA, 0, *this)
, NAMED(STATUS, r_STATUS, 0, *this)
, NAMED(CONFIG, r_CONFIG, 0, *this)
, NAMED(INTR, r_INTR, 0, *this)
, NAMED(CLOCK_DIV, r_CLOCK_DIV, 0, *this)
, NAMED(SS_SETUP, r_SS_SETUP, 0, *this)
, NAMED(SS_HOLD, r_SS_HOLD, 0, *this)
, NAMED(SS_DISABLE, r_SS_DISABLE, 0, *this)
, NAMED(SS_ACTIVEHIGH, r_SS_ACTIVEHIGH, 0, *this)
, NAMED(XIP_ENABLE, r_XIP_ENABLE, 0, *this)
, NAMED(XIP_INST, r_XIP_INST, 0, *this)
, NAMED(XIP_MODES, r_XIP_MODES, 0, *this)
, NAMED(XIP_WRITE32, r_XIP_WRITE32, 0, *this)
, NAMED(XIP_READWRITE32, r_XIP_READWRITE32, 0, *this)
, NAMED(XIP_READ32, r_XIP_READ32, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::minres::qspi_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(DATA, 0x0UL);
    target.addResource(STATUS, 0x4UL);
    target.addResource(CONFIG, 0x8UL);
    target.addResource(INTR, 0xcUL);
    target.addResource(CLOCK_DIV, 0x20UL);
    target.addResource(SS_SETUP, 0x24UL);
    target.addResource(SS_HOLD, 0x28UL);
    target.addResource(SS_DISABLE, 0x2cUL);
    target.addResource(SS_ACTIVEHIGH, 0x30UL);
    target.addResource(XIP_ENABLE, 0x40UL);
    target.addResource(XIP_INST, 0x44UL);
    target.addResource(XIP_MODES, 0x48UL);
    target.addResource(XIP_WRITE32, 0x50UL);
    target.addResource(XIP_READWRITE32, 0x54UL);
    target.addResource(XIP_READ32, 0x58UL);
}

#endif // _SPI_REGS_H_
