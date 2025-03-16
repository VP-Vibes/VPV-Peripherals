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

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace sc_dt;

qspi::qspi(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
, NAMEDD(regs, apb3spi_regs) {
    xip_sck(flash_mem.target);
    regs->registerResources(*this);
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();

    SC_THREAD(peq_cb);
    sensitive<< cmd.event();

    regs->status.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        regs->r_status.tx_free = 31;
        data = regs->r_status;
        return true;
    });
    regs->data.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            SCCDEBUG(SCMOD)<<"read data 0x"<<std::hex<<data;
        }
        return true;
    });
    regs->data.set_write_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            SCCDEBUG(SCMOD)<<"write data 0x"<<std::hex<<data;
            switch(data&0xf00){
            case 0x100: // write cmd
            case 0x200: // read cmd
                cmd.notify(data&0x3ff);
                break;
            case 0x800: // ssgen cmd
                if(data&0x80) {
                    sel_slv_id = data & 0x7f;
                } else {
                    sel_slv_id = std::numeric_limits<unsigned>::max();
                }
                break;
            }
        }
        return true;
    });
    regs->xip_write.set_write_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            SCCDEBUG(SCMOD)<<"write xip_write 0x"<<std::hex<<data;
        }
        return true;
    });
    regs->xip_read_write.set_write_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            SCCDEBUG(SCMOD)<<"write xip_read_write 0x"<<std::hex<<data;
        }
        return true;
    });
    regs->xip_read.set_write_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            SCCDEBUG(SCMOD)<<"write xip_read 0x"<<std::hex<<data;
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
    while(true){
        auto e = cmd.get();
        spi::spi_packet_payload payload(sel_slv_id);
        //payload.set_data()
    }
}

} /* namespace minres */
} /* namespace vpvper */
