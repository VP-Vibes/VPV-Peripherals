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
//             *      fll.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_FLL_H_
#define _PULPISSIMO_GEN_FLL_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {

class fll_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(STATUS_t, uint32_t);
        BF_FIELD(MF, 0, 16);
    END_BF_DECL() r_STATUS;
    BEGIN_BF_DECL(CFG1_t, uint32_t);
        BF_FIELD(CKM, 31, 1);
        BF_FIELD(CKG, 30, 1);
        BF_FIELD(CKDIV, 26, 4);
        BF_FIELD(ICS, 16, 10);
        BF_FIELD(MFN, 0, 16);
    END_BF_DECL() r_CFG1;
    BEGIN_BF_DECL(CFG2_t, uint32_t);
        BF_FIELD(DITH, 31, 1);
        BF_FIELD(CKM, 30, 1);
        BF_FIELD(CKSEL, 29, 1);
        BF_FIELD(LT, 16, 12);
        BF_FIELD(SCKL, 10, 6);
        BF_FIELD(UCKL, 4, 6);
        BF_FIELD(LG, 0, 4);
    END_BF_DECL() r_CFG2;
    BEGIN_BF_DECL(INTEG_t, uint32_t);
        BF_FIELD(INTEG, 16, 10);
        BF_FIELD(FRAC, 6, 10);
    END_BF_DECL() r_INTEG;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<STATUS_t> STATUS;
    scc::sc_register<CFG1_t> CFG1;
    scc::sc_register<CFG2_t> CFG2;
    scc::sc_register<INTEG_t> INTEG;
    
    fll_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::fll_regs::fll_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(STATUS, r_STATUS, 0, *this)
, NAMED(CFG1, r_CFG1, 0, *this)
, NAMED(CFG2, r_CFG2, 0, *this)
, NAMED(INTEG, r_INTEG, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::fll_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(STATUS, 0x0UL);
    target.addResource(CFG1, 0x4UL);
    target.addResource(CFG2, 0x8UL);
    target.addResource(INTEG, 0xcUL);
}

#endif // _PULPISSIMO_GEN_FLL_H_
