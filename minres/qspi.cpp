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
    regs->status.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        regs->r_status.tx_free = 32;
        return false;
    });
    regs->data.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        return false;
    });
    regs->data.set_write_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t const& data, sc_core::sc_time d) -> bool {
        return false;
    });
    // TODO write to irq
}

qspi::~qspi() = default;

void qspi::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}
} /* namespace minres */
} /* namespace vpvper */
