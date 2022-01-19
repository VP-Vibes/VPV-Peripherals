/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio.h"
#include "gen/gpio_regs.h"
#include <scc/report.h>
#include <scc/utilities.h>
#if defined(HAS_WEB_SOCKETS)
#include <generic/sc_comm_singleton.h>
#endif
#include <limits>

namespace vpvper {
namespace sifive {
using namespace sc_core;
using namespace sc_dt;

gpio::gpio(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, NAMED(clk_i)
, NAMED(rst_i)
, NAMED(pins_o, 32)
, NAMED(pins_i, 32)
, NAMED(iof0_o, 32)
, NAMED(iof1_o, 32)
, NAMED(iof0_i, 32)
, NAMED(iof1_i, 32)
, NAMEDD(regs, gpio_regs)
, NAMED(write_to_ws, false) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    auto pins_i_cb = [this](unsigned int tag, tlm::scc::tlm_signal_gp<sc_logic> &gp, tlm::tlm_phase &phase,
                            sc_core::sc_time &delay) -> tlm::tlm_sync_enum {
        this->pin_input(tag, gp, delay);
        return tlm::TLM_COMPLETED;
    };
    auto i = 0U;
    for (auto &s : pins_i) {
        s.register_nb_transport(pins_i_cb, i);
        ++i;
    }
    auto iof0_i_cb = [this](unsigned int tag, tlm::scc::tlm_signal_gp<bool> &gp, tlm::tlm_phase &phase,
                            sc_core::sc_time &delay) -> tlm::tlm_sync_enum {
        last_iof0[tag] = gp.get_value();
        this->iof_input(tag, 0, gp, delay);
        return tlm::TLM_COMPLETED;
    };
    i = 0;
    for (auto &s : iof0_i) {
        s.register_nb_transport(iof0_i_cb, i);
        ++i;
    }
    auto iof1_i_cb = [this](unsigned int tag, tlm::scc::tlm_signal_gp<bool> &gp, tlm::tlm_phase &phase,
                            sc_core::sc_time &delay) -> tlm::tlm_sync_enum {
        last_iof1[tag] = gp.get_value();
        this->iof_input(tag, 1, gp, delay);
        return tlm::TLM_COMPLETED;
    };
    i = 0;
    for (auto &s : iof1_i) {
        s.register_nb_transport(iof1_i_cb, i);
        ++i;
    }
    auto update_pins_cb = [this](scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time d) -> bool {
        if (!this->regs->in_reset()) {
            auto changed_bits = (reg.get() ^ data);
            reg.put(data);
            update_pins(changed_bits);
        }
        return true;
    };
    regs->port.set_write_cb(update_pins_cb);
    regs->output_en.set_write_cb(update_pins_cb);
    regs->out_xor.set_write_cb(update_pins_cb);
    regs->iof_en.set_write_cb(update_pins_cb);
    regs->iof_sel.set_write_cb(update_pins_cb);
}

gpio::~gpio() = default;

void gpio::before_end_of_elaboration() {
#if defined(HAS_WEB_SOCKETS)
    if (write_to_ws.get_value()) {
        auto end_point = std::string{"/ws/"} + name();
        SCCTRACE() << "Adding WS handler for " << end_point;
        handler = std::make_shared<vpvper::generic::WsHandler>();
        vpvper::generic::sc_comm_singleton::inst().registerWebSocketHandler(end_point.c_str(), handler);
    }
#endif
}

void gpio::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
    update_pins(std::numeric_limits<uint32_t>::max());
}

void gpio::clock_cb() { this->clk = clk_i.read(); }

tlm::tlm_phase gpio::write_output(tlm::scc::tlm_signal_gp<sc_dt::sc_logic> &gp, size_t i, sc_dt::sc_logic val) {
    sc_core::sc_time delay{SC_ZERO_TIME};
    tlm::tlm_phase phase{tlm::BEGIN_REQ};
    gp.set_command(tlm::TLM_WRITE_COMMAND);
    gp.set_response_status(tlm::TLM_OK_RESPONSE);
    gp.set_value(val);
    pins_o.at(i)->nb_transport_fw(gp, phase, delay);
    return phase;
}

void gpio::update_pins(uint32_t changed_bits) {
    sc_core::sc_inout_rv<32>::data_type out_val;
    tlm::scc::tlm_signal_gp<sc_dt::sc_logic> gp;
    sc_logic val;
    for (size_t i = 0, mask = 1; i < 32; ++i, mask <<= 1) {
        if (changed_bits & mask) {
            if (((regs->r_iof_en & mask) != 0) && (iof0_i[i].size() == 0 || iof1_i[i].size() == 0)) {
                if ((regs->r_iof_sel & mask) == 0 && iof0_i[i].size() > 0) {
                    val = last_iof0[i] ? sc_dt::Log_1 : sc_dt::Log_0;
                } else if ((regs->r_iof_sel & mask) == 1 && iof1_i[i].size() > 0)
                    val = last_iof1[i] ? sc_dt::Log_1 : sc_dt::Log_0;
            } else {
                if (regs->r_output_en & mask)
                    val = regs->r_port & mask ? sc_dt::Log_1 : sc_dt::Log_0;
                else
                    val = sc_dt::Log_Z;
                if (regs->r_out_xor & mask) val = ~val;
            }
            tlm::tlm_phase phase = write_output(gp, i, val);
        }
    }
}

void gpio::pin_input(unsigned int tag, tlm::scc::tlm_signal_gp<sc_logic> &gp, sc_core::sc_time &delay) {
    if (delay > SC_ZERO_TIME) {
        wait(delay);
        delay = SC_ZERO_TIME;
    }
    auto mask = 1u << tag;
    switch (gp.get_value().value()) {
    case sc_dt::Log_1:
        if ((regs->r_output_en & mask) == 0) regs->r_value |= mask;
        forward_pin_input(tag, gp);
        break;
    case sc_dt::Log_0:
        if ((regs->r_output_en & mask) == 0) regs->r_value &= ~mask;
        forward_pin_input(tag, gp);
        break;
    default:
    	break;
    }
}

void gpio::forward_pin_input(unsigned int tag, tlm::scc::tlm_signal_gp<sc_logic> &gp) {
    const auto mask = 1U << tag;
    if (regs->iof_en & mask) {
        auto &socket = regs->iof_sel & mask ? iof1_o[tag] : iof0_o[tag];
        tlm::scc::tlm_signal_gp<> new_gp;
        for (size_t i = 0; i < socket.size(); ++i) {
            sc_core::sc_time delay{SC_ZERO_TIME};
            tlm::tlm_phase phase{tlm::BEGIN_REQ};
            new_gp.set_command(tlm::TLM_WRITE_COMMAND);
            new_gp.set_response_status(tlm::TLM_OK_RESPONSE);
            new_gp.set_value(gp.get_value().value() == sc_dt::Log_1);
            new_gp.update_extensions_from(gp);
            socket->nb_transport_fw(new_gp, phase, delay); // we don't care about phase and sync enum
        }
    }
}

void gpio::iof_input(unsigned int tag, unsigned iof_idx, tlm::scc::tlm_signal_gp<> &gp, sc_core::sc_time &delay) {
    if (delay > SC_ZERO_TIME) {
        wait(delay);
        delay = SC_ZERO_TIME;
    }
    const auto mask = 1U << tag;
    if (regs->r_iof_en & mask) {
        const auto idx = regs->r_iof_sel & mask ? 1 : 0;
        if (iof_idx == idx) {
            auto &socket = pins_o[tag];
            for (size_t i = 0; i < socket.size(); ++i) {
                sc_core::sc_time delay{SC_ZERO_TIME};
                tlm::tlm_phase phase{tlm::BEGIN_REQ};
                tlm::scc::tlm_signal_gp<sc_logic> new_gp;
                new_gp.set_command(tlm::TLM_WRITE_COMMAND);
                auto val = gp.get_value();
                new_gp.set_value(val ? sc_dt::Log_1 : sc_dt::Log_0);
                new_gp.copy_extensions_from(gp);
                socket->nb_transport_fw(new_gp, phase, delay); // we don't care about phase and sync enum
                gp.update_extensions_from(new_gp);
            }
        }
    }
}

} /* namespace sifive */
} /* namespace vpvper */
