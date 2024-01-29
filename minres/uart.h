/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MINRES_UART_TERMINAL_H_
#define _MINRES_UART_TERMINAL_H_

#include <scc/tlm_target.h>
#include <scc/clock_if_mixins.h>
#include <cci_configuration>

namespace vpvper {
namespace minres {

class Apb3Uart;

class uart : public sc_core::sc_module, public scc::tlm_target<> {
public:

    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_out<bool> tx_o{"tx_o"};

    sc_core::sc_out<bool> irq_o{"irq_o"};

    uart(sc_core::sc_module_name nm);

    virtual ~uart() override;

    void set_clock_period(sc_core::sc_time period) {
        clk_period=period;
    }

    cci::cci_param<unsigned> fifo_depth{"fifo_depth", 32};

    cci::cci_param<bool> bit_true_transfer{"bit_true_transfer", false};

protected:
    void reset_cb();
    void transmit_data();
    void update_irq();
    sc_core::sc_time clk_period;
    std::unique_ptr<Apb3Uart> regs;
    sc_core::sc_fifo<uint8_t> rx_fifo{"rx_fifo", 8}, tx_fifo{"tx_fifo", 8};
    std::vector<uint8_t> queue;
};

using uart_terminal_tc =scc::ticking_clock<uart>;
using uart_terminal_tl =scc::tickless_clock<uart>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _MINRES_UART_TERMINAL_H_ */
