/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Created on: Sat Jan 07 23:53:33 CET 2023
 *             *      gpio.h Author: <RDL Generator>
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

class gpio_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_PADDIR_00_31;
    uint32_t r_GPIOEN_00_31;
    uint32_t r_PADIN_00_31;
    uint32_t r_PADOUT_00_31;
    uint32_t r_PADOUTSET_00_31;
    uint32_t r_PADOUTCLR_00_31;
    uint32_t r_INTEN_00_31;
    uint32_t r_INTTYPE_00_15;
    uint32_t r_INTTYPE_16_31;
    uint32_t r_INTSTATUS_00_31;
    uint32_t r_PADCFG_00_07;
    uint32_t r_PADCFG_08_15;
    uint32_t r_PADCFG_16_23;
    uint32_t r_PADCFG_24_31;
    uint32_t r_PADDIR_32_63;
    uint32_t r_GPIOEN_32_63;
    uint32_t r_PADIN_32_63;
    uint32_t r_PADOUT_32_63;
    uint32_t r_PADOUTSET_32_63;
    uint32_t r_PADOUTCLR_32_63;
    uint32_t r_INTEN_32_63;
    uint32_t r_INTTYPE_32_47;
    uint32_t r_INTTYPE_48_63;
    uint32_t r_INTSTATUS_32_63;
    uint32_t r_PADCFG_32_39;
    uint32_t r_PADCFG_40_47;
    uint32_t r_PADCFG_48_55;
    uint32_t r_PADCFG_56_63;
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<uint32_t> PADDIR_00_31;
    scc::sc_register<uint32_t> GPIOEN_00_31;
    scc::sc_register<uint32_t> PADIN_00_31;
    scc::sc_register<uint32_t> PADOUT_00_31;
    scc::sc_register<uint32_t> PADOUTSET_00_31;
    scc::sc_register<uint32_t> PADOUTCLR_00_31;
    scc::sc_register<uint32_t> INTEN_00_31;
    scc::sc_register<uint32_t> INTTYPE_00_15;
    scc::sc_register<uint32_t> INTTYPE_16_31;
    scc::sc_register<uint32_t> INTSTATUS_00_31;
    scc::sc_register<uint32_t> PADCFG_00_07;
    scc::sc_register<uint32_t> PADCFG_08_15;
    scc::sc_register<uint32_t> PADCFG_16_23;
    scc::sc_register<uint32_t> PADCFG_24_31;
    scc::sc_register<uint32_t> PADDIR_32_63;
    scc::sc_register<uint32_t> GPIOEN_32_63;
    scc::sc_register<uint32_t> PADIN_32_63;
    scc::sc_register<uint32_t> PADOUT_32_63;
    scc::sc_register<uint32_t> PADOUTSET_32_63;
    scc::sc_register<uint32_t> PADOUTCLR_32_63;
    scc::sc_register<uint32_t> INTEN_32_63;
    scc::sc_register<uint32_t> INTTYPE_32_47;
    scc::sc_register<uint32_t> INTTYPE_48_63;
    scc::sc_register<uint32_t> INTSTATUS_32_63;
    scc::sc_register<uint32_t> PADCFG_32_39;
    scc::sc_register<uint32_t> PADCFG_40_47;
    scc::sc_register<uint32_t> PADCFG_48_55;
    scc::sc_register<uint32_t> PADCFG_56_63;
    
    gpio_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
} // namespace vpvper
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::pulpissimo::gen::gpio_regs::gpio_regs(sc_core::sc_module_name nm)
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
, NAMED(PADOUTSET_32_63, r_PADOUTSET_32_63, 0, *this)
, NAMED(PADOUTCLR_32_63, r_PADOUTCLR_32_63, 0, *this)
, NAMED(INTEN_32_63, r_INTEN_32_63, 0, *this)
, NAMED(INTTYPE_32_47, r_INTTYPE_32_47, 0, *this)
, NAMED(INTTYPE_48_63, r_INTTYPE_48_63, 0, *this)
, NAMED(INTSTATUS_32_63, r_INTSTATUS_32_63, 0, *this)
, NAMED(PADCFG_32_39, r_PADCFG_32_39, 0, *this)
, NAMED(PADCFG_40_47, r_PADCFG_40_47, 0, *this)
, NAMED(PADCFG_48_55, r_PADCFG_48_55, 0, *this)
, NAMED(PADCFG_56_63, r_PADCFG_56_63, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void vpvper::pulpissimo::gen::gpio_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(PADDIR_00_31, 0x0UL+offset);
    target.addResource(GPIOEN_00_31, 0x4UL+offset);
    target.addResource(PADIN_00_31, 0x8UL+offset);
    target.addResource(PADOUT_00_31, 0xcUL+offset);
    target.addResource(PADOUTSET_00_31, 0x10UL+offset);
    target.addResource(PADOUTCLR_00_31, 0x14UL+offset);
    target.addResource(INTEN_00_31, 0x18UL+offset);
    target.addResource(INTTYPE_00_15, 0x1cUL+offset);
    target.addResource(INTTYPE_16_31, 0x20UL+offset);
    target.addResource(INTSTATUS_00_31, 0x24UL+offset);
    target.addResource(PADCFG_00_07, 0x28UL+offset);
    target.addResource(PADCFG_08_15, 0x2cUL+offset);
    target.addResource(PADCFG_16_23, 0x30UL+offset);
    target.addResource(PADCFG_24_31, 0x34UL+offset);
    target.addResource(PADDIR_32_63, 0x38UL+offset);
    target.addResource(GPIOEN_32_63, 0x3cUL+offset);
    target.addResource(PADIN_32_63, 0x40UL+offset);
    target.addResource(PADOUT_32_63, 0x44UL+offset);
    target.addResource(PADOUTSET_32_63, 0x48UL+offset);
    target.addResource(PADOUTCLR_32_63, 0x4cUL+offset);
    target.addResource(INTEN_32_63, 0x50UL+offset);
    target.addResource(INTTYPE_32_47, 0x54UL+offset);
    target.addResource(INTTYPE_48_63, 0x58UL+offset);
    target.addResource(INTSTATUS_32_63, 0x5cUL+offset);
    target.addResource(PADCFG_32_39, 0x60UL+offset);
    target.addResource(PADCFG_40_47, 0x64UL+offset);
    target.addResource(PADCFG_48_55, 0x68UL+offset);
    target.addResource(PADCFG_56_63, 0x6cUL+offset);
}

