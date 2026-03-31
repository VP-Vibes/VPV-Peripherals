#include "host_phy.h"
#include "eth/eth_tlm.h"
#include "scc/report.h"
#include <arpa/inet.h>
#include <cerrno>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sysc/kernel/sc_module.h>
#include <sysc/kernel/sc_time.h>
#include <thread>
#include <tlm_core/tlm_2/tlm_generic_payload/tlm_gp.h>
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
    eth_rx.register_b_transport([this](eth::eth_packet_payload& trans, sc_core::sc_time& t) {
        auto& frame = trans.get_data();
        if(sock) {
            struct ethhdr* eth = reinterpret_cast<struct ethhdr*>(frame.data());
            struct sockaddr_ll to = {0};
            to.sll_family = AF_PACKET;
            to.sll_ifindex = ifindex;
            to.sll_halen = ETH_ALEN;
            memcpy(to.sll_addr, eth->h_dest, 6);
            ssize_t sent = sendto(sock, frame.data(), frame.size(), 0, (struct sockaddr*)&to, sizeof(to));
        }
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    });
    SC_THREAD(receive);
}

host_phy::~host_phy() {
    if(promisc_mode.get_value())
        set_promisc(sock, ifindex, 0);
    close(sock);
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
    promisc_mode.lock();
    if(promisc_mode.get_value()) {
        if(set_promisc(sock, ifindex, 1) < 0)
            SCCERR(SCMOD) << "Could not set promisc mode : " << strerror(errno);
    }
    std::thread rec_thread{[this]() {
        // Receive loop
        SCCINFO(SCMOD) << "Start listening on " << if_name.get_value().c_str() << " ...";
        std::vector<uint8_t> buf(65536);
        while(1) {
            struct sockaddr_ll from = {0};
            socklen_t fromlen = sizeof(from);
            ssize_t n = recvfrom(sock, buf.data(), sizeof(buf), 0, (struct sockaddr*)&from, &fromlen);
            auto err_no = errno;
            if(n < 0) {
                if(err_no == EAGAIN || err_no == EWOULDBLOCK)
                    continue;
                SCCERR(SCMOD) << "Error in recvfrom : " << strerror(errno);
                break;
            }
            if((size_t)n < sizeof(struct ethhdr))
                continue;
            struct ethhdr* rx = (struct ethhdr*)buf.data();
            std::vector<uint8_t> frame{buf.begin(), buf.begin() + n};
            que.push(frame);
        }
    }};
    rec_thread.detach();
}

void host_phy::receive() {
    std::vector<uint8_t> frame_data;
    while(true) {
        wait(que.data_event());
        while(que.try_get(frame_data)) {
            eth::eth_packet_payload packet;
            packet.set_data(frame_data);
            sc_core::sc_time d;
            eth_tx->b_transport(packet, d);
        }
    }
}
} // namespace generic
} // namespace vpvper