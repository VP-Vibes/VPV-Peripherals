/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:41 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#pragma once

#include <cci/cfg/cci_param_typed.h>
#include <eth/eth_tlm.h>
#include <scc/async_queue.h>
#include <scc/clock_if_mixins.h>
#include <scc/tlm_target.h>
#include <sysc/kernel/sc_event.h>
#include <sysc/utils/sc_vector.h>
#include <tlm/nw/initiator_mixin.h>
#include <tlm/nw/target_mixin.h>
#include <util/ring_buffer.h>

namespace vpvper {
namespace generic {

class host_phy : public sc_core::sc_module {
public:
    tlm::nw::initiator_mixin<eth::eth_pkt_initiator_socket<>> eth_tx{"eth_tx"};

    tlm::nw::target_mixin<eth::eth_pkt_target_socket<>> eth_rx{"eth_rx"};

    cci::cci_param<uint8_t> interface_speed{"speed_class", 3, "the transfer speed of the ethmac: 1=10MBit/s, 2=100MBit/s, 3=1GBit/s"};

    cci::cci_param<std::string> if_name{"if_name", "eth0", "eth interface to use"};

    cci::cci_param<bool> promisc_mode{"promisc_mode", false, "use promisuous mode"};

    host_phy(sc_core::sc_module_name nm);

    virtual ~host_phy() override;

protected:
    void start_of_simulation() override;
    void receive();
    int sock{0};
    int ifindex{0};
    scc::async_queue<std::vector<uint8_t>> que;
};
} // namespace generic
} // namespace vpvper