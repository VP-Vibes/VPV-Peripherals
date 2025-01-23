/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "uart.h"
#include "gen/uart_regs.h"

#include <scc/report.h>
#include <scc/utilities.h>

using namespace std;

namespace vpvper {
namespace minres {

using namespace sc_core;

SC_HAS_PROCESS(uart); // NOLINT

uart::uart(sc_core::sc_module_name nm)
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
    regs->rx_tx_reg.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            reg.put(data);
            tx_fifo.nb_write(static_cast<uint8_t>(regs->r_rx_tx_reg.data));
            update_irq();
        }
        return true;
    });
    regs->rx_tx_reg.set_read_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
            if(!this->regs->in_reset()) {
                uint8_t val;
                if(rx_fifo.nb_read(val)) {
                    regs->r_rx_tx_reg.data = val;
                    regs->r_rx_tx_reg.rx_avail = 1;
                    update_irq();
                } else
                    regs->r_rx_tx_reg.rx_avail = 0;
                regs->r_rx_tx_reg.tx_free = tx_fifo.num_free() != 0;
                data = reg.get() & reg.rdmask;
            }
            return true;
        });
    regs->int_ctrl_reg.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        update_irq();
        return true;
    });
}

uart::~uart() = default;

void uart::update_irq() {
    regs->r_int_ctrl_reg.write_intr_pend = regs->r_int_ctrl_reg.read_intr_enable && tx_fifo.num_available() == 0;
    regs->r_int_ctrl_reg.read_intr_pend = regs->r_int_ctrl_reg.write_intr_enable && rx_fifo.num_available() != 0;
    irq_o = regs->r_int_ctrl_reg.write_intr_pend || regs->r_int_ctrl_reg.read_intr_pend;
}

void uart::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
        tx_o.write(true);
    } else
        regs->reset_stop();
}

void uart::transmit_data() {
    uint8_t txdata;
    sc_core::sc_time bit_duration(SC_ZERO_TIME);
    while(true) {
        if(bit_true_transfer.get_value())
            tx_o.write(true);
        wait(tx_fifo.data_written_event());
        while(tx_fifo.nb_read(txdata)) {
            update_irq();
            bit_duration = (regs->r_clk_divider_reg.clock_divider + 1) * clk_period;
            if(bit_true_transfer.get_value()) {
                tx_o.write(true); // start bit
                wait(bit_duration);
                for(int i = 8; i > 0; --i) {
                    tx_o.write(txdata & (1 << (i - 1))); // 8 data bits, MSB first
                    wait(bit_duration);
                }
                switch(regs->r_frame_config_reg.parity) {
                case 1:                                     // even
                    tx_o.write(util::hasOddParity(txdata)); // parity bit
                    wait(bit_duration);
                    break;
                case 2:                                      // odd
                    tx_o.write(!util::hasOddParity(txdata)); // parity bit
                    wait(bit_duration);
                    break;
                }
                if(regs->r_frame_config_reg.stop_bit) {
                    tx_o.write(true); // stop bit 1
                    wait(bit_duration);
                }
                tx_o.write(true); // stop bit 1/2
                wait(bit_duration);
            }
            if(txdata != '\r')
                queue.push_back(txdata);
            if(queue.size() && (txdata == '\n' || txdata == 0)) {
                std::string msg(queue.begin(), queue.end() - 1);
                SCCINFO(SCMOD) << "rx: '" << msg << "'";
                queue.clear();
            }
        }
    }
}

} // namespace minres
} /* namespace vpvper */
