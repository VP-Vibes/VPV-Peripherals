/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:47 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#include "ethmac.h"
#include "eth/eth_tlm.h"
#include "gen/ethmac_regs.h"
#include "scc/report.h"
#include "tlm/nw/tlm_network_sockets.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <fmt/format.h>
#include <iterator>
#include <memory>
#include <scc/utilities.h>
#include <stdexcept>
#include <sysc/kernel/sc_simcontext.h>
#include <tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h>

namespace vpvper {
namespace minres {
namespace {

void write_u16_le(std::FILE* f, std::uint16_t v) {
    const std::uint8_t b[2] = {static_cast<std::uint8_t>(v & 0xFF), static_cast<std::uint8_t>(v >> 8)};
    if(std::fwrite(b, 1, 2, f) != 2)
        throw std::runtime_error("pcap write failed");
}

void write_u32_le(std::FILE* f, std::uint32_t v) {
    const std::uint8_t b[4] = {static_cast<std::uint8_t>(v & 0xFF), static_cast<std::uint8_t>((v >> 8) & 0xFF),
                               static_cast<std::uint8_t>((v >> 16) & 0xFF), static_cast<std::uint8_t>(v >> 24)};
    if(std::fwrite(b, 1, 4, f) != 4)
        throw std::runtime_error("pcap write failed");
}

std::uint64_t now_us() { return sc_core::sc_time_stamp() / 1_us; }
} // namespace

class pcap_writer {
public:
    // linktype 1 = Ethernet (DLT_EN10MB)
    explicit pcap_writer(std::string const& path, std::uint32_t snaplen = 65535, std::uint32_t linktype = 1)
    : f_(std::fopen(path.c_str(), "wb"))
    , snaplen_(snaplen) {
        if(!f_)
            throw std::runtime_error("failed to open pcap file");
        // Global header (pcap, little-endian, microsecond timestamps)
        write_u32_le(f_, 0xa1b2c3d4); // magic
        write_u16_le(f_, 2);          // version_major
        write_u16_le(f_, 4);          // version_minor
        write_u32_le(f_, 0);          // thiszone
        write_u32_le(f_, 0);          // sigfigs
        write_u32_le(f_, snaplen_);   // snaplen
        write_u32_le(f_, linktype);   // network (DLT)
    }

    ~pcap_writer() {
        if(f_)
            std::fclose(f_);
    }

    pcap_writer(const pcap_writer&) = delete;

    pcap_writer& operator=(const pcap_writer&) = delete;

    void write_frame(nonstd::span<const std::uint8_t> frame, std::uint64_t ts_us = now_us()) {
        if(!f_)
            throw std::runtime_error("pcap not open");
        const std::uint32_t orig_len = static_cast<std::uint32_t>(frame.size());
        const std::uint32_t incl_len = (frame.size() > snaplen_) ? snaplen_ : static_cast<std::uint32_t>(frame.size());
        const std::uint32_t ts_sec = static_cast<std::uint32_t>(ts_us / 1'000'000ULL);
        const std::uint32_t ts_usec = static_cast<std::uint32_t>(ts_us % 1'000'000ULL);
        // Per-packet record header (16 bytes)
        write_u32_le(f_, ts_sec);
        write_u32_le(f_, ts_usec);
        write_u32_le(f_, incl_len);
        write_u32_le(f_, orig_len);
        // Packet bytes
        if(incl_len != 0) {
            if(std::fwrite(frame.data(), 1, incl_len, f_) != incl_len)
                throw std::runtime_error("pcap write failed");
        }
    }

private:
    std::FILE* f_ = nullptr;
    std::uint32_t snaplen_ = 65535;
};

#if SYSTEMC_VERSION < 20241015
SC_HAS_PROCESS(ethmac); // NOLINT
#endif

using namespace eth;

ethmac::ethmac(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::ethmac_regs) {
    regs->registerResources(*this);
    // eth rx socket callback
    eth_rx.register_b_transport([this](eth_packet_types::tlm_payload_type& ethp, sc_core::sc_time&) {
        if(rx_buffer.full())
            wait(rx_empty_evt);
        auto& rx_data = ethp.get_data();
        if(pcap)
            pcap->write_frame(nonstd::span<uint8_t>(rx_data.data(), rx_data.size()));
        ethp.set_response_status(tlm::TLM_OK_RESPONSE);
        if(!regs->r_mac_ctrl.rx_flush) {
            uint32_t size = rx_data.size() * 8;
            rx_buffer.push_back(size & 0xff);
            rx_buffer.push_back((size >> 8) & 0xff);
            rx_buffer.push_back((size >> 16) & 0xff);
            rx_buffer.push_back((size >> 24) & 0xff);
            for(auto c : rx_data)
                rx_buffer.push_back(c);
            regs->r_mac_ctrl.rx_pending = !rx_buffer.empty();
            update_irq();
        }
    });
    // callback functions to enable readonly/writeonly functionality
    regs->mac_tx.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time& t) -> bool {
        if(!regs->in_reset() && !regs->r_mac_ctrl.tx_flush) {
            if(tx_state == tx_states::LENGTH) {
                tx_expected_bytes = (v + 7) / 8;
                tx_state = tx_states::DATA;
            } else {
                for(auto i = 0u; i < 4; ++i) {
                    tx_buffer.push_back((v >> (i * 8)) & 0xff);
                }
                if(tx_buffer.size() >= tx_expected_bytes) {
                    SCCDEBUG(SCOBJ) << "Sending eth frame of length " << tx_expected_bytes;
                    eth_packet_types::tlm_payload_type ethp;
                    ethp.set_command(eth::ETH::FRAME);
                    auto& tx_data = ethp.get_data();
                    tx_data.resize(0);
                    tx_data.reserve(tx_expected_bytes);
                    while(!tx_buffer.empty()) {
                        if(tx_data.size() < tx_expected_bytes)
                            tx_data.push_back(tx_buffer.front());
                        tx_buffer.pop_front();
                    }
                    if(pcap)
                        pcap->write_frame(nonstd::span<uint8_t>(tx_data.data(), tx_expected_bytes));
                    sc_core::sc_time delay;
                    eth_tx->b_transport(ethp, delay);
                    tx_state = tx_states::LENGTH;
                }
            }
            regs->r_mac_tx_availability.words_avail = (tx_buffer.capacity() - std::max(tx_buffer.size(), 1ul)) / sizeof(uint32_t);
            regs->r_mac_ctrl.tx_ready = !tx_buffer.full();
            update_irq();
        }
        return true;
    });
    regs->mac_tx_availability.set_write_cb(
        [this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time& t) -> bool { return true; });
    regs->mac_rx.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time& t) -> bool { return true; });
    regs->mac_rx.set_read_cb([this](scc::sc_register<uint32_t> const&, uint32_t& v, sc_core::sc_time& t) -> bool {
        v = 0;
        if(!regs->in_reset() && !regs->r_mac_ctrl.rx_flush) {
            for(auto i = 0; i < 4 && !rx_buffer.empty(); ++i) {
                v |= rx_buffer.front() << (i * 8);
                rx_buffer.pop_front();
            }
            regs->r_mac_ctrl.rx_pending = !rx_buffer.empty();
            update_irq();
        }
        return true;
    });
    regs->mac_rx_stats.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time& t) -> bool { return true; });
    regs->mac_ctrl.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t const& v, sc_core::sc_time& t) -> bool {
        if(!regs->in_reset()) {
            regs->r_mac_ctrl = v;
            regs->r_mac_ctrl.tx_ready = !tx_buffer.full();
            regs->r_mac_ctrl.rx_pending = !rx_buffer.empty();
            if(regs->r_mac_ctrl.rx_flush && !rx_buffer.empty())
                rx_buffer.clear();
            if(regs->r_mac_ctrl.tx_flush && !tx_buffer.empty())
                tx_buffer.clear();
        }
        return true;
    });
    regs->mac_ctrl.set_read_cb([this](scc::sc_register<uint32_t> const&, uint32_t& v, sc_core::sc_time& t) -> bool {
        if(!regs->in_reset()) {
            regs->r_mac_ctrl.tx_ready = !tx_buffer.full();
            regs->r_mac_ctrl.rx_pending = !rx_buffer.empty();
            v = regs->r_mac_ctrl;
        }
        return true;
    });
    // methods & threads
    SC_METHOD(reset_cb);
    dont_initialize();
    sensitive << rst_i;
}

ethmac::~ethmac() = default; // NOLINT

void ethmac::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
        tx_state = tx_states::LENGTH;
        rx_buffer.clear();
        tx_buffer.clear();
    } else {
        tx_state = tx_states::LENGTH;
        rx_buffer.clear();
        tx_buffer.clear();
        regs->reset_stop();
    }
}

void ethmac::start_of_simulation() {
    if(generate_pcap_file.get_value())
        pcap = std::make_unique<pcap_writer>(fmt::format("capture_{}.pcap", basename()));
}

void ethmac::update_irq() {
    irq_o[0].write(regs->r_mac_intr.tx_free_intr_enable && regs->r_mac_ctrl.tx_ready ||
                   regs->r_mac_intr.rx_data_avail_intr_enable && regs->r_mac_ctrl.rx_pending);
}
} // namespace minres
} // namespace vpvper