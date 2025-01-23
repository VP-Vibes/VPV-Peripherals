/*
* Copyright (c) 2023 - 2025 MINRES Technologies GmbH
*
* SPDX-License-Identifier: Apache-2.0
*
* Generated at 2025-01-21 12:45:36 UTC 
* by peakrdl_mnrs version 1.2.9
*/

#ifndef _GPIO_H_
#define _GPIO_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class gpio_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        uint32_t r_value;
    
        uint32_t r_write;
    
        uint32_t r_writeEnable;
    
        uint32_t r_pullup;
    
        uint32_t r_puldown;
    
        BEGIN_BF_DECL(driveStrength_0_t, uint32_t);
        BF_FIELD(pin_0, 0, 3);
        BF_FIELD(pin_1, 4, 3);
        BF_FIELD(pin_2, 8, 3);
        BF_FIELD(pin_3, 12, 3);
        BF_FIELD(pin_4, 16, 3);
        BF_FIELD(pin_5, 20, 3);
        BF_FIELD(pin_6, 24, 3);
        BF_FIELD(pin_7, 28, 3);
        END_BF_DECL() r_driveStrength_0;
    
        BEGIN_BF_DECL(driveStrength_1_t, uint32_t);
        BF_FIELD(pin_8, 0, 3);
        BF_FIELD(pin_9, 4, 3);
        BF_FIELD(pin_10, 8, 3);
        BF_FIELD(pin_11, 12, 3);
        BF_FIELD(pin_12, 16, 3);
        BF_FIELD(pin_13, 20, 3);
        BF_FIELD(pin_14, 24, 3);
        BF_FIELD(pin_15, 28, 3);
        END_BF_DECL() r_driveStrength_1;
    
        BEGIN_BF_DECL(driveStrength_2_t, uint32_t);
        BF_FIELD(pin_16, 0, 3);
        BF_FIELD(pin_17, 4, 3);
        BF_FIELD(pin_18, 8, 3);
        BF_FIELD(pin_19, 12, 3);
        BF_FIELD(pin_20, 16, 3);
        BF_FIELD(pin_21, 20, 3);
        BF_FIELD(pin_22, 24, 3);
        BF_FIELD(pin_23, 28, 3);
        END_BF_DECL() r_driveStrength_2;
    
        BEGIN_BF_DECL(driveStrength_3_t, uint32_t);
        BF_FIELD(pin_24, 0, 3);
        BF_FIELD(pin_25, 4, 3);
        BF_FIELD(pin_26, 8, 3);
        BF_FIELD(pin_27, 12, 3);
        BF_FIELD(pin_28, 16, 3);
        BF_FIELD(pin_29, 20, 3);
        BF_FIELD(pin_30, 24, 3);
        BF_FIELD(pin_31, 28, 3);
        END_BF_DECL() r_driveStrength_3;
    
        uint32_t r_ie;
    
        uint32_t r_ip;
    
        uint32_t r_irq_trigger;
    
        uint32_t r_irq_type;
    
        BEGIN_BF_DECL(boot_sel_t, uint32_t);
        BF_FIELD(bootSel, 0, 3);
        END_BF_DECL() r_boot_sel;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<uint32_t> value;
        scc::sc_register<uint32_t> write;
        scc::sc_register<uint32_t> writeEnable;
        scc::sc_register<uint32_t> pullup;
        scc::sc_register<uint32_t> puldown;
        scc::sc_register<driveStrength_0_t> driveStrength_0;
        scc::sc_register<driveStrength_1_t> driveStrength_1;
        scc::sc_register<driveStrength_2_t> driveStrength_2;
        scc::sc_register<driveStrength_3_t> driveStrength_3;
        scc::sc_register<uint32_t> ie;
        scc::sc_register<uint32_t> ip;
        scc::sc_register<uint32_t> irq_trigger;
        scc::sc_register<uint32_t> irq_type;
        scc::sc_register<boot_sel_t> boot_sel;

    gpio_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline gpio_regs::gpio_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(value, r_value, 0, *this)
, NAMED(write, r_write, 0, *this)
, NAMED(writeEnable, r_writeEnable, 0, *this)
, NAMED(pullup, r_pullup, 0, *this)
, NAMED(puldown, r_puldown, 0, *this)
, NAMED(driveStrength_0, r_driveStrength_0, 0, *this)
, NAMED(driveStrength_1, r_driveStrength_1, 0, *this)
, NAMED(driveStrength_2, r_driveStrength_2, 0, *this)
, NAMED(driveStrength_3, r_driveStrength_3, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this)
, NAMED(irq_trigger, r_irq_trigger, 0, *this)
, NAMED(irq_type, r_irq_type, 0, *this)
, NAMED(boot_sel, r_boot_sel, 0, *this) {}

template <unsigned BUSWIDTH> inline void gpio_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(value, 0x0UL);
    target.addResource(write, 0x4UL);
    target.addResource(writeEnable, 0x8UL);
    target.addResource(pullup, 0xcUL);
    target.addResource(puldown, 0x10UL);
    target.addResource(driveStrength_0, 0x14UL);
    target.addResource(driveStrength_1, 0x18UL);
    target.addResource(driveStrength_2, 0x1cUL);
    target.addResource(driveStrength_3, 0x20UL);
    target.addResource(ie, 0x24UL);
    target.addResource(ip, 0x28UL);
    target.addResource(irq_trigger, 0x2cUL);
    target.addResource(irq_type, 0x30UL);
    target.addResource(boot_sel, 0x34UL);
}
}//namespace minres
}//namespace vpvper
#endif // _GPIO_H_