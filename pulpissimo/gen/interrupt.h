/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Tue Feb 15 10:40:56 CET 2022
 *             *      interrupt.h Author: <RDL Generator>
 *
 */

#ifndef _PULPISSIMO_GEN_INTERRUPT_H_
#define _PULPISSIMO_GEN_INTERRUPT_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace pulpissimo {
namespace gen {

class interrupt_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_MASK;
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
    scc::sc_register<uint32_t> INT;
    scc::sc_register<uint32_t> INT_SET;
    scc::sc_register<uint32_t> INT_CLR;
    scc::sc_register<uint32_t> ACK;
    scc::sc_register<uint32_t> ACK_SET;
    scc::sc_register<uint32_t> ACK_CLR;
    scc::sc_register<uint32_t> FIFO_DATA;

    interrupt_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset = 0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::interrupt_regs::interrupt_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(MASK, r_MASK, 0, *this)
, NAMED(INT, r_INT, 0, *this)
, NAMED(INT_SET, r_INT_SET, 0, *this)
, NAMED(INT_CLR, r_INT_CLR, 0, *this)
, NAMED(ACK, r_ACK, 0, *this)
, NAMED(ACK_SET, r_ACK_SET, 0, *this)
, NAMED(ACK_CLR, r_ACK_CLR, 0, *this)
, NAMED(FIFO_DATA, r_FIFO_DATA, 0, *this) {}

template <unsigned BUSWIDTH>
inline void pulpissimo::gen::interrupt_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(MASK, 0x0UL);
    target.addResource(INT, 0xcUL);
    target.addResource(INT_SET, 0x10UL);
    target.addResource(INT_CLR, 0x14UL);
    target.addResource(ACK, 0x18UL);
    target.addResource(ACK_SET, 0x1cUL);
    target.addResource(ACK_CLR, 0x20UL);
    target.addResource(FIFO_DATA, 0x24UL);
}

#endif // _PULPISSIMO_GEN_INTERRUPT_H_
