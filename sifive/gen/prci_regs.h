/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PRCI_REGS_H_
#define _PRCI_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class prci_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(hfrosccfg_t, uint32_t);
    BF_FIELD(hfroscdiv, 0, 6);
    BF_FIELD(hfrosctrim, 16, 5);
    BF_FIELD(hfroscen, 30, 1);
    BF_FIELD(hfroscrdy, 31, 1);
    END_BF_DECL() r_hfrosccfg;

    BEGIN_BF_DECL(hfxosccfg_t, uint32_t);
    BF_FIELD(hfxoscrdy, 31, 1);
    BF_FIELD(hfxoscen, 30, 1);
    END_BF_DECL() r_hfxosccfg;

    BEGIN_BF_DECL(pllcfg_t, uint32_t);
    BF_FIELD(pllr, 0, 3);
    BF_FIELD(pllf, 4, 6);
    BF_FIELD(pllq, 10, 2);
    BF_FIELD(pllsel, 16, 1);
    BF_FIELD(pllrefsel, 17, 1);
    BF_FIELD(pllbypass, 18, 1);
    BF_FIELD(plllock, 31, 1);
    END_BF_DECL() r_pllcfg;

    uint32_t r_plloutdiv;

    uint32_t r_coreclkcfg;

    // register declarations
    scc::sc_register<hfrosccfg_t> hfrosccfg;
    scc::sc_register<hfxosccfg_t> hfxosccfg;
    scc::sc_register<pllcfg_t> pllcfg;
    scc::sc_register<uint32_t> plloutdiv;
    scc::sc_register<uint32_t> coreclkcfg;

    prci_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::prci_regs::prci_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(hfrosccfg, r_hfrosccfg, 0, *this)
, NAMED(hfxosccfg, r_hfxosccfg, 0x40000000, *this)
, NAMED(pllcfg, r_pllcfg, 0, *this)
, NAMED(plloutdiv, r_plloutdiv, 0, *this)
, NAMED(coreclkcfg, r_coreclkcfg, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::prci_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(hfrosccfg, 0x0UL);
    target.addResource(hfxosccfg, 0x4UL);
    target.addResource(pllcfg, 0x8UL);
    target.addResource(plloutdiv, 0xcUL);
    target.addResource(coreclkcfg, 0x10UL);
}

#endif // _PRCI_REGS_H_
