/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _UART_REGS_H_
#define _UART_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class uart_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(txdata_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(full, 31, 1);
    END_BF_DECL() r_txdata;

    BEGIN_BF_DECL(rxdata_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(empty, 31, 1);
    END_BF_DECL() r_rxdata;

    BEGIN_BF_DECL(txctrl_t, uint32_t);
    BF_FIELD(txen, 0, 1);
    BF_FIELD(nstop, 1, 1);
    BF_FIELD(txcnt, 16, 3);
    END_BF_DECL() r_txctrl;

    BEGIN_BF_DECL(rxctrl_t, uint32_t);
    BF_FIELD(rxen, 0, 1);
    BF_FIELD(rxcnt, 16, 3);
    END_BF_DECL() r_rxctrl;

    BEGIN_BF_DECL(ie_t, uint32_t);
    BF_FIELD(txwm, 0, 1);
    BF_FIELD(rxwm, 1, 1);
    END_BF_DECL() r_ie;

    BEGIN_BF_DECL(ip_t, uint32_t);
    BF_FIELD(txwm, 0, 1);
    BF_FIELD(rxwm, 1, 1);
    END_BF_DECL() r_ip;

    BEGIN_BF_DECL(div_t, uint32_t);
    BF_FIELD(div, 0, 16);
    END_BF_DECL() r_div;

    // register declarations
    scc::sc_register<txdata_t> txdata;
    scc::sc_register<rxdata_t> rxdata;
    scc::sc_register<txctrl_t> txctrl;
    scc::sc_register<rxctrl_t> rxctrl;
    scc::sc_register<ie_t> ie;
    scc::sc_register<ip_t> ip;
    scc::sc_register<div_t> div;

    uart_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH>& target);
};
} /* namespace sifive */
} /* namespace vpvper */
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::uart_regs::uart_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(txdata, r_txdata, 0, *this)
, NAMED(rxdata, r_rxdata, 0, *this)
, NAMED(txctrl, r_txctrl, 0, *this)
, NAMED(rxctrl, r_rxctrl, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this)
, NAMED(div, r_div, 0, *this) {}

template <unsigned BUSWIDTH>
inline void vpvper::sifive::uart_regs::registerResources(scc::tlm_target<BUSWIDTH>& target) {
    target.addResource(txdata, 0x0UL);
    target.addResource(rxdata, 0x4UL);
    target.addResource(txctrl, 0x8UL);
    target.addResource(rxctrl, 0xcUL);
    target.addResource(ie, 0x10UL);
    target.addResource(ip, 0x14UL);
    target.addResource(div, 0x18UL);
}

#endif // _UART_REGS_H_
