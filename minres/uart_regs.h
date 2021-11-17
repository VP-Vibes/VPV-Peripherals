/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MINRES_UART_REGS_H_
#define _MINRES_UART_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class uart_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(rxtxdata_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(rx_avail, 16, 1);
    END_BF_DECL() r_rxtxdata;

    BEGIN_BF_DECL(irq_ctrl_t, uint32_t);
    BF_FIELD(tx_ie, 0, 1);
    BF_FIELD(rx_ie, 1, 1);
    BF_FIELD(tx_ip, 8, 1);
    BF_FIELD(rx_ip, 9, 1);
    BF_FIELD(num_tx_avail, 16, 8);
    BF_FIELD(num_rx_avail, 24, 8);
    END_BF_DECL() r_irq_ctrl;

    BEGIN_BF_DECL(clock_config_t, uint32_t);
    BF_FIELD(clockDivider, 0, 20);
    END_BF_DECL() r_clock_config;

    BEGIN_BF_DECL(frame_t, uint32_t);
    BF_FIELD(data_length, 0, 3);
    BF_FIELD(parity, 1, 2);
    BF_FIELD(stop_bits, 16, 1);
    END_BF_DECL() r_frame;

    BEGIN_BF_DECL(status_t, uint32_t);
    BF_FIELD(readError, 0, 1);
    BF_FIELD(readOverflowError, 1, 1);
    BF_FIELD(rx_active, 8, 1);
    BF_FIELD(tx_active, 9, 1);
    BF_FIELD(set_tx_active, 10, 1);
    BF_FIELD(clear_tx_active, 11, 1);
    END_BF_DECL() r_status;

    // register declarations
    scc::sc_register<rxtxdata_t> rxtxdata;
    scc::sc_register<irq_ctrl_t> irq_ctrl;
    scc::sc_register<clock_config_t> clock_config;
    scc::sc_register<frame_t> frame;
    scc::sc_register<status_t> status;

    uart_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);

    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace minres */
} /* namespace vpvper */
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::minres::uart_regs::uart_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(rxtxdata, r_rxtxdata, 0, *this)
, NAMED(irq_ctrl, r_irq_ctrl, 0, *this)
, NAMED(clock_config, r_clock_config, 0, *this)
, NAMED(frame, r_frame, 0, *this)
, NAMED(status, r_status, 0, *this)
{}

template <unsigned BUSWIDTH> inline void vpvper::minres::uart_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(rxtxdata, 0x0UL);
    target.addResource(irq_ctrl, 0x4UL);
    target.addResource(clock_config, 0x8UL);
    target.addResource(frame, 0xCUL);
    target.addResource(status, 0x10UL);
}

inline void vpvper::minres::uart_regs::trace(sc_core::sc_trace_file *tf) const {
    rxtxdata.trace(tf);
    irq_ctrl.trace(tf);
    clock_config.trace(tf);
    frame.trace(tf);
    status.trace(tf);
}

#endif // _MINRES_UART_REGS_H_
