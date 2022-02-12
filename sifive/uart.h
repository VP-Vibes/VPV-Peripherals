/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UART_H_
#define _UART_H_

#include <cci_configuration>
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#include <scc/tlm_target.h>
#include <tlm/scc/tlm_signal.h>

namespace vpvper {
namespace sifive {

class tlm_signal_uart_extension;
class uart_regs;
class WsHandler;

class uart : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    tlm::scc::tlm_signal_bool_out tx_o;
    tlm::scc::tlm_signal_bool_in rx_i;

    sc_core::sc_out<bool> irq_o;

    cci::cci_param<bool> bit_true_transfer;

    uart(sc_core::sc_module_name const& nm);
    virtual ~uart() override;

protected:
    void clock_cb();
    void reset_cb();
    void transmit_data();
    void receive_data(tlm::scc::tlm_signal_gp<> &gp, sc_core::sc_time &delay);
    void update_irq();
    sc_core::sc_time clk{sc_core::SC_ZERO_TIME}, rx_last_start{sc_core::SC_ZERO_TIME};
    std::unique_ptr<uart_regs> regs;
    sc_core::sc_fifo<uint8_t> rx_fifo, tx_fifo;
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _UART_H_ */
