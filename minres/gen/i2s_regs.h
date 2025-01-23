/*
* Copyright (c) 2023 - 2025 MINRES Technologies GmbH
*
* SPDX-License-Identifier: Apache-2.0
*
* Generated at 2025-01-21 12:45:36 UTC 
* by peakrdl_mnrs version 1.2.9
*/

#ifndef _I2S_H_
#define _I2S_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class i2s_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        uint32_t r_left_ch;
    
        uint32_t r_right_ch;
    
        BEGIN_BF_DECL(control_t, uint32_t);
        BF_FIELD(mode, 0, 2);
        BF_FIELD(disable_left, 2, 1);
        BF_FIELD(disable_right, 3, 1);
        BF_FIELD(is_master, 4, 1);
        BF_FIELD(sample_size, 5, 2);
        BF_FIELD(pdm_scale, 7, 3);
        END_BF_DECL() r_control;
    
        BEGIN_BF_DECL(status_t, uint32_t);
        BF_FIELD(enabled, 0, 1);
        BF_FIELD(active, 1, 1);
        BF_FIELD(left_avail, 2, 1);
        BF_FIELD(right_avail, 3, 1);
        BF_FIELD(left_overflow, 4, 1);
        BF_FIELD(right_overflow, 5, 1);
        END_BF_DECL() r_status;
    
        BEGIN_BF_DECL(i2s_clock_ctrl_t, uint32_t);
        BF_FIELD(divider, 0, 20);
        END_BF_DECL() r_i2s_clock_ctrl;
    
        BEGIN_BF_DECL(pdm_clock_ctrl_t, uint32_t);
        BF_FIELD(divider, 0, 8);
        END_BF_DECL() r_pdm_clock_ctrl;
    
        BEGIN_BF_DECL(pdm_filter_ctrl_t, uint32_t);
        BF_FIELD(decimationFactor, 0, 10);
        END_BF_DECL() r_pdm_filter_ctrl;
    
        BEGIN_BF_DECL(ie_t, uint32_t);
        BF_FIELD(en_left_sample_avail, 0, 1);
        BF_FIELD(en_right_sample_avail, 1, 1);
        END_BF_DECL() r_ie;
    
        BEGIN_BF_DECL(ip_t, uint32_t);
        BF_FIELD(left_sample_avail, 0, 1);
        BF_FIELD(right_sample_avail, 1, 1);
        END_BF_DECL() r_ip;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<uint32_t> left_ch;
        scc::sc_register<uint32_t> right_ch;
        scc::sc_register<control_t> control;
        scc::sc_register<status_t> status;
        scc::sc_register<i2s_clock_ctrl_t> i2s_clock_ctrl;
        scc::sc_register<pdm_clock_ctrl_t> pdm_clock_ctrl;
        scc::sc_register<pdm_filter_ctrl_t> pdm_filter_ctrl;
        scc::sc_register<ie_t> ie;
        scc::sc_register<ip_t> ip;

    i2s_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline i2s_regs::i2s_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(left_ch, r_left_ch, 0, *this)
, NAMED(right_ch, r_right_ch, 0, *this)
, NAMED(control, r_control, 0, *this)
, NAMED(status, r_status, 0, *this)
, NAMED(i2s_clock_ctrl, r_i2s_clock_ctrl, 0, *this)
, NAMED(pdm_clock_ctrl, r_pdm_clock_ctrl, 0, *this)
, NAMED(pdm_filter_ctrl, r_pdm_filter_ctrl, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this) {}

template <unsigned BUSWIDTH> inline void i2s_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(left_ch, 0x0UL);
    target.addResource(right_ch, 0x4UL);
    target.addResource(control, 0x8UL);
    target.addResource(status, 0xcUL);
    target.addResource(i2s_clock_ctrl, 0x10UL);
    target.addResource(pdm_clock_ctrl, 0x14UL);
    target.addResource(pdm_filter_ctrl, 0x18UL);
    target.addResource(ie, 0x1cUL);
    target.addResource(ip, 0x20UL);
}
}//namespace minres
}//namespace vpvper
#endif // _I2S_H_