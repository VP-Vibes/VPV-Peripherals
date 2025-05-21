/*
 * spimem.cpp
 *
 *  Created on: Mar 2, 2025
 *      Author: eyck
 */

#include "spi_mem.h"
#include <tlm/scc/tlm_extensions.h>

namespace vpvper {
namespace generic {

spi_mem::spi_mem(sc_core::sc_module_name const& nm) {
    spi_t.register_nb_transport_fw(
        [this](spi::spi_packet_payload& trans, tlm::tlm_phase& ph, sc_core::sc_time& t) -> tlm::tlm_sync_enum {
            if(ph == tlm::nw::INDICATION) {
                t += 8 * trans.get_sender_clk_period();
                cmd.notify(*trans.get_data().data(), sc_core::SC_ZERO_TIME);
            }
            return tlm::TLM_ACCEPTED;
        });
    spi_t.register_b_transport([this](spi::spi_packet_payload& gp, sc_core::sc_time& t) {
        auto gp_ext = gp.get_extension<tlm::scc::tlm_payload_extension>();
        if(!gp_ext) {
            gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            return;
        }
        sckt->b_transport(*gp_ext->gp, t);
    });
    sckt(mem.target);
    SC_HAS_PROCESS(spi_mem);
    SC_THREAD(cmd_cb);
}

spi_mem::~spi_mem() = default;

void spi_mem::cmd_cb() {
    wait(sc_core::SC_ZERO_TIME);
    while(true) {
        auto e = cmd.get();
        switch(e) {
        case 0x90: { // S25FL128S identify
            std::vector<uint8_t> addr(3);
            for(auto& a : addr)
                a = cmd.get();
            auto gp0 = spi::spi_pkt_mm::get().allocate();
            gp0->get_data().push_back(0x01);
            tlm::tlm_phase ph = tlm::nw::RESPONSE;
            sc_core::sc_time t;
            spi_t->nb_transport_bw(*gp0, ph, t);
            auto gp1 = spi::spi_pkt_mm::get().allocate();
            gp1->get_data().push_back(0x18);
            ph = tlm::nw::RESPONSE;
            spi_t->nb_transport_bw(*gp1, ph, t);
            break;
        }
        case 0xf0: // S25FL128S reset
        case 0x66: // n25qxxx reset enable
        case 0x99: // n25qxxx reset memory
            break;
        case 0x06: // Write Enable (WREN)
            wren = true;
            break;
        case 0x04: // Write Disable (WRDI)
            wren = false;
            break;
        case 0x01: { //  Write Registers (WRR 01h), SR1, CR1
            auto sr1 = cmd.get();
            auto cr1 = cmd.get();
            break;
        }
        case 0x05: { // Read Status Register 1RDSR1
            auto gp0 = spi::spi_pkt_mm::get().allocate();
            gp0->get_data().push_back(wren ? 0x02 : 0x0);
            tlm::tlm_phase ph = tlm::nw::RESPONSE;
            sc_core::sc_time t;
            spi_t->nb_transport_bw(*gp0, ph, t);
            break;
        }
        default:
            SCCWARN(SCMOD) << "command not understood: 0x" << std::hex << e;
        }
    }
}

} /* namespace generic */
} // namespace vpvper
