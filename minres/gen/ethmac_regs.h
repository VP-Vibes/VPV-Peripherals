/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-26 15:30:36 UTC
 * by peakrdl_mnrs version 1.3.1
 */

#ifndef GEN_ETHMAC_H_
#define GEN_ETHMAC_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {
namespace gen {

class ethmac_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(mac_ctrl_t, uint32_t);
    BF_FIELD(tx_flush, 0, 1);
    BF_FIELD(tx_ready, 1, 1);
    BF_FIELD(tx_aligner_enable, 2, 1);
    BF_FIELD(rx_flush, 4, 1);
    BF_FIELD(rx_pending, 5, 1);
    BF_FIELD(rx_aligner_enable, 6, 1);
    END_BF_DECL() r_mac_ctrl;

    uint32_t r_mac_tx;

    BEGIN_BF_DECL(mac_tx_availability_t, uint32_t);
    BF_FIELD(words_avail, 0, 11);
    END_BF_DECL() r_mac_tx_availability;

    uint32_t r_mac_rx;

    BEGIN_BF_DECL(mac_rx_stats_t, uint32_t);
    BF_FIELD(rx_errors, 0, 8);
    BF_FIELD(rx_drops, 8, 8);
    END_BF_DECL() r_mac_rx_stats;

    BEGIN_BF_DECL(mac_intr_t, uint32_t);
    BF_FIELD(tx_free_intr_enable, 0, 1);
    BF_FIELD(rx_data_avail_intr_enable, 1, 1);
    END_BF_DECL() r_mac_intr;

    BEGIN_BF_DECL(mdio_data_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(write, 8, 1);
    BF_FIELD(read, 9, 1);
    BF_FIELD(ssgen, 11, 1);
    BF_FIELD(rx_data_invalid, 31, 1);
    END_BF_DECL() r_mdio_data;

    BEGIN_BF_DECL(mdio_status_t, uint32_t);
    BF_FIELD(tx_free, 0, 6);
    BF_FIELD(rx_avail, 16, 6);
    END_BF_DECL() r_mdio_status;

    BEGIN_BF_DECL(mdio_config_t, uint32_t);
    BF_FIELD(cpol, 0, 1);
    BF_FIELD(cpha, 1, 1);
    BF_FIELD(mode, 4, 1);
    END_BF_DECL() r_mdio_config;

    BEGIN_BF_DECL(mdio_intr_t, uint32_t);
    BF_FIELD(tx_ie, 0, 1);
    BF_FIELD(rx_ie, 1, 1);
    BF_FIELD(tx_ip, 8, 1);
    BF_FIELD(rx_ip, 9, 1);
    BF_FIELD(tx_active, 16, 1);
    END_BF_DECL() r_mdio_intr;

    BEGIN_BF_DECL(mdio_sclk_config_t, uint32_t);
    BF_FIELD(clk_divider, 0, 12);
    END_BF_DECL() r_mdio_sclk_config;

    BEGIN_BF_DECL(mdio_ssgen_setup_t, uint32_t);
    BF_FIELD(setup_cycles, 0, 12);
    END_BF_DECL() r_mdio_ssgen_setup;

    BEGIN_BF_DECL(mdio_ssgen_hold_t, uint32_t);
    BF_FIELD(hold_cycles, 0, 12);
    END_BF_DECL() r_mdio_ssgen_hold;

    BEGIN_BF_DECL(mdio_ssgen_disable_t, uint32_t);
    BF_FIELD(disable_cycles, 0, 12);
    END_BF_DECL() r_mdio_ssgen_disable;

    BEGIN_BF_DECL(mdio_ssgen_active_high_t, uint32_t);
    BF_FIELD(spi_cs_active_high, 0, 1);
    END_BF_DECL() r_mdio_ssgen_active_high;

    BEGIN_BF_DECL(mdio_direct_write_t, uint32_t);
    BF_FIELD(data, 0, 8);
    END_BF_DECL() r_mdio_direct_write;

    BEGIN_BF_DECL(mdio_direct_read_write_t, uint32_t);
    BF_FIELD(data, 0, 8);
    END_BF_DECL() r_mdio_direct_read_write;

    BEGIN_BF_DECL(mdio_direct_read_t, uint32_t);
    BF_FIELD(data, 0, 8);
    END_BF_DECL() r_mdio_direct_read;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    scc::sc_register<mac_ctrl_t> mac_ctrl;
    scc::sc_register<uint32_t> mac_tx;
    scc::sc_register<mac_tx_availability_t> mac_tx_availability;
    scc::sc_register<uint32_t> mac_rx;
    scc::sc_register<mac_rx_stats_t> mac_rx_stats;
    scc::sc_register<mac_intr_t> mac_intr;
    scc::sc_register<mdio_data_t> mdio_data;
    scc::sc_register<mdio_status_t> mdio_status;
    scc::sc_register<mdio_config_t> mdio_config;
    scc::sc_register<mdio_intr_t> mdio_intr;
    scc::sc_register<mdio_sclk_config_t> mdio_sclk_config;
    scc::sc_register<mdio_ssgen_setup_t> mdio_ssgen_setup;
    scc::sc_register<mdio_ssgen_hold_t> mdio_ssgen_hold;
    scc::sc_register<mdio_ssgen_disable_t> mdio_ssgen_disable;
    scc::sc_register<mdio_ssgen_active_high_t> mdio_ssgen_active_high;
    scc::sc_register<mdio_direct_write_t> mdio_direct_write;
    scc::sc_register<mdio_direct_read_write_t> mdio_direct_read_write;
    scc::sc_register<mdio_direct_read_t> mdio_direct_read;

    ethmac_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline ethmac_regs::ethmac_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(mac_ctrl, r_mac_ctrl, 0, *this)
, NAMED(mac_tx, r_mac_tx, 0, *this)
, NAMED(mac_tx_availability, r_mac_tx_availability, 0, *this)
, NAMED(mac_rx, r_mac_rx, 0, *this)
, NAMED(mac_rx_stats, r_mac_rx_stats, 0, *this)
, NAMED(mac_intr, r_mac_intr, 0, *this)
, NAMED(mdio_data, r_mdio_data, 0, *this)
, NAMED(mdio_status, r_mdio_status, 0, *this)
, NAMED(mdio_config, r_mdio_config, 0, *this)
, NAMED(mdio_intr, r_mdio_intr, 0, *this)
, NAMED(mdio_sclk_config, r_mdio_sclk_config, 0, *this)
, NAMED(mdio_ssgen_setup, r_mdio_ssgen_setup, 0, *this)
, NAMED(mdio_ssgen_hold, r_mdio_ssgen_hold, 0, *this)
, NAMED(mdio_ssgen_disable, r_mdio_ssgen_disable, 0, *this)
, NAMED(mdio_ssgen_active_high, r_mdio_ssgen_active_high, 0, *this)
, NAMED(mdio_direct_write, r_mdio_direct_write, 0, *this)
, NAMED(mdio_direct_read_write, r_mdio_direct_read_write, 0, *this)
, NAMED(mdio_direct_read, r_mdio_direct_read, 0, *this) {}

template <unsigned BUSWIDTH> inline void ethmac_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(mac_ctrl, 0x0UL);
    target.addResource(mac_tx, 0x10UL);
    target.addResource(mac_tx_availability, 0x14UL);
    target.addResource(mac_rx, 0x20UL);
    target.addResource(mac_rx_stats, 0x2cUL);
    target.addResource(mac_intr, 0x30UL);
    target.addResource(mdio_data, 0x40UL);
    target.addResource(mdio_status, 0x44UL);
    target.addResource(mdio_config, 0x48UL);
    target.addResource(mdio_intr, 0x4cUL);
    target.addResource(mdio_sclk_config, 0x60UL);
    target.addResource(mdio_ssgen_setup, 0x64UL);
    target.addResource(mdio_ssgen_hold, 0x68UL);
    target.addResource(mdio_ssgen_disable, 0x6cUL);
    target.addResource(mdio_ssgen_active_high, 0x70UL);
    target.addResource(mdio_direct_write, 0x90UL);
    target.addResource(mdio_direct_read_write, 0x94UL);
    target.addResource(mdio_direct_read, 0x98UL);
}
} // namespace gen
} // namespace minres
} // namespace vpvper
#endif // GEN_ETHMAC_H_