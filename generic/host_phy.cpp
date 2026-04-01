#include "host_phy.h"
#include <cerrno>
#include <cstdint>
#include <cstring>
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
        struct sockaddr_ll to = {0};
        to.sll_family = AF_PACKET;
        to.sll_ifindex = ifindex;
        to.sll_halen = ETH_ALEN;
        memcpy(to.sll_addr, eth->h_dest, 6);
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
    if(promiscuous.get_value())
        set_promisc(sock, ifindex, 0);
    close(sock);
    sock = -1;
    ifindex = 0;
}

void host_phy::start_of_simulation() {
    // L2 packet socket: receives full Ethernet header + payload
    sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock < 0) {
        SCCFATAL(SCMOD) << "Could not open socket: " << strerror(errno);
    }
    ifindex = ifindex_of(sock, if_name.get_value().c_str());
    if(ifindex < 0) {
        SCCFATAL(SCMOD) << "Could get index of socket (SIOCGIFINDEX): " << strerror(errno);
    }
    struct sockaddr_ll sll = {0};
    sll.sll_family = AF_PACKET;
    sll.sll_protocol = htons(ETH_P_ALL);
    sll.sll_ifindex = ifindex;
    if(bind(sock, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
        SCCFATAL(SCMOD) << "Could bind to socket: " << strerror(errno);
    }
    promiscuous.lock();
    if(promiscuous.get_value()) {
        if(set_promisc(sock, ifindex, 1) < 0)
            SCCERR(SCMOD) << "Could not set promisc mode : " << strerror(errno);
    }
    std::thread rx_host_thread{[this]() {
        // Receive loop
        SCCINFO(SCMOD) << "Start listening on " << if_name.get_value().c_str() << " ...";
        std::vector<uint8_t> buf(65536);
        while(1) {
            struct sockaddr_ll addr = {0};
            socklen_t addr_len = sizeof(addr);
            auto len = recvfrom(sock, buf.data(), sizeof(buf), 0, (struct sockaddr*)&addr, &addr_len);
            auto err_no = errno;
            if(len < 0) {
                if(err_no == EAGAIN || err_no == EWOULDBLOCK)
                    continue;
                SCCERR(SCMOD) << "Error in recvfrom : " << strerror(errno);
                break;
            }
            if(len < sizeof(struct ethhdr) || addr.sll_pkttype == PACKET_OUTGOING)
                continue;
            struct ethhdr* rx = (struct ethhdr*)buf.data();
            std::vector<uint8_t> frame{buf.begin(), buf.begin() + len};
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