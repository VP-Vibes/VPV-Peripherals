/*
 * Copyright (c) 2019 - 2024 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _APB3SPIXDRMASTERCTRL_H_
#define _APB3SPIXDRMASTERCTRL_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class Apb3SpiXdrMasterCtrl_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(data_t, uint32_t);
        BF_FIELD(data, 0, 8);
        BF_FIELD(write, 8, 1);
        BF_FIELD(read, 9, 1);
        BF_FIELD(kind, 11, 1);
        BF_FIELD(rx_data_invalid, 31, 1);
        END_BF_DECL() r_data;
    
        BEGIN_BF_DECL(status_t, uint32_t);
        BF_FIELD(tx_free, 0, 6);
        BF_FIELD(rx_avail, 16, 6);
        END_BF_DECL() r_status;
    
        BEGIN_BF_DECL(config_t, uint32_t);
        BF_FIELD(kind, 0, 2);
        BF_FIELD(mode, 4, 3);
        END_BF_DECL() r_config;
    
        BEGIN_BF_DECL(intr_t, uint32_t);
        BF_FIELD(tx_ie, 0, 1);
        BF_FIELD(rx_ie, 1, 1);
        BF_FIELD(tx_ip, 8, 1);
        BF_FIELD(rx_ip, 9, 1);
        BF_FIELD(tx_active, 16, 1);
        END_BF_DECL() r_intr;
    
        BEGIN_BF_DECL(sclk_config_t, uint32_t);
        BF_FIELD(clk_divider, 0, 12);
        END_BF_DECL() r_sclk_config;
    
        BEGIN_BF_DECL(ssgen_setup_t, uint32_t);
        BF_FIELD(setup_cycles, 0, 12);
        END_BF_DECL() r_ssgen_setup;
    
        BEGIN_BF_DECL(ssgen_hold_t, uint32_t);
        BF_FIELD(hold_cycles, 0, 12);
        END_BF_DECL() r_ssgen_hold;
    
        BEGIN_BF_DECL(ssgen_disable_t, uint32_t);
        BF_FIELD(disable_cycles, 0, 12);
        END_BF_DECL() r_ssgen_disable;
    
        BEGIN_BF_DECL(ssgen_active_high_t, uint32_t);
        BF_FIELD(high_cycles, 0, 1);
        END_BF_DECL() r_ssgen_active_high;
    
        BEGIN_BF_DECL(xip_enable_t, uint32_t);
        BF_FIELD(enable, 0, 1);
        END_BF_DECL() r_xip_enable;
    
        BEGIN_BF_DECL(xip_config_t, uint32_t);
        BF_FIELD(instruction, 0, 8);
        BF_FIELD(enable, 8, 1);
        BF_FIELD(dummy_value, 16, 8);
        BF_FIELD(dummy_count, 24, 4);
        END_BF_DECL() r_xip_config;
    
        BEGIN_BF_DECL(xip_mode_t, uint32_t);
        BF_FIELD(instruction, 0, 3);
        BF_FIELD(address, 8, 3);
        BF_FIELD(dummy, 16, 3);
        BF_FIELD(payload, 24, 3);
        END_BF_DECL() r_xip_mode;
    
        BEGIN_BF_DECL(xip_write_t, uint32_t);
        BF_FIELD(data, 0, 8);
        END_BF_DECL() r_xip_write;
    
        BEGIN_BF_DECL(xip_read_write_t, uint32_t);
        BF_FIELD(data, 0, 8);
        END_BF_DECL() r_xip_read_write;
    
        BEGIN_BF_DECL(xip_read_t, uint32_t);
        BF_FIELD(data, 0, 8);
        END_BF_DECL() r_xip_read;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<data_t> data;
        scc::sc_register<status_t> status;
        scc::sc_register<config_t> config;
        scc::sc_register<intr_t> intr;
        scc::sc_register<sclk_config_t> sclk_config;
        scc::sc_register<ssgen_setup_t> ssgen_setup;
        scc::sc_register<ssgen_hold_t> ssgen_hold;
        scc::sc_register<ssgen_disable_t> ssgen_disable;
        scc::sc_register<ssgen_active_high_t> ssgen_active_high;
        scc::sc_register<xip_enable_t> xip_enable;
        scc::sc_register<xip_config_t> xip_config;
        scc::sc_register<xip_mode_t> xip_mode;
        scc::sc_register<xip_write_t> xip_write;
        scc::sc_register<xip_read_write_t> xip_read_write;
        scc::sc_register<xip_read_t> xip_read;

    Apb3SpiXdrMasterCtrl_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline Apb3SpiXdrMasterCtrl_regs::Apb3SpiXdrMasterCtrl_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(data, r_data, 0, *this)
, NAMED(status, r_status, 0, *this)
, NAMED(config, r_config, 0, *this)
, NAMED(intr, r_intr, 0, *this)
, NAMED(sclk_config, r_sclk_config, 0, *this)
, NAMED(ssgen_setup, r_ssgen_setup, 0, *this)
, NAMED(ssgen_hold, r_ssgen_hold, 0, *this)
, NAMED(ssgen_disable, r_ssgen_disable, 0, *this)
, NAMED(ssgen_active_high, r_ssgen_active_high, 0, *this)
, NAMED(xip_enable, r_xip_enable, 0, *this)
, NAMED(xip_config, r_xip_config, 0, *this)
, NAMED(xip_mode, r_xip_mode, 0, *this)
, NAMED(xip_write, r_xip_write, 0, *this)
, NAMED(xip_read_write, r_xip_read_write, 0, *this)
, NAMED(xip_read, r_xip_read, 0, *this) {}

template <unsigned BUSWIDTH> inline void Apb3SpiXdrMasterCtrl_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(data, 0x0UL);
    target.addResource(status, 0x4UL);
    target.addResource(config, 0x8UL);
    target.addResource(intr, 0xcUL);
    target.addResource(sclk_config, 0x20UL);
    target.addResource(ssgen_setup, 0x24UL);
    target.addResource(ssgen_hold, 0x28UL);
    target.addResource(ssgen_disable, 0x2cUL);
    target.addResource(ssgen_active_high, 0x30UL);
    target.addResource(xip_enable, 0x40UL);
    target.addResource(xip_config, 0x44UL);
    target.addResource(xip_mode, 0x48UL);
    target.addResource(xip_write, 0x50UL);
    target.addResource(xip_read_write, 0x54UL);
    target.addResource(xip_read, 0x58UL);
}
}//namespace minres
}//namespace vpvper
#endif // _APB3SPIXDRMASTERCTRL_H_