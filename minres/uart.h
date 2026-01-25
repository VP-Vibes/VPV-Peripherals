/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _MINRES_UART_H_
#define _MINRES_UART_H_

#include "minres/gen/uart_regs.h"
#include "scc/signal_opt_ports.h"
#include <cci_configuration>
#include <scc/clock_if_mixins.h>
#include <scc/tlm_target.h>
#include <sysc/datatypes/bit/sc_bv.h>

namespace vpvper {
namespace minres {

class uart_regs;

class uart : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    sc_core::sc_out<bool> tx_o{"tx_o"};

    sc_core::sc_in<bool> rx_i{"rx_i"};

    sc_core::sc_out<bool> irq_o{"irq_o"};

    scc::sc_out_opt<sc_dt::sc_bv<2>> events_o{"events_o"};

    uart(sc_core::sc_module_name nm);

    virtual ~uart() override;

    void set_clock_period(sc_core::sc_time period) { clk_period = period; }

    cci::cci_param<unsigned> fifo_depth{"fifo_depth", 32};

    cci::cci_param<bool> bit_true_transfer{"bit_true_transfer", false};

protected:
    void reset_cb();
    void transmit_data();
    void update_irq();
    sc_core::sc_time clk_period;
    std::unique_ptr<uart_regs> regs;
    sc_core::sc_fifo<uint8_t> rx_fifo{"rx_fifo", 8};
    sc_core::sc_fifo<uint64_t> tx_fifo{"tx_fifo", 8};
    std::vector<std::vector<uint8_t>> queues;
};

using uart_tc = scc::ticking_clock<uart>;
using uart_tl = scc::tickless_clock<uart>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _MINRES_UART_H_ */
