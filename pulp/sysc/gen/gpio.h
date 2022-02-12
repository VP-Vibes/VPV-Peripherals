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
// Created on: Sun Feb 13 08:28:03 CET 2022
//             *      gpio.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYSC_GEN_GPIO_H_
#define _SYSC_GEN_GPIO_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>

namespace sysc {
namespace gen {

class gpio_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(PADDIR_00_31_t, uint32_t);
    END_BF_DECL() r_PADDIR_00_31;
    BEGIN_BF_DECL(GPIOEN_00_31_t, uint32_t);
    END_BF_DECL() r_GPIOEN_00_31;
    BEGIN_BF_DECL(PADIN_00_31_t, uint32_t);
    END_BF_DECL() r_PADIN_00_31;
    BEGIN_BF_DECL(PADOUT_00_31_t, uint32_t);
    END_BF_DECL() r_PADOUT_00_31;
    BEGIN_BF_DECL(PADOUTSET_00_31_t, uint32_t);
    END_BF_DECL() r_PADOUTSET_00_31;
    BEGIN_BF_DECL(PADOUTCLR_00_31_t, uint32_t);
    END_BF_DECL() r_PADOUTCLR_00_31;
    BEGIN_BF_DECL(INTEN_00_31_t, uint32_t);
    END_BF_DECL() r_INTEN_00_31;
    BEGIN_BF_DECL(INTTYPE_00_15_t, uint32_t);
    END_BF_DECL() r_INTTYPE_00_15;
    BEGIN_BF_DECL(INTTYPE_16_31_t, uint32_t);
    END_BF_DECL() r_INTTYPE_16_31;
    BEGIN_BF_DECL(INTSTATUS_00_31_t, uint32_t);
    END_BF_DECL() r_INTSTATUS_00_31;
    BEGIN_BF_DECL(PADCFG_00_07_t, uint32_t);
    END_BF_DECL() r_PADCFG_00_07;
    BEGIN_BF_DECL(PADCFG_08_15_t, uint32_t);
    END_BF_DECL() r_PADCFG_08_15;
    BEGIN_BF_DECL(PADCFG_16_23_t, uint32_t);
    END_BF_DECL() r_PADCFG_16_23;
    BEGIN_BF_DECL(PADCFG_24_31_t, uint32_t);
    END_BF_DECL() r_PADCFG_24_31;
    BEGIN_BF_DECL(PADDIR_32_63_t, uint32_t);
    END_BF_DECL() r_PADDIR_32_63;
    BEGIN_BF_DECL(GPIOEN_32_63_t, uint32_t);
    END_BF_DECL() r_GPIOEN_32_63;
    BEGIN_BF_DECL(PADIN_32_63_t, uint32_t);
    END_BF_DECL() r_PADIN_32_63;
    BEGIN_BF_DECL(PADOUT_32_63_t, uint32_t);
    END_BF_DECL() r_PADOUT_32_63;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<PADDIR_00_31_t> PADDIR_00_31;
    scc::sc_register<GPIOEN_00_31_t> GPIOEN_00_31;
    scc::sc_register<PADIN_00_31_t> PADIN_00_31;
    scc::sc_register<PADOUT_00_31_t> PADOUT_00_31;
    scc::sc_register<PADOUTSET_00_31_t> PADOUTSET_00_31;
    scc::sc_register<PADOUTCLR_00_31_t> PADOUTCLR_00_31;
    scc::sc_register<INTEN_00_31_t> INTEN_00_31;
    scc::sc_register<INTTYPE_00_15_t> INTTYPE_00_15;
    scc::sc_register<INTTYPE_16_31_t> INTTYPE_16_31;
    scc::sc_register<INTSTATUS_00_31_t> INTSTATUS_00_31;
    scc::sc_register<PADCFG_00_07_t> PADCFG_00_07;
    scc::sc_register<PADCFG_08_15_t> PADCFG_08_15;
    scc::sc_register<PADCFG_16_23_t> PADCFG_16_23;
    scc::sc_register<PADCFG_24_31_t> PADCFG_24_31;
    scc::sc_register<PADDIR_32_63_t> PADDIR_32_63;
    scc::sc_register<GPIOEN_32_63_t> GPIOEN_32_63;
    scc::sc_register<PADIN_32_63_t> PADIN_32_63;
    scc::sc_register<PADOUT_32_63_t> PADOUT_32_63;
    
    gpio_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace sysc
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline sysc::gen::gpio_regs::gpio_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(PADDIR_00_31, r_PADDIR_00_31, 0, *this)
, NAMED(GPIOEN_00_31, r_GPIOEN_00_31, 0, *this)
, NAMED(PADIN_00_31, r_PADIN_00_31, 0, *this)
, NAMED(PADOUT_00_31, r_PADOUT_00_31, 0, *this)
, NAMED(PADOUTSET_00_31, r_PADOUTSET_00_31, 0, *this)
, NAMED(PADOUTCLR_00_31, r_PADOUTCLR_00_31, 0, *this)
, NAMED(INTEN_00_31, r_INTEN_00_31, 0, *this)
, NAMED(INTTYPE_00_15, r_INTTYPE_00_15, 0, *this)
, NAMED(INTTYPE_16_31, r_INTTYPE_16_31, 0, *this)
, NAMED(INTSTATUS_00_31, r_INTSTATUS_00_31, 0, *this)
, NAMED(PADCFG_00_07, r_PADCFG_00_07, 0, *this)
, NAMED(PADCFG_08_15, r_PADCFG_08_15, 0, *this)
, NAMED(PADCFG_16_23, r_PADCFG_16_23, 0, *this)
, NAMED(PADCFG_24_31, r_PADCFG_24_31, 0, *this)
, NAMED(PADDIR_32_63, r_PADDIR_32_63, 0, *this)
, NAMED(GPIOEN_32_63, r_GPIOEN_32_63, 0, *this)
, NAMED(PADIN_32_63, r_PADIN_32_63, 0, *this)
, NAMED(PADOUT_32_63, r_PADOUT_32_63, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void sysc::gen::gpio_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(PADDIR_00_31, 0x0UL);
    target.addResource(GPIOEN_00_31, 0x4UL);
    target.addResource(PADIN_00_31, 0x8UL);
    target.addResource(PADOUT_00_31, 0xcUL);
    target.addResource(PADOUTSET_00_31, 0x10UL);
    target.addResource(PADOUTCLR_00_31, 0x14UL);
    target.addResource(INTEN_00_31, 0x18UL);
    target.addResource(INTTYPE_00_15, 0x1cUL);
    target.addResource(INTTYPE_16_31, 0x20UL);
    target.addResource(INTSTATUS_00_31, 0x24UL);
    target.addResource(PADCFG_00_07, 0x28UL);
    target.addResource(PADCFG_08_15, 0x2cUL);
    target.addResource(PADCFG_16_23, 0x30UL);
    target.addResource(PADCFG_24_31, 0x34UL);
    target.addResource(PADDIR_32_63, 0x38UL);
    target.addResource(GPIOEN_32_63, 0x3cUL);
    target.addResource(PADIN_32_63, 0x40UL);
    target.addResource(PADOUT_32_63, 0x44UL);
}

#endif // _SYSC_GEN_GPIO_H_
