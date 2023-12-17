/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SPI_H_
#define _SPI_H_

#include <sysc/utils/sc_vector.h>
#include <tlm/scc/tlm_signal.h>
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#include "gen/spi_regs.h"

namespace vpvper {
namespace sifive {

class spi : public sc_core::sc_module, public scc::tlm_target<> {
public:
    template <typename T> using tlm_in = tlm::scc::tlm_signal_opt_target_socket<T>;
    template <typename T> using tlm_out = tlm::scc::tlm_signal_opt_initiator_socket<T>;

    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    tlm::scc::tlm_signal_bool_opt_out sck_o{"sck_o"};
    tlm::scc::tlm_signal_bool_opt_out mosi_o{"mosi_o"};
    tlm::scc::tlm_signal_bool_opt_in miso_i{"miso_i"};
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_out> scs_o{"scs_o", 4};

    sc_core::sc_out<bool> irq_o{"irq_o"};

    cci::cci_param<bool> bit_true_transfer{"bit_true_transfer", false};

    spi(sc_core::sc_module_name nm);

    ~spi() override = default;

    spi(spi &other) = delete;

    spi(spi &&other) = delete;

    spi &operator=(spi &other) = delete;

    spi &operator=(spi &&other) = delete;

private:
    void clock_cb();
    void reset_cb();
    void transmit_data();
    void receive_data(tlm::scc::tlm_signal_gp<> &gp, sc_core::sc_time &delay);
    void update_irq();
    sc_core::sc_event update_irq_evt;
    sc_core::sc_time clk;
    std::unique_ptr<spi_regs> regs;
    sc_core::sc_fifo<uint8_t> rx_fifo, tx_fifo;

};


} /* namespace sifive */
} /* namespace vpvper */

#endif /* _SPI_H_ */
