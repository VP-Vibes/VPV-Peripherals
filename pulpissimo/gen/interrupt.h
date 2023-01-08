/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:33 CET 2023
 *             *      interrupt.h Author: <RDL Generator>
 *
 */

#pragma once

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace vpvper {
namespace pulpissimo {
namespace gen {

class interrupt_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_MASK;
    uint32_t r_MASK_SET;
    uint32_t r_MASK_CLR;
    uint32_t r_INT;
    uint32_t r_INT_SET;
    uint32_t r_INT_CLR;
    uint32_t r_ACK;
    uint32_t r_ACK_SET;
    uint32_t r_ACK_CLR;
    uint32_t r_FIFO_DATA;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> MASK;
    scc::sc_register<uint32_t> MASK_SET;
    scc::sc_register<uint32_t> MASK_CLR;
    scc::sc_register<uint32_t> INT;
    scc::sc_register<uint32_t> INT_SET;
    scc::sc_register<uint32_t> INT_CLR;
    scc::sc_register<uint32_t> ACK;
    scc::sc_register<uint32_t> ACK_SET;
    scc::sc_register<uint32_t> ACK_CLR;
    scc::sc_register<uint32_t> FIFO_DATA;
    
    interrupt_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
} // namespace vpvper
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::interrupt_regs::interrupt_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(MASK, r_MASK, 0, *this)
, NAMED(MASK_SET, r_MASK_SET, 0, *this)
, NAMED(MASK_CLR, r_MASK_CLR, 0, *this)
, NAMED(INT, r_INT, 0, *this)
, NAMED(INT_SET, r_INT_SET, 0, *this)
, NAMED(INT_CLR, r_INT_CLR, 0, *this)
, NAMED(ACK, r_ACK, 0, *this)
, NAMED(ACK_SET, r_ACK_SET, 0, *this)
, NAMED(ACK_CLR, r_ACK_CLR, 0, *this)
, NAMED(FIFO_DATA, r_FIFO_DATA, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::interrupt_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(MASK, 0x0UL+offset);
    target.addResource(MASK_SET, 0x4UL+offset);
    target.addResource(MASK_CLR, 0x8UL+offset);
    target.addResource(INT, 0xcUL+offset);
    target.addResource(INT_SET, 0x10UL+offset);
    target.addResource(INT_CLR, 0x14UL+offset);
    target.addResource(ACK, 0x18UL+offset);
    target.addResource(ACK_SET, 0x1cUL+offset);
    target.addResource(ACK_CLR, 0x20UL+offset);
    target.addResource(FIFO_DATA, 0x24UL+offset);
}

