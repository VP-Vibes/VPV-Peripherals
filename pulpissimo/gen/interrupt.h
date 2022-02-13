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
// Created on: Sun Feb 13 11:14:08 CET 2022
//             *      interrupt.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_INTERRUPT_H_
#define _PULPISSIMO_GEN_INTERRUPT_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

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
    uint32_t r_INT;
    uint32_t r_ACK;
    uint32_t r_FIFO_DATA;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> MASK;
    scc::sc_register<uint32_t> INT;
    scc::sc_register<uint32_t> ACK;
    scc::sc_register<uint32_t> FIFO_DATA;
    
    interrupt_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
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
, NAMED(ACK, r_ACK, 0, *this)
, NAMED(FIFO_DATA, r_FIFO_DATA, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::interrupt_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(MASK, 0x0UL);
    target.addResource(INT, 0xcUL);
    target.addResource(ACK, 0x18UL);
    target.addResource(FIFO_DATA, 0x24UL);
}

#endif // _PULPISSIMO_GEN_INTERRUPT_H_
