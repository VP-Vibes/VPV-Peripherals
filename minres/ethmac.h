/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:41 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#pragma once

#include "scc/signal_opt_ports.h"
#include "util/ring_buffer.h"
#include <cci/cfg/cci_param_typed.h>
#include <eth/eth_tlm.h>
#include <limits>
#include <memory>
#include <scc/clock_if_mixins.h>
#include <scc/tlm_target.h>
#include <sysc/kernel/sc_event.h>
#include <sysc/utils/sc_vector.h>
#include <tlm/nw/initiator_mixin.h>
#include <tlm/nw/target_mixin.h>
#include <util/ring_buffer.h>

namespace vpvper {
namespace minres {

namespace gen {
class ethmac_regs;
}

struct pcap_writer;

class ethmac : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};

    tlm::nw::initiator_mixin<eth::eth_pkt_initiator_socket<>> eth_tx{"eth_tx"};

    tlm::nw::target_mixin<eth::eth_pkt_target_socket<>> eth_rx{"eth_rx"};

    sc_core::sc_vector<sc_core::sc_out<bool>> irq_o{"irq_o", 2};

    scc::sc_out_opt<sc_dt::sc_bv<2>> events_o{"events_o"};

    cci::cci_param<std::string> pcap_file_name{"pcap_file_name", "",
                                               "if set enbales the generation of a pcap file of sent frames with the given name"};

    ethmac(sc_core::sc_module_name nm);

    virtual ~ethmac() override;

protected:
    void set_clock_period(sc_core::sc_time period) { clk_period = period; }

    sc_core::sc_time clk_period;

    void start_of_simulation() override;

    void reset_cb();

    void update_irq();

    sc_core::sc_time clk;
    sc_core::sc_event rx_empty_evt;
    std::unique_ptr<gen::ethmac_regs> regs;
    util::ring_buffer<uint8_t> tx_buffer{4096}, rx_buffer{8192};
    enum class tx_states { LENGTH, DATA };
    tx_states tx_state{tx_states::LENGTH};
    size_t tx_expected_bytes{std::numeric_limits<size_t>::max()};
    std::unique_ptr<pcap_writer> pcap;
};

using ethmac_tc = scc::ticking_clock<ethmac>;
using ethmac_tl = scc::tickless_clock<ethmac>;

} // namespace minres
} // namespace vpvper