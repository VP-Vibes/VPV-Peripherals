/*
* Copyright (c) 2023 - 2024 MINRES Technologies GmbH
*
* SPDX-License-Identifier: Apache-2.0
*
* Generated at 2024-02-08 14:41:42 UTC 
* by peakrdl_mnrs version 1.2.2
*/

#ifndef _APB3UART_H_
#define _APB3UART_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class Apb3Uart_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(rx_tx_reg_t, uint32_t);
        BF_FIELD(data, 0, 8);
        BF_FIELD(rx_avail, 14, 1);
        BF_FIELD(tx_free, 15, 1);
        END_BF_DECL() r_rx_tx_reg;
    
        BEGIN_BF_DECL(int_ctrl_reg_t, uint32_t);
        BF_FIELD(write_intr_enable, 0, 1);
        BF_FIELD(read_intr_enable, 1, 1);
        BF_FIELD(write_intr_pend, 8, 1);
        BF_FIELD(read_intr_pend, 9, 1);
        END_BF_DECL() r_int_ctrl_reg;
    
        BEGIN_BF_DECL(clk_divider_reg_t, uint32_t);
        BF_FIELD(clock_divider, 0, 20);
        END_BF_DECL() r_clk_divider_reg;
    
        BEGIN_BF_DECL(frame_config_reg_t, uint32_t);
        BF_FIELD(data_lenght, 0, 3);
        BF_FIELD(parity, 3, 2);
        BF_FIELD(stop_bit, 5, 1);
        END_BF_DECL() r_frame_config_reg;
    
        BEGIN_BF_DECL(status_reg_t, uint32_t);
        BF_FIELD(read_error, 0, 1);
        BF_FIELD(stall, 1, 1);
        BF_FIELD(break_rd, 8, 1);
        BF_FIELD(break_wr, 9, 1);
        BF_FIELD(set_break, 10, 1);
        BF_FIELD(clear_break, 11, 1);
        END_BF_DECL() r_status_reg;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<rx_tx_reg_t> rx_tx_reg;
        scc::sc_register<int_ctrl_reg_t> int_ctrl_reg;
        scc::sc_register<clk_divider_reg_t> clk_divider_reg;
        scc::sc_register<frame_config_reg_t> frame_config_reg;
        scc::sc_register<status_reg_t> status_reg;

    Apb3Uart_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline Apb3Uart_regs::Apb3Uart_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(rx_tx_reg, r_rx_tx_reg, 0, *this)
, NAMED(int_ctrl_reg, r_int_ctrl_reg, 0, *this)
, NAMED(clk_divider_reg, r_clk_divider_reg, 0, *this)
, NAMED(frame_config_reg, r_frame_config_reg, 0, *this)
, NAMED(status_reg, r_status_reg, 0, *this) {}

template <unsigned BUSWIDTH> inline void Apb3Uart_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(rx_tx_reg, 0x0UL);
    target.addResource(int_ctrl_reg, 0x4UL);
    target.addResource(clk_divider_reg, 0x8UL);
    target.addResource(frame_config_reg, 0xcUL);
    target.addResource(status_reg, 0x10UL);
}
}//namespace minres
}//namespace vpvper
#endif // _APB3UART_H_
