/*
* Copyright (c) 2023 - 2025 MINRES Technologies GmbH
*
* SPDX-License-Identifier: Apache-2.0
*
* Generated at 2025-01-21 12:45:36 UTC 
* by peakrdl_mnrs version 1.2.9
*/

#ifndef _SYSCTRL_H_
#define _SYSCTRL_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class sysctrl_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(sysctrl_t, uint32_t);
        BF_FIELD(cc_reset, 0, 2);
        BF_FIELD(mem_reset, 2, 1);
        END_BF_DECL() r_sysctrl;
    
        BEGIN_BF_DECL(pllctrl_t, uint32_t);
        BF_FIELD(p_counter, 0, 6);
        BF_FIELD(s_counter, 6, 2);
        BF_FIELD(locked, 31, 1);
        END_BF_DECL() r_pllctrl;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<sysctrl_t> sysctrl;
        scc::sc_register<pllctrl_t> pllctrl;

    sysctrl_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline sysctrl_regs::sysctrl_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(sysctrl, r_sysctrl, 0, *this)
, NAMED(pllctrl, r_pllctrl, 0, *this) {}

template <unsigned BUSWIDTH> inline void sysctrl_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(sysctrl, 0x0UL);
    target.addResource(pllctrl, 0x4UL);
}
}//namespace minres
}//namespace vpvper
#endif // _SYSCTRL_H_