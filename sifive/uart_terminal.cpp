/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "uart_terminal.h"
#include "gen/uart_regs.h"

#include <scc/report.h>
#include <scc/utilities.h>

using namespace std;

namespace vpvper {
namespace sifive {

using namespace sc_core;

SC_HAS_PROCESS(uart_terminal);// NOLINT

uart_terminal::uart_terminal(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, NAMEDD(regs, uart_regs) {
    regs->registerResources(*this);
//    SC_METHOD(clock_cb);
//    sensitive << clk_i.pos();
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    SC_THREAD(transmit_data);
    regs->txdata.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time& d) -> bool {
        if (!this->regs->in_reset()) {
            if(d.value()) {
                wait(d);
                d=SC_ZERO_TIME;
            }
            reg.put(data);
            tx_fifo.nb_write(static_cast<uint8_t>(regs->r_txdata.data));
            regs->r_txdata.full = tx_fifo.num_free() == 0;
            regs->r_ip.txwm = regs->r_txctrl.txcnt <= (7 - tx_fifo.num_free()) ? 1 : 0;
            update_irq();
        }
        return true;
    });
    regs->rxdata.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time& d) -> bool {
        if (!this->regs->in_reset()) {
            if(d.value()) {
                wait(d);
                d=SC_ZERO_TIME;
            }
            uint8_t val;
            if (rx_fifo.nb_read(val)) {
                regs->r_rxdata.data = val;
                if (regs->r_rxctrl.rxcnt <= rx_fifo.num_available()) {
                    regs->r_ip.rxwm = 1;
                    update_irq();
                }
            }
            data = reg.get() & reg.rdmask;
        }
        return true;
    });
    regs->ie.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        update_irq();
        return true;
    });
    regs->ip.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        update_irq();
        return true;
    });
}

uart_terminal::~uart_terminal() = default;

void uart_terminal::update_irq() {
    irq_o = (regs->r_ip.rxwm == 1 && regs->r_ie.rxwm == 1) || (regs->r_ip.txwm == 1 && regs->r_ie.txwm == 1);
}

void uart_terminal::reset_cb() {
    if (rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

void uart_terminal::transmit_data() {
    uint8_t txdata;
    sc_core::sc_time bit_duration(SC_ZERO_TIME);
    while (true) {
#ifdef BITTRUE
        tx_o.write(true);
#endif
        wait(tx_fifo.data_written_event());
        while (tx_fifo.nb_read(txdata)) {
            regs->r_txdata.full = tx_fifo.num_free() == 0;
            regs->r_ip.txwm = regs->r_txctrl.txcnt <= (7 - tx_fifo.num_free()) ? 1 : 0;
            bit_duration = (regs->r_div.div + 1) * clk;
#ifdef BITTRUE
            tx_o.write(true); // start bit
            wait(bit_duration);
            for (int i = 8; i > 0; --i) {
                tx_o.write(txdata & (1 << (i - 1))); // 8 data bits, MSB first
                wait(bit_duration);
            }
            if (regs->r_txctrl.nstop) {
                tx_o.write(true); // stop bit 1
                wait(bit_duration);
            }
            tx_o.write(true); // stop bit 1/2
            wait(bit_duration);
#endif
            if (txdata != '\r') queue.push_back(txdata);
            if (queue.size() && (txdata == '\n' || txdata == 0)) {
                std::string msg(queue.begin(), queue.end() - 1);
                SCCINFO(SCMOD) << "rx: '" << msg << "'";
                queue.clear();
            }
        }
    }
}

} /* namespace generic */
} /* namespace vpvper */
