/*
 * Copyright (c) 2019 - 2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _APB3GPIO_H_
#define _APB3GPIO_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class Apb3Gpio : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        uint32_t r_value;
    
        uint32_t r_write;
    
        uint32_t r_writeEnable;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<uint32_t> value;
        scc::sc_register<uint32_t> write;
        scc::sc_register<uint32_t> writeEnable;

    Apb3Gpio(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline Apb3Gpio::Apb3Gpio(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(value, r_value, 0, *this)
, NAMED(write, r_write, 0, *this)
, NAMED(writeEnable, r_writeEnable, 0, *this) {}

template <unsigned BUSWIDTH> inline void Apb3Gpio::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(value, 0x0UL);
    target.addResource(write, 0x4UL);
    target.addResource(writeEnable, 0x8UL);
}
}//namespace minres
}//namespace vpvper
#endif // _APB3GPIO_H_