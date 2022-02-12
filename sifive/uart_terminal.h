/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _GENERIC_UART_TERMINAL_H_
#define _GENERIC_UART_TERMINAL_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>

namespace vpvper {
namespace sifive {

class uart_regs;

class uart_terminal : public sc_core::sc_module, public scc::tlm_target<> {
public:

    sc_core::sc_in<bool> rst_i{"rst_i"};
#ifdef BITTRUE
    sc_core::sc_out<bool> tx_o{"tx_o"};
#endif
    sc_core::sc_out<bool> irq_o{"irq_o"};

    uart_terminal(sc_core::sc_module_name nm);
    virtual ~uart_terminal() override;

    void set_clock_period(sc_core::sc_time period) {
        clk=period;
    }
protected:
    void reset_cb();
    void transmit_data();
    void update_irq();
    sc_core::sc_time clk;
    std::unique_ptr<uart_regs> regs;
    sc_core::sc_fifo<uint8_t> rx_fifo{"rx_fifo", 8}, tx_fifo{"tx_fifo", 8};
    std::vector<uint8_t> queue;
};

using uart_terminal_clk =scc::ticking_clock<uart_terminal>;
using uart_terminal_time =scc::tickless_clock<uart_terminal>;

} /* namespace generic */
} /* namespace vpvper */

#endif /* _GENERIC_UART_TERMINAL_H_ */
