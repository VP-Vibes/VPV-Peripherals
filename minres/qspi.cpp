/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "qspi.h"
#include "gen/apb3spi_regs.h"
#include <limits>
#include <scc/report.h>
#include <scc/utilities.h>
#include <tlm/scc/tlm_extensions.h>
#include <vector>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace sc_dt;

qspi::qspi(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, apb3spi_regs) {
    regs->registerResources(*this);
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();

    spi_i.register_nb_transport_bw(
        [this](spi::spi_packet_payload& trans, tlm::tlm_phase& ph, sc_core::sc_time& t) -> tlm::tlm_sync_enum {
            if(ph == tlm::nw::CONFIRM) {
                switch(trans.get_data().size()) {
                case 1:
                    rsp.push_back(*trans.get_data().data());
                    break;
                case 2:
                case 4:
                default:
                    SCCWARN(SCMOD) << "Unsupported data width: " << trans.get_data().size();
                }
                ph = tlm::nw::RESPONSE;
            }
            return tlm::TLM_ACCEPTED;
        });
    xip_sck.register_b_transport([this](tlm::tlm_generic_payload& gp, sc_core::sc_time& t) {
        spi::spi_packet_payload spi_gp;
        tlm::scc::tlm_payload_extension gp_ext;
        gp_ext.gp = &gp;
        spi_gp.set_extension(&gp_ext);
        spi_i->b_transport(spi_gp, t);
        spi_gp.set_extension<tlm::scc::tlm_payload_extension>(nullptr);
    });
    xip_sck.register_transport_dbg([this](tlm::tlm_generic_payload& gp) {
        spi::spi_packet_payload spi_gp;
        tlm::scc::tlm_payload_extension gp_ext;
        gp_ext.gp = &gp;
        spi_gp.set_extension(&gp_ext);
        auto ret = spi_i->transport_dbg(spi_gp);
        spi_gp.set_extension<tlm::scc::tlm_payload_extension>(nullptr);
        return ret;
    });
    SC_THREAD(peq_cb);
    sensitive << cmd.event();

    regs->status.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            data = regs->r_status = (rsp.size() & 0xff) << 16 | 32 - cmd.size();
            SCCDEBUG(SCMOD) << "read status 0x" << std::hex << data;
        }
        return true;
    });
    regs->data.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            regs->r_data |= 0x80000000;
            if(rsp.size()) {
                regs->r_data = rsp.front();
                rsp.pop_front();
            }
            data = regs->r_data;
            SCCDEBUG(SCMOD) << "read data 0x" << std::hex << data;
        }
        return true;
    });
    regs->data.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
            if(!this->regs->in_reset()) {
                SCCDEBUG(SCMOD) << "write data 0x" << std::hex << data;
                switch(data & 0xf00) {
                case 0x100: // write cmd
                    cmd.notify(data & 0xff);
                    break;
                case 0x200: // read cmd
                    regs->r_status = (rsp.size() & 0xff) << 16 | (regs->r_status & 0xffff);
                    break;
                case 0x800: // ssgen cmd
                    if(data & 0x80) {
                        sel_slv_id = data & 0x7f;
                    } else {
                        sel_slv_id = std::numeric_limits<unsigned>::max();
                    }
                    break;
                }
            }
            return true;
        });
    regs->xip_write.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
            if(!this->regs->in_reset()) {
                SCCDEBUG(SCMOD) << "write xip_write 0x" << std::hex << data;
            }
            return true;
        });
    regs->xip_read_write.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
            if(!this->regs->in_reset()) {
                SCCDEBUG(SCMOD) << "write xip_read_write 0x" << std::hex << data;
            }
            return true;
        });
    regs->xip_read.set_write_cb(
        [this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
            if(!this->regs->in_reset()) {
                SCCDEBUG(SCMOD) << "write xip_read 0x" << std::hex << data;
            }
            return true;
        });
}

qspi::~qspi() = default;

void qspi::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

void qspi::peq_cb() {
    wait(sc_core::SC_ZERO_TIME);
    while(true) {
        auto e = cmd.get();
        auto payload = spi::spi_pkt_mm::get().allocate();
        payload->set_target_id(sel_slv_id);
        payload->get_data().push_back(static_cast<uint8_t>(e));
        tlm::tlm_phase ph{tlm::nw::REQUEST};
        sc_core::sc_time t;
        spi_i->nb_transport_fw(*payload, ph, t);
        regs->r_status = (regs->r_status & 0xffff0000) | (cmd.size() & 0xff) << 16;
    }
}

} /* namespace minres */
} /* namespace vpvper */
