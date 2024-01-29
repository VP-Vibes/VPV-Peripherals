/*
 * Copyright (c) 2019 - 2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _APB3IRQCTRL_H_
#define _APB3IRQCTRL_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class Apb3IrqCtrl : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(pendingsReg_t, uint32_t);
        BF_FIELD(pendings, 0, 4);
        END_BF_DECL() r_pendingsReg;
    
        BEGIN_BF_DECL(masksReg_t, uint32_t);
        BF_FIELD(masks, 0, 4);
        END_BF_DECL() r_masksReg;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<pendingsReg_t> pendingsReg;
        scc::sc_register<masksReg_t> masksReg;

    Apb3IrqCtrl(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline Apb3IrqCtrl::Apb3IrqCtrl(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(pendingsReg, r_pendingsReg, 0, *this)
, NAMED(masksReg, r_masksReg, 0, *this) {}

template <unsigned BUSWIDTH> inline void Apb3IrqCtrl::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(pendingsReg, 0x0UL);
    target.addResource(masksReg, 0x4UL);
}
}//namespace minres
}//namespace vpvper
#endif // _APB3IRQCTRL_H_