/*
 * Copyright (c) 2023 - 2026 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Generated at 2026-01-10 11:30:19 UTC
 * by peakrdl_mnrs version 1.2.10
 */

#ifndef _ETHMAC_H_
#define _ETHMAC_H_

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
    BF_FIELD(tx_space_avail, 1, 1);
    BF_FIELD(tx_aligner_enable, 2, 1);
    BF_FIELD(rx_flush, 4, 1);
    BF_FIELD(rx_data_avail, 5, 1);
    BF_FIELD(rx_aligner_enable, 6, 1);
    END_BF_DECL() r_mac_ctrl;

    uint32_t r_tx_data_reg;

    BEGIN_BF_DECL(tx_avail_reg_t, uint32_t);
    BF_FIELD(tx_availibility, 0, 9);
    END_BF_DECL() r_tx_avail_reg;

    uint32_t r_rx_data_reg;

    BEGIN_BF_DECL(rx_stat_reg_t, uint32_t);
    BF_FIELD(rx_errors, 0, 8);
    BF_FIELD(rx_drops, 8, 8);
    END_BF_DECL() r_rx_stat_reg;

    BEGIN_BF_DECL(int_ctrl_reg_t, uint32_t);
    BF_FIELD(tx_free_intr_enable, 0, 1);
    BF_FIELD(rx_data_avail_intr_enable, 1, 1);
    END_BF_DECL() r_int_ctrl_reg;

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////

    scc::sc_register<mac_ctrl_t> mac_ctrl;
    scc::sc_register<uint32_t> tx_data_reg;
    scc::sc_register<tx_avail_reg_t> tx_avail_reg;
    scc::sc_register<uint32_t> rx_data_reg;
    scc::sc_register<rx_stat_reg_t> rx_stat_reg;
    scc::sc_register<int_ctrl_reg_t> int_ctrl_reg;

    ethmac_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline ethmac_regs::ethmac_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(mac_ctrl, r_mac_ctrl, 0, *this)
, NAMED(tx_data_reg, r_tx_data_reg, 0, *this)
, NAMED(tx_avail_reg, r_tx_avail_reg, 0, *this)
, NAMED(rx_data_reg, r_rx_data_reg, 0, *this)
, NAMED(rx_stat_reg, r_rx_stat_reg, 0, *this)
, NAMED(int_ctrl_reg, r_int_ctrl_reg, 0, *this) {}

template <unsigned BUSWIDTH> inline void ethmac_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(mac_ctrl, 0x0UL);
    target.addResource(tx_data_reg, 0x10UL);
    target.addResource(tx_avail_reg, 0x14UL);
    target.addResource(rx_data_reg, 0x20UL);
    target.addResource(rx_stat_reg, 0x2cUL);
    target.addResource(int_ctrl_reg, 0x30UL);
}
} // namespace gen
} // namespace minres
} // namespace vpvper
#endif // _ETHMAC_H_