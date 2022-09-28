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
namespace minres {

using namespace sc_core;

SC_HAS_PROCESS(uart_terminal);// NOLINT

uart_terminal::uart_terminal(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, uart_regs) {
    regs->registerResources(*this);
    //    SC_METHOD(clock_cb);
    //    sensitive << clk_i.pos();
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    SC_THREAD(transmit_data);
    regs->rxtxdata.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        if (!this->regs->in_reset()) {
            reg.put(data);
            tx_fifo.nb_write(static_cast<uint8_t>(regs->r_rxtxdata.data));
            update_irq();
        }
        return true;
    });
    regs->rxtxdata.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time d) -> bool {
        if (!this->regs->in_reset()) {
            uint8_t val;
            if (rx_fifo.nb_read(val)) {
                regs->r_rxtxdata.data = val;
                regs->r_rxtxdata.rx_avail=1;
                update_irq();
            } else
                regs->r_rxtxdata.rx_avail=0;
            data = reg.get() & reg.rdmask;
        }
        return true;
    });
    regs->irq_ctrl.set_write_cb([this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        update_irq();
        return true;
    });
}

uart_terminal::~uart_terminal() = default;

void uart_terminal::update_irq() {
    regs->r_irq_ctrl.num_tx_avail = tx_fifo.num_free();
    regs->r_irq_ctrl.num_rx_avail = rx_fifo.num_free();
    regs->r_irq_ctrl.tx_ip = regs->r_irq_ctrl.rx_ie && tx_fifo.num_available() == 0;
    regs->r_irq_ctrl.rx_ip = regs->r_irq_ctrl.tx_ie && rx_fifo.num_available() != 0;
    irq_o = regs->r_irq_ctrl.tx_ip  || regs->r_irq_ctrl.rx_ip;
}

void uart_terminal::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
        tx_o.write(true);
    } else
        regs->reset_stop();
}

void uart_terminal::transmit_data() {
    uint8_t txdata;
    sc_core::sc_time bit_duration(SC_ZERO_TIME);
    while (true) {
        if(bit_true_transfer.get_value())
            tx_o.write(true);
        wait(tx_fifo.data_written_event());
        while (tx_fifo.nb_read(txdata)) {
            update_irq();
            bit_duration = (regs->r_clock_config.clockDivider + 1) * clk_period;
            if(bit_true_transfer.get_value()) {
                tx_o.write(true); // start bit
                wait(bit_duration);
                for (int i = 8; i > 0; --i) {
                    tx_o.write(txdata & (1 << (i - 1))); // 8 data bits, MSB first
                    wait(bit_duration);
                }
                switch(regs->r_frame.parity){
                case 1: //even
                    tx_o.write(util::hasOddParity(txdata)); // parity bit
                    wait(bit_duration);
                    break;
                case 2: // odd
                    tx_o.write(!util::hasOddParity(txdata)); // parity bit
                    wait(bit_duration);
                    break;
                }
                if (regs->r_frame.stop_bits) {
                    tx_o.write(true); // stop bit 1
                    wait(bit_duration);
                }
                tx_o.write(true); // stop bit 1/2
                wait(bit_duration);
            }
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
