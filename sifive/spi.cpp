/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "spi.h"
#include "gen/spi_regs.h"
#include <generic/tlm_extensions.h>

#include <cci_configuration>
#include <scc/tlm_target.h>
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>

#include <scc/utilities.h>
#include <util/ities.h>

namespace vpvper {
namespace sifive {
using namespace sc_core;

SC_HAS_PROCESS(spi);
spi::spi(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk)
, NAMEDD(regs, spi_regs)
, rx_fifo(8)
, tx_fifo(8) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();
    SC_THREAD(transmit_data);
    miso_i.register_nb_transport(
        [this](tlm::scc::tlm_signal_gp<bool>& gp, tlm::tlm_phase& phase, sc_core::sc_time& delay) -> tlm::tlm_sync_enum {
            this->receive_data(gp, delay);
            return tlm::TLM_COMPLETED;
        });
    regs->txdata.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            reg.put(data);
            tx_fifo.nb_write(static_cast<uint8_t>(regs->r_txdata.data));
        }
        return true;
    });
    regs->rxdata.set_read_cb([this](const scc::sc_register<uint32_t>& reg, uint32_t& data, sc_core::sc_time d) -> bool {
        if(!this->regs->in_reset()) {
            uint8_t val;
            if(rx_fifo.nb_read(val)) {
                regs->r_rxdata.empty = 0;
                regs->r_rxdata.data = val;
                if(regs->r_rxmark.rxmark <= rx_fifo.num_available()) {
                    regs->r_ip.rxwm = 1;
                    update_irq();
                }
            } else
                regs->r_rxdata.empty = 1;
            data = reg.get() & reg.rdmask;
        }
        return true;
    });
    regs->csmode.set_write_cb([this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
        if(regs->r_csmode.mode == 2 && regs->r_csmode.mode != bit_sub<0, 2>(data) && regs->r_csid < 4) {
            tlm::tlm_phase phase(tlm::BEGIN_REQ);
            sc_core::sc_time delay(sc_core::SC_ZERO_TIME);
            tlm::scc::tlm_signal_gp<> gp;
            gp.set_command(tlm::TLM_WRITE_COMMAND);
            gp.set_value(true);
            scs_o[regs->r_csid]->nb_transport_fw(gp, phase, delay);
        }
        reg.put(data);
        return true;
    });
    regs->csid.set_write_cb([this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
        if(regs->r_csmode.mode == 2 && regs->csid != data && regs->r_csid < 4) {
            tlm::tlm_phase phase(tlm::BEGIN_REQ);
            sc_core::sc_time delay(sc_core::SC_ZERO_TIME);
            tlm::scc::tlm_signal_gp<> gp;
            gp.set_command(tlm::TLM_WRITE_COMMAND);
            gp.set_value(true);
            scs_o[regs->r_csid]->nb_transport_fw(gp, phase, delay);
        }
        reg.put(data);
        return true;
    });
    regs->csdef.set_write_cb([this](const scc::sc_register<uint32_t>& reg, const uint32_t& data, sc_core::sc_time d) -> bool {
        auto diff = regs->csdef ^ data;
        if(regs->r_csmode.mode == 2 && diff != 0 && (regs->r_csid < 4) && (diff & (1 << regs->r_csid)) != 0) {
            tlm::tlm_phase phase(tlm::BEGIN_REQ);
            sc_core::sc_time delay(sc_core::SC_ZERO_TIME);
            tlm::scc::tlm_signal_gp<> gp;
            gp.set_command(tlm::TLM_WRITE_COMMAND);
            gp.set_value(true);
            scs_o[regs->r_csid]->nb_transport_fw(gp, phase, delay);
        }
        reg.put(data);
        return true;
    });
    regs->ie.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        reg.put(data);
        update_irq_evt.notify();
        return true;
    });
    regs->ip.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data, sc_core::sc_time d) -> bool {
        reg.put(data);
        update_irq_evt.notify();
        return true;
    });

    SC_METHOD(update_irq);
    sensitive << update_irq_evt << rx_fifo.data_written_event() << rx_fifo.data_read_event() << tx_fifo.data_written_event()
              << tx_fifo.data_read_event();
}

void spi::clock_cb() { this->clk = clk_i.read(); }

void spi::reset_cb() {
    if(rst_i.read())
        regs->reset_start();
    else
        regs->reset_stop();
}

void spi::transmit_data() {
    uint8_t txdata;
    tlm::tlm_phase phase(tlm::BEGIN_REQ);
    sc_core::sc_time delay(sc_core::SC_ZERO_TIME);
    sc_core::sc_time bit_duration(sc_core::SC_ZERO_TIME);
    sc_core::sc_time start_time;

    auto set_bit = [&](bool val, tlm::scc::tlm_signal_bool_opt_out& socket, bool data_valid = false) -> std::pair<bool, uint32_t> {
        if(socket.get_interface() == nullptr)
            return std::pair<bool, uint32_t>{false, 0};
        auto* gp = tlm::scc::tlm_signal_gp<>::create();
        auto* ext = new vpvper::generic::tlm_signal_spi_extension();
        ext->tx.data_bits = 8;
        ext->start_time = start_time;
        ext->tx.m2s_data = txdata;
        ext->tx.m2s_data_valid = data_valid;
        ext->tx.s2m_data_valid = false;
        gp->set_extension(ext);
        gp->set_command(tlm::TLM_WRITE_COMMAND);
        gp->set_value(val);
        tlm::tlm_phase phase(tlm::BEGIN_REQ);
        gp->acquire();
        phase = tlm::BEGIN_REQ;
        delay = sc_core::SC_ZERO_TIME;
        socket->nb_transport_fw(*gp, phase, delay);
        std::pair<bool, uint32_t> ret{ext->tx.s2m_data_valid != 0, ext->tx.s2m_data};
        gp->release();
        return ret;
    };

    wait(delay); // intentionally 0ns;
    while(true) {
        wait(tx_fifo.data_written_event());
        if(regs->r_csmode.mode != 3 && regs->r_csid < 4) // not in OFF mode
            set_bit(false, scs_o[regs->r_csid]);
        set_bit(regs->r_sckmode.pol, sck_o);
        while(tx_fifo.nb_read(txdata)) {
            regs->r_txdata.full = tx_fifo.num_free() == 0;
            regs->r_ip.txwm = regs->r_txmark.txmark <= (7 - tx_fifo.num_free()) ? 1 : 0;
            update_irq_evt.notify();
            bit_duration = 2 * (regs->r_sckdiv.div + 1) * clk;
            start_time = sc_core::sc_time_stamp();
            set_bit(txdata & 0x80, mosi_o); // 8 data bits, MSB first
            auto s2m = set_bit(1 - regs->r_sckmode.pol, sck_o, true);
            wait(bit_duration / 2);
            set_bit(regs->r_sckmode.pol, sck_o, true);
            wait(bit_duration / 2);
            if(bit_true_transfer.get_value()) {
                for(size_t i = 0, mask = 0x40; i < 7; ++i, mask >= 1) {
                    set_bit(txdata & mask, mosi_o); // 8 data bits, MSB first
                    set_bit(1 - regs->r_sckmode.pol, sck_o);
                    wait(bit_duration / 2);
                    set_bit(regs->r_sckmode.pol, sck_o);
                    wait(bit_duration / 2);
                }
            } else
                wait(7 * bit_duration);
            if(s2m.first)
                rx_fifo.nb_write(s2m.second & 0xff);
            update_irq_evt.notify();
        }
        if(regs->r_csmode.mode == 0 && regs->r_csid < 4) // in AUTO mode
            set_bit(false, scs_o[regs->r_csid]);
    }
}

void spi::receive_data(tlm::scc::tlm_signal_gp<>& gp, sc_core::sc_time& delay) {}

void spi::update_irq() {
    regs->r_ip.rxwm = regs->r_rxmark.rxmark < rx_fifo.num_available();
    regs->r_ip.txwm = regs->r_txmark.txmark <= tx_fifo.num_available();
    regs->r_txdata.full = tx_fifo.num_free() == 0;
    irq_o.write((regs->r_ie.rxwm > 0 && regs->r_ip.rxwm > 0) || (regs->r_ie.txwm > 0 && regs->r_ip.txwm > 0));
}

} /* namespace sifive */
} /* namespace vpvper */
