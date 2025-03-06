/*
 * Copyright (c) 2023 - 2025 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2025-01-21 12:45:36 UTC
 * by peakrdl_mnrs version 1.2.9
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class camera_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    uint32_t r_pixel;

    BEGIN_BF_DECL(config_t, uint32_t);
    BF_FIELD(rows_in_reset, 0, 8);
    BF_FIELD(vrst_pix, 8, 2);
    BF_FIELD(ramp_gain, 10, 2);
    BF_FIELD(offset_ramp, 12, 2);
    BF_FIELD(output_curr, 14, 2);
    BF_FIELD(rows_delay, 16, 5);
    BF_FIELD(bias_curr_increase, 21, 1);
    BF_FIELD(cds_gain, 22, 1);
    BF_FIELD(output_mode, 23, 1);
    BF_FIELD(mclk_mode, 24, 2);
    BF_FIELD(vref, 26, 2);
    BF_FIELD(cvc_curr, 28, 2);
    BF_FIELD(idle_mode, 30, 1);
    BF_FIELD(high_speed, 31, 1);
    END_BF_DECL() r_config;

    BEGIN_BF_DECL(data_size_t, uint32_t);
    BF_FIELD(data_size, 0, 2);
    END_BF_DECL() r_data_size;

    BEGIN_BF_DECL(start_t, uint32_t);
    BF_FIELD(start, 0, 1);
    END_BF_DECL() r_start;

    BEGIN_BF_DECL(status_t, uint32_t);
    BF_FIELD(pixel_avail, 0, 1);
    END_BF_DECL() r_status;

    BEGIN_BF_DECL(camera_clock_ctrl_t, uint32_t);
    BF_FIELD(divider, 0, 20);
    END_BF_DECL() r_camera_clock_ctrl;

    BEGIN_BF_DECL(ie_t, uint32_t);
    BF_FIELD(en_pixel_avail, 0, 1);
    BF_FIELD(en_frame_finished, 1, 1);
    END_BF_DECL() r_ie;

    BEGIN_BF_DECL(ip_t, uint32_t);
    BF_FIELD(pixel_avail_irq_pend, 0, 1);
    BF_FIELD(frame_finished_irq_pend, 1, 1);
    END_BF_DECL() r_ip;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    scc::sc_register<uint32_t> pixel;
    scc::sc_register<config_t> config;
    scc::sc_register<data_size_t> data_size;
    scc::sc_register<start_t> start;
    scc::sc_register<status_t> status;
    scc::sc_register<camera_clock_ctrl_t> camera_clock_ctrl;
    scc::sc_register<ie_t> ie;
    scc::sc_register<ip_t> ip;

    camera_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline camera_regs::camera_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(pixel, r_pixel, 0, *this)
, NAMED(config, r_config, 0, *this)
, NAMED(data_size, r_data_size, 0, *this)
, NAMED(start, r_start, 0, *this)
, NAMED(status, r_status, 0, *this)
, NAMED(camera_clock_ctrl, r_camera_clock_ctrl, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this) {}

template <unsigned BUSWIDTH> inline void camera_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(pixel, 0x0UL);
    target.addResource(config, 0x4UL);
    target.addResource(data_size, 0x8UL);
    target.addResource(start, 0xcUL);
    target.addResource(status, 0x10UL);
    target.addResource(camera_clock_ctrl, 0x14UL);
    target.addResource(ie, 0x18UL);
    target.addResource(ip, 0x1cUL);
}
} // namespace minres
} // namespace vpvper
#endif // _CAMERA_H_