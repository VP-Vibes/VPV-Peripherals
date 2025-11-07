/*
 * Copyright (c) 2023 - 2025 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2025-01-21 12:45:36 UTC
 * by peakrdl_mnrs version 1.2.9
 */

#ifndef _DMA_H_
#define _DMA_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class dma_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(control_t, uint32_t);
    BF_FIELD(ch0_enable_transfer, 0, 1);
    BF_FIELD(ch1_enable_transfer, 1, 1);
    END_BF_DECL() r_control;

    BEGIN_BF_DECL(status_t, uint32_t);
    BF_FIELD(ch0_busy, 0, 1);
    BF_FIELD(ch1_busy, 1, 1);
    END_BF_DECL() r_status;

    BEGIN_BF_DECL(ie_t, uint32_t);
    BF_FIELD(ch0_ie_seg_transfer_done, 0, 1);
    BF_FIELD(ch0_ie_transfer_done, 1, 1);
    BF_FIELD(ch1_ie_seg_transfer_done, 2, 1);
    BF_FIELD(ch1_ie_transfer_done, 3, 1);
    END_BF_DECL() r_ie;

    BEGIN_BF_DECL(ip_t, uint32_t);
    BF_FIELD(ch0_ip_seg_transfer_done, 0, 1);
    BF_FIELD(ch0_ip_transfer_done, 1, 1);
    BF_FIELD(ch1_ip_seg_transfer_done, 2, 1);
    BF_FIELD(ch1_ip_transfer_done, 3, 1);
    END_BF_DECL() r_ip;

    BEGIN_BF_DECL(ch0_event_t, uint32_t);
    BF_FIELD(select, 0, 5);
    BF_FIELD(combine, 31, 1);
    END_BF_DECL() r_ch0_event;

    BEGIN_BF_DECL(ch0_transfer_t, uint32_t);
    BF_FIELD(width, 0, 2);
    BF_FIELD(seg_length, 2, 10);
    BF_FIELD(seg_count, 12, 20);
    END_BF_DECL() r_ch0_transfer;

    uint32_t r_ch0_src_start_addr;

    BEGIN_BF_DECL(ch0_src_addr_inc_t, uint32_t);
    BF_FIELD(src_step, 0, 12);
    BF_FIELD(src_stride, 12, 20);
    END_BF_DECL() r_ch0_src_addr_inc;

    uint32_t r_ch0_dst_start_addr;

    BEGIN_BF_DECL(ch0_dst_addr_inc_t, uint32_t);
    BF_FIELD(dst_step, 0, 12);
    BF_FIELD(dst_stride, 12, 20);
    END_BF_DECL() r_ch0_dst_addr_inc;

    BEGIN_BF_DECL(ch1_event_t, uint32_t);
    BF_FIELD(select, 0, 5);
    BF_FIELD(combine, 31, 1);
    END_BF_DECL() r_ch1_event;

    BEGIN_BF_DECL(ch1_transfer_t, uint32_t);
    BF_FIELD(width, 0, 2);
    BF_FIELD(seg_length, 2, 10);
    BF_FIELD(seg_count, 12, 20);
    END_BF_DECL() r_ch1_transfer;

    uint32_t r_ch1_src_start_addr;

    BEGIN_BF_DECL(ch1_src_addr_inc_t, uint32_t);
    BF_FIELD(src_step, 0, 12);
    BF_FIELD(src_stride, 12, 20);
    END_BF_DECL() r_ch1_src_addr_inc;

    uint32_t r_ch1_dst_start_addr;

    BEGIN_BF_DECL(ch1_dst_addr_inc_t, uint32_t);
    BF_FIELD(dst_step, 0, 12);
    BF_FIELD(dst_stride, 12, 20);
    END_BF_DECL() r_ch1_dst_addr_inc;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    scc::sc_register<control_t> control;
    scc::sc_register<status_t> status;
    scc::sc_register<ie_t> ie;
    scc::sc_register<ip_t> ip;
    scc::sc_register<ch0_event_t> ch0_event;
    scc::sc_register<ch0_transfer_t> ch0_transfer;
    scc::sc_register<uint32_t> ch0_src_start_addr;
    scc::sc_register<ch0_src_addr_inc_t> ch0_src_addr_inc;
    scc::sc_register<uint32_t> ch0_dst_start_addr;
    scc::sc_register<ch0_dst_addr_inc_t> ch0_dst_addr_inc;
    scc::sc_register<ch1_event_t> ch1_event;
    scc::sc_register<ch1_transfer_t> ch1_transfer;
    scc::sc_register<uint32_t> ch1_src_start_addr;
    scc::sc_register<ch1_src_addr_inc_t> ch1_src_addr_inc;
    scc::sc_register<uint32_t> ch1_dst_start_addr;
    scc::sc_register<ch1_dst_addr_inc_t> ch1_dst_addr_inc;

    dma_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline dma_regs::dma_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(control, r_control, 0, *this)
, NAMED(status, r_status, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this)
, NAMED(ch0_event, r_ch0_event, 0, *this)
, NAMED(ch0_transfer, r_ch0_transfer, 0, *this)
, NAMED(ch0_src_start_addr, r_ch0_src_start_addr, 0, *this)
, NAMED(ch0_src_addr_inc, r_ch0_src_addr_inc, 0, *this)
, NAMED(ch0_dst_start_addr, r_ch0_dst_start_addr, 0, *this)
, NAMED(ch0_dst_addr_inc, r_ch0_dst_addr_inc, 0, *this)
, NAMED(ch1_event, r_ch1_event, 0, *this)
, NAMED(ch1_transfer, r_ch1_transfer, 0, *this)
, NAMED(ch1_src_start_addr, r_ch1_src_start_addr, 0, *this)
, NAMED(ch1_src_addr_inc, r_ch1_src_addr_inc, 0, *this)
, NAMED(ch1_dst_start_addr, r_ch1_dst_start_addr, 0, *this)
, NAMED(ch1_dst_addr_inc, r_ch1_dst_addr_inc, 0, *this) {}

template <unsigned BUSWIDTH> inline void dma_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(control, 0x0UL);
    target.addResource(status, 0x4UL);
    target.addResource(ie, 0x8UL);
    target.addResource(ip, 0xcUL);
    target.addResource(ch0_event, 0x10UL);
    target.addResource(ch0_transfer, 0x14UL);
    target.addResource(ch0_src_start_addr, 0x18UL);
    target.addResource(ch0_src_addr_inc, 0x1cUL);
    target.addResource(ch0_dst_start_addr, 0x20UL);
    target.addResource(ch0_dst_addr_inc, 0x24UL);
    target.addResource(ch1_event, 0x28UL);
    target.addResource(ch1_transfer, 0x2cUL);
    target.addResource(ch1_src_start_addr, 0x30UL);
    target.addResource(ch1_src_addr_inc, 0x34UL);
    target.addResource(ch1_dst_start_addr, 0x38UL);
    target.addResource(ch1_dst_addr_inc, 0x3cUL);
}
} // namespace minres
} // namespace vpvper
#endif // _DMA_H_