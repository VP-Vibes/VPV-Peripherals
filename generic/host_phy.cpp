#include "host_phy.h"
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

namespace vpvper {
namespace generic {
namespace {
const std::array<unsigned char, 6> broadcast_mac{0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
}
namespace {
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

host_phy::host_phy(sc_core::sc_module_name nm)
: sc_core::sc_module(nm) {
    rx.register_b_transport([this](eth::eth_packet_payload& ethp, sc_core::sc_time& t) {
        ethp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
        if(sock < 0 || ifindex == 0) {
            SCCWARN(SCMOD) << "dropping outgoing Ethernet frame because no host raw socket is available";
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
        to.sll_ifindex = ifindex;
        to.sll_halen = ETH_ALEN;
        memcpy(to.sll_addr, eth->h_dest, 6);
        SCCDEBUG(SCMOD) << "Sending frame of lenght " << frame.size() << " from " << mac2str(eth->h_source) << " to mac addr "
                        << mac2str(eth->h_dest);
        ssize_t sent = sendto(sock, frame.data(), frame.size(), 0, (struct sockaddr*)&to, sizeof(to));
        if(sent < 0) {
            SCCWARN(SCMOD) << "failed to send Ethernet frame on interface '" << if_name.get_value() << "': " << std::strerror(errno);
            return;
        }
        if(static_cast<std::size_t>(sent) != frame.size()) {
            SCCWARN(SCMOD) << "short Ethernet frame write on interface '" << if_name.get_value() << "': " << sent << "/" << frame.size()
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
            auto len = recvfrom(sock, buf.data(), buf.size(), 0, (struct sockaddr*)&addr, &addr_len);
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
            if(memcmp(eth->h_dest, model_mac, 6) == 0 || memcmp(eth->h_dest, broadcast_mac.data(), 6) == 0) {
                SCCDEBUG(SCMOD) << "received frame of lenght " << len << " from mac addr " << mac2str(eth->h_source) << " with proto "
                                << std::hex << eth->h_proto;
                memcpy(eth->h_dest, model_mac, 6);
                std::vector<uint8_t> frame{buf.begin(), buf.begin() + len};
                que.push(frame);
            }
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