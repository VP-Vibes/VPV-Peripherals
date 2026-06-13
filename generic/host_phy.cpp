#include "host_phy.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <fmt/format.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <scc/report.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace vpvper {
namespace generic {
namespace {

const std::array<unsigned char, 6> broadcast_mac{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
constexpr std::size_t kEthernetHeaderLength = 14U;
constexpr std::uint16_t kEthertypeIpv4 = 0x0800U;
constexpr std::size_t kIpv4MinHeaderLength = 20U;
constexpr std::size_t kTcpMinHeaderLength = 20U;
constexpr std::size_t kUdpHeaderLength = 8U;
constexpr std::uint8_t kTcpProtocolNumber = 6U;
constexpr std::uint8_t kUdpProtocolNumber = 17U;
constexpr std::uint8_t kTcpChecksumOffset = 16U;
constexpr std::uint8_t kUdpLengthOffset = 4U;
constexpr std::uint8_t kUdpChecksumOffset = 6U;
constexpr std::uint16_t kIpv4FragmentOffsetMask = 0x1FFFU;
constexpr std::uint16_t kIpv4MoreFragmentsFlag = 0x2000U;

std::uint16_t read_u16_be(const std::uint8_t* data) {
    return static_cast<std::uint16_t>((static_cast<std::uint16_t>(data[0]) << 8) | data[1]);
}

std::uint32_t read_u32_be(const std::uint8_t* data) {
    return (static_cast<std::uint32_t>(data[0]) << 24) | (static_cast<std::uint32_t>(data[1]) << 16) |
           (static_cast<std::uint32_t>(data[2]) << 8) | data[3];
}

void write_u16_be(std::uint8_t* data, std::uint16_t value) {
    data[0] = static_cast<std::uint8_t>(value >> 8);
    data[1] = static_cast<std::uint8_t>(value & 0xFFU);
}

std::uint32_t checksum_add_bytes(std::uint32_t sum, const std::uint8_t* data, std::size_t length) {
    std::size_t index = 0;
    while(index + 1 < length) {
        sum += static_cast<std::uint32_t>((static_cast<std::uint32_t>(data[index]) << 8) | data[index + 1]);
        index += 2;
    }
    if(index < length) {
        sum += static_cast<std::uint32_t>(data[index]) << 8;
    }
    return sum;
}

std::uint16_t checksum_fold(std::uint32_t sum) {
    while((sum >> 16) != 0U) {
        sum = (sum & 0xFFFFU) + (sum >> 16);
    }
    return static_cast<std::uint16_t>(sum);
}

std::uint16_t checksum_finish(std::uint32_t sum) {
    sum = checksum_fold(sum);
    return static_cast<std::uint16_t>(~sum & 0xFFFFU);
}

std::uint16_t calculate_transport_pseudo_header_seed_ipv4(const std::uint8_t* ip_packet, std::size_t transport_length,
                                                          std::uint8_t protocol) {
    std::array<std::uint8_t, 4> pseudo_tail{};
    std::uint32_t sum = 0;
    sum = checksum_add_bytes(sum, ip_packet + 12, 8);
    pseudo_tail[0] = 0;
    pseudo_tail[1] = protocol;
    pseudo_tail[2] = static_cast<std::uint8_t>(transport_length >> 8);
    pseudo_tail[3] = static_cast<std::uint8_t>(transport_length & 0xFFU);
    sum = checksum_add_bytes(sum, pseudo_tail.data(), pseudo_tail.size());
    return checksum_fold(sum);
}

std::uint16_t calculate_transport_checksum_ipv4(const std::uint8_t* ip_packet, const std::uint8_t* transport_segment,
                                                std::size_t transport_length, std::uint8_t protocol) {
    std::array<std::uint8_t, 4> pseudo_tail{};
    std::uint32_t sum = 0;
    sum = checksum_add_bytes(sum, ip_packet + 12, 8);
    pseudo_tail[0] = 0;
    pseudo_tail[1] = protocol;
    pseudo_tail[2] = static_cast<std::uint8_t>(transport_length >> 8);
    pseudo_tail[3] = static_cast<std::uint8_t>(transport_length & 0xFFU);
    sum = checksum_add_bytes(sum, pseudo_tail.data(), pseudo_tail.size());
    sum = checksum_add_bytes(sum, transport_segment, transport_length);
    const std::uint16_t checksum = checksum_finish(sum);
    if(protocol == kUdpProtocolNumber && checksum == 0U) {
        return 0xFFFFU;
    }
    return checksum;
}

/*
 * Parse an Ethernet II frame, detect an IPv4/TCP or IPv4/UDP payload,
 * calculate the transport checksum, and replace the checksum field only when
 * it currently contains the IPv4 pseudo-header seed used by checksum offload.
 */
int fix_ipv4_checksums(std::vector<std::uint8_t>& frame) {
    if(frame.size() < kEthernetHeaderLength) { // frame is too short for Ethernet II
        return 1;
    }
    auto ethertype = read_u16_be(frame.data() + 12);
    if(ethertype != kEthertypeIpv4) { // unsupported EtherType
        return 2;
    }
    auto ethernet_payload_length = frame.size() - kEthernetHeaderLength;
    auto ip_packet = frame.data() + kEthernetHeaderLength;
    if(ethernet_payload_length < kIpv4MinHeaderLength) { // IPv4 header is truncated
        return 3;
    }
    if((ip_packet[0] >> 4) != 4U) { // payload is not IPv4
        return 4;
    }
    auto ip_header_length = static_cast<std::size_t>(ip_packet[0] & 0x0FU) * 4U;
    if(ip_header_length < kIpv4MinHeaderLength || ip_header_length > ethernet_payload_length) { // invalid IPv4 header length
        return 5;
    }
    auto ip_total_length = read_u16_be(ip_packet + 2);
    if(ip_total_length < ip_header_length || ip_total_length > ethernet_payload_length) { // invalid IPv4 total length
        return 6;
    }
    auto protocol = ip_packet[9];
    if(protocol != kTcpProtocolNumber && protocol != kUdpProtocolNumber) { // IPv4 payload is not TCP or UDP
        return 0;
    }
    auto flags_and_fragment_offset = read_u16_be(ip_packet + 6);
    if((flags_and_fragment_offset & kIpv4FragmentOffsetMask) != 0U ||
       (flags_and_fragment_offset & kIpv4MoreFragmentsFlag) != 0U) { // fragmented IPv4 packets must be reassembled
        return 0;
    }
    auto transport_segment = ip_packet + ip_header_length;
    auto transport_length = ip_total_length - ip_header_length;
    auto source_port = read_u16_be(transport_segment);
    auto destination_port = read_u16_be(transport_segment + 2);
    uint8_t checksum_offset = 0;
    std::uint32_t sequence_number = 0;
    uint16_t udp_length = 0;
    bool rewrite_checksum = true;
    if(protocol == kTcpProtocolNumber) {
        checksum_offset = kTcpChecksumOffset;
        if(transport_length < kTcpMinHeaderLength) { // TCP header is truncated
            return 7;
        }
        const std::size_t tcp_header_length = static_cast<std::size_t>(transport_segment[12] >> 4) * 4U;
        if(tcp_header_length < kTcpMinHeaderLength || tcp_header_length > transport_length) { // invalid TCP data offset
            return 8;
        }
        sequence_number = read_u32_be(transport_segment + 4);
    } else {
        checksum_offset = kUdpChecksumOffset;
        if(transport_length < kUdpHeaderLength) { // UDP header is truncated
            return 9;
        }
        udp_length = read_u16_be(transport_segment + kUdpLengthOffset);
        if(udp_length < kUdpHeaderLength || udp_length > transport_length) { // invalid UDP length
            return 10;
        }
        transport_length = udp_length;
        if(read_u16_be(transport_segment + kUdpChecksumOffset) == 0U) {
            rewrite_checksum = false;
        }
    }
    auto stored_checksum = read_u16_be(transport_segment + checksum_offset);
    auto pseudo_header_seed = calculate_transport_pseudo_header_seed_ipv4(ip_packet, transport_length, protocol);
    if(rewrite_checksum && stored_checksum == pseudo_header_seed) {
        write_u16_be(transport_segment + checksum_offset, 0);
        auto calculated_checksum = calculate_transport_checksum_ipv4(ip_packet, transport_segment, transport_length, protocol);
        auto checksum_updated = stored_checksum != calculated_checksum;
        write_u16_be(transport_segment + checksum_offset, checksum_updated ? calculated_checksum : stored_checksum);
    }
    return 0;
}

int ifindex_of(int sock, const char* ifname) {
    struct ifreq ifr = {0};
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    if(ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
        return -1;
    return ifr.ifr_ifindex;
}

int set_promisc(int sock, int ifindex, int enable) {
    struct packet_mreq mreq = {0};
    mreq.mr_ifindex = ifindex;
    mreq.mr_type = PACKET_MR_PROMISC;
    int opt = enable ? PACKET_ADD_MEMBERSHIP : PACKET_DROP_MEMBERSHIP;
    return setsockopt(sock, SOL_PACKET, opt, &mreq, sizeof(mreq));
}

std::string mac2str(unsigned char* mac) {
    return fmt::format("{:02x}:{:02x}:{:02x}:{:02x}:{:02x}:{:02x}", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}
} // namespace

host_phy::host_phy(sc_core::sc_module_name nm, std::string eth_if_name)
: sc_core::sc_module(nm)
, if_name{"if_name", eth_if_name, "eth interface to use"} {
    rx.register_b_transport([this](eth::eth_packet_payload& ethp, sc_core::sc_time& t) {
        ethp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        if(sock < 0 || ifindex == 0) {
            SCCWARN(SCMOD) << "Dropping outgoing Ethernet frame because no host raw socket is available";
            return;
        }
        auto& frame = ethp.get_data();
        struct ethhdr* eth = reinterpret_cast<struct ethhdr*>(frame.data());
        if(!model_mac_init) {
            memcpy(model_mac, eth->h_source, 6);
            model_mac_init = true;
        }
        // get destination MAC address and create socket addr, send frame to socket
        struct sockaddr_ll to = {0};
        to.sll_family = AF_PACKET;
        to.sll_protocol = htons(ETH_P_ALL);
        to.sll_ifindex = ifindex;
        to.sll_halen = ETH_ALEN;
        memcpy(to.sll_addr, eth->h_dest, 6);
        if(map_mac_addr.get_value())
            memcpy(eth->h_source, hw_mac, 6);

        SCCDEBUG(SCMOD) << "Sending frame of length " << frame.size() << " " << mac2str(eth->h_source) << " -> " << mac2str(eth->h_dest)
                        << " with EtherType=0x" << std::hex << ntohs(eth->h_proto);
        ssize_t sent = sendto(sock, frame.data(), frame.size(), 0, (struct sockaddr*)&to, sizeof(to));
        if(sent < 0) {
            SCCWARN(SCMOD) << "Failed to send Ethernet frame on interface '" << if_name.get_value() << "': " << std::strerror(errno);
            return;
        }
        if(static_cast<std::size_t>(sent) != frame.size()) {
            SCCWARN(SCMOD) << "Short Ethernet frame write on interface '" << if_name.get_value() << "': " << sent << "/" << frame.size()
                           << " bytes";
            return;
        }
        ethp.set_response_status(tlm::TLM_OK_RESPONSE);
    });
    SC_THREAD(rx_thread);
}

host_phy::~host_phy() {
    set_promisc(sock, ifindex, 0);
    close(sock);
    sock = -1;
    ifindex = 0;
}

void host_phy::start_of_simulation() {
    // create L2 packet socket: receives full Ethernet header + payload
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock < 0) {
        SCCFATAL(SCMOD) << "Could not open socket: " << strerror(errno);
        return;
    }
    // get my MAC address
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name.get_value().c_str(), IFNAMSIZ - 1);
    if(ioctl(sock, SIOCGIFHWADDR, &ifr) < 0) {
        SCCFATAL(SCMOD) << "Could get MAC addr of socket (SIOCGIFHWADDR): " << strerror(errno);
        close(sock);
        return;
    }
    memcpy(hw_mac, (unsigned char*)ifr.ifr_hwaddr.sa_data, 6);
    // get eth interface index
    ifindex = ifindex_of(sock, if_name.get_value().c_str());
    if(ifindex < 0) {
        SCCFATAL(SCMOD) << "Could get index of socket (SIOCGIFINDEX): " << strerror(errno);
        close(sock);
        return;
    }
    // bind the socket to the eth device
    struct sockaddr_ll sll = {0};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifindex;
    if(bind(sock, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
        SCCFATAL(SCMOD) << "Could bind to socket: " << strerror(errno);
        close(sock);
        return;
    }
    // set promiscuous mode so we get all packets
    if(set_promisc(sock, ifindex, 1) < 0) {
        SCCFATAL(SCMOD) << "Could not set promisc mode : " << strerror(errno);
        close(sock);
        return;
    }
    // start receiving thread filtering incoming packets
    std::thread rx_host_thread{[this]() {
        // Receive loop
        SCCINFO(SCMOD) << "Start listening on " << if_name.get_value().c_str() << " ...";
        std::vector<uint8_t> buf(65536);
        // buf.resize(65536);
        while(1) {
            struct sockaddr_ll addr = {0};
            socklen_t addr_len = sizeof(addr);
            auto len = recvfrom(sock, buf.data(), buf.size(), 0, nullptr, nullptr);
            auto err_no = errno;
            if(len < 0) {
                if(err_no == EAGAIN || err_no == EWOULDBLOCK)
                    continue;
                SCCERR(SCMOD) << "Error in recvfrom : " << strerror(errno);
                break;
            }
            if(len < sizeof(struct ethhdr) || addr.sll_pkttype == PACKET_OUTGOING)
                continue;
            struct ethhdr* eth = (struct ethhdr*)buf.data();
            SCCTRACE(SCMOD) << if_name.get_value() << " received packet " << mac2str(eth->h_source) << " -> " << mac2str(eth->h_dest)
                            << " with protocol " << ntohs(eth->h_proto);
            auto mac_addr = map_mac_addr.get_value() ? hw_mac : model_mac;
            if(memcmp(eth->h_dest, mac_addr, 6) && memcmp(eth->h_dest, broadcast_mac.data(), 6))
                // this is not our packet
                continue;
            SCCDEBUG(SCMOD) << "Received frame of length " << len << " " << mac2str(eth->h_source) << " -> " << mac2str(eth->h_dest)
                            << " with EtherType=0x" << std::hex << ntohs(eth->h_proto);
            if(map_mac_addr.get_value())
                memcpy(eth->h_dest, model_mac, 6);
            std::vector<uint8_t> frame{buf.begin(), buf.begin() + len};
            if(ipv4_checksum_fix.get_value())
                fix_ipv4_checksums(frame);
            que.push(frame);
        }
    }};
    rx_host_thread.detach();
}

void host_phy::rx_thread() {
    std::vector<uint8_t> frame_data;
    while(true) {
        wait(que.data_event());
        while(que.try_get(frame_data)) {
            eth::eth_packet_payload ethp;
            ethp.set_sender_clk_period(sc_core::SC_ZERO_TIME);
            ethp.set_data(std::move(frame_data));
            sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
            tx->b_transport(ethp, delay);
            if(delay != sc_core::SC_ZERO_TIME)
                wait(delay);
        }
    }
}
} // namespace generic
} // namespace vpvper