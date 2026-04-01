/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:41 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#pragma once

#include <cci_configuration>
#include <eth/eth_tlm.h>
#include <scc/async_queue.h>
#include <tlm/nw/initiator_mixin.h>
#include <tlm/nw/target_mixin.h>

namespace vpvper {
namespace generic {

/**
 * @brief Bridges Ethernet TLM packet sockets to a Linux host raw Ethernet interface.
 *
 * Frames received on @ref rx are transmitted through an `AF_PACKET` raw socket to
 * the configured Linux network interface. Frames captured from the Linux host
 * interface are forwarded into the virtual platform through @ref tx.
 *
 * The module is intentionally transport-oriented: it passes complete Ethernet
 * frames unchanged between the virtual platform and the host NIC.
 */
class host_phy : public sc_core::sc_module {
public:
    /**
     * @brief Socket emitting Ethernet frames received from the host NIC into the VP.
     */
    tlm::nw::initiator_mixin<eth::eth_pkt_initiator_socket<>> tx{"tx"};
    /**
     * @brief Socket receiving Ethernet frames from the VP for transmission to the host NIC.
     */
    tlm::nw::target_mixin<eth::eth_pkt_target_socket<>> rx{"rx"};
    /**
     * @brief Linux host interface name used for raw-socket binding.
     */
    cci::cci_param<std::string> if_name{"if_name", "eth0", "eth interface to use"};
    /**
     * @brief Enables promiscuous reception on the host interface when supported.
     */
    cci::cci_param<bool> promiscuous{"promiscuous", false, "use promisuous mode"};
    /**
     * @brief Construct a host Ethernet bridge.
     *
     * @param nm SystemC module name.
     */
    host_phy(sc_core::sc_module_name nm);
    /**
     * @brief Destroy the bridge and release any host-side raw-socket resources.
     */
    virtual ~host_phy();

protected:
    /**
     * @brief Opens the raw socket if an interface has been configured.
     */
    void start_of_simulation() override;
    /**
     * @brief Polls the async_queue and forwards received frames through @ref tx.
     */
    void rx_thread();

    int sock{-1};
    int ifindex{0};
    scc::async_queue<std::vector<uint8_t>> que;
};
} // namespace generic
} // namespace vpvper