/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SPI_REGS_H_
#define _SPI_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class spi_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(sckdiv_t, uint32_t);
    BF_FIELD(div, 0, 12);
    END_BF_DECL() r_sckdiv;

    BEGIN_BF_DECL(sckmode_t, uint32_t);
    BF_FIELD(pha, 0, 1);
    BF_FIELD(pol, 1, 1);
    END_BF_DECL() r_sckmode;

    uint32_t r_csid;

    uint32_t r_csdef;

    BEGIN_BF_DECL(csmode_t, uint32_t);
    BF_FIELD(mode, 0, 2);
    END_BF_DECL() r_csmode;

    BEGIN_BF_DECL(delay0_t, uint32_t);
    BF_FIELD(cssck, 0, 8);
    BF_FIELD(sckcs, 16, 8);
    END_BF_DECL() r_delay0;

    BEGIN_BF_DECL(delay1_t, uint32_t);
    BF_FIELD(intercs, 0, 16);
    BF_FIELD(interxfr, 16, 8);
    END_BF_DECL() r_delay1;

    BEGIN_BF_DECL(fmt_t, uint32_t);
    BF_FIELD(proto, 0, 2);
    BF_FIELD(endian, 2, 1);
    BF_FIELD(dir, 3, 1);
    BF_FIELD(len, 16, 4);
    END_BF_DECL() r_fmt;

    BEGIN_BF_DECL(txdata_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(full, 31, 1);
    END_BF_DECL() r_txdata;

    BEGIN_BF_DECL(rxdata_t, uint32_t);
    BF_FIELD(data, 0, 8);
    BF_FIELD(empty, 31, 1);
    END_BF_DECL() r_rxdata;

    BEGIN_BF_DECL(txmark_t, uint32_t);
    BF_FIELD(txmark, 0, 3);
    END_BF_DECL() r_txmark;

    BEGIN_BF_DECL(rxmark_t, uint32_t);
    BF_FIELD(rxmark, 0, 3);
    END_BF_DECL() r_rxmark;

    BEGIN_BF_DECL(fctrl_t, uint32_t);
    BF_FIELD(en, 0, 1);
    END_BF_DECL() r_fctrl;

    BEGIN_BF_DECL(ffmt_t, uint32_t);
    BF_FIELD(cmd_en, 0, 1);
    BF_FIELD(addr_len, 1, 2);
    BF_FIELD(pad_cnt, 3, 4);
    BF_FIELD(cmd_proto, 7, 2);
    BF_FIELD(addr_proto, 9, 2);
    BF_FIELD(data_proto, 11, 2);
    BF_FIELD(cmd_code, 16, 8);
    BF_FIELD(pad_code, 24, 8);
    END_BF_DECL() r_ffmt;

    BEGIN_BF_DECL(ie_t, uint32_t);
    BF_FIELD(txwm, 0, 1);
    BF_FIELD(rxwm, 1, 1);
    END_BF_DECL() r_ie;

    BEGIN_BF_DECL(ip_t, uint32_t);
    BF_FIELD(txwm, 0, 1);
    BF_FIELD(rxwm, 1, 1);
    END_BF_DECL() r_ip;

    // register declarations
    scc::sc_register<sckdiv_t> sckdiv;
    scc::sc_register<sckmode_t> sckmode;
    scc::sc_register<uint32_t> csid;
    scc::sc_register<uint32_t> csdef;
    scc::sc_register<csmode_t> csmode;
    scc::sc_register<delay0_t> delay0;
    scc::sc_register<delay1_t> delay1;
    scc::sc_register<fmt_t> fmt;
    scc::sc_register<txdata_t> txdata;
    scc::sc_register<rxdata_t> rxdata;
    scc::sc_register<txmark_t> txmark;
    scc::sc_register<rxmark_t> rxmark;
    scc::sc_register<fctrl_t> fctrl;
    scc::sc_register<ffmt_t> ffmt;
    scc::sc_register<ie_t> ie;
    scc::sc_register<ip_t> ip;

    spi_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::spi_regs::spi_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(sckdiv, r_sckdiv, 0, *this)
, NAMED(sckmode, r_sckmode, 0, *this)
, NAMED(csid, r_csid, 0, *this)
, NAMED(csdef, r_csdef, 0, *this)
, NAMED(csmode, r_csmode, 0, *this)
, NAMED(delay0, r_delay0, 0, *this)
, NAMED(delay1, r_delay1, 0, *this)
, NAMED(fmt, r_fmt, 0, *this)
, NAMED(txdata, r_txdata, 0, *this)
, NAMED(rxdata, r_rxdata, 0, *this)
, NAMED(txmark, r_txmark, 0, *this)
, NAMED(rxmark, r_rxmark, 0, *this)
, NAMED(fctrl, r_fctrl, 0, *this)
, NAMED(ffmt, r_ffmt, 0, *this)
, NAMED(ie, r_ie, 0, *this)
, NAMED(ip, r_ip, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::spi_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(sckdiv, 0x0UL);
    target.addResource(sckmode, 0x4UL);
    target.addResource(csid, 0x10UL);
    target.addResource(csdef, 0x14UL);
    target.addResource(csmode, 0x18UL);
    target.addResource(delay0, 0x28UL);
    target.addResource(delay1, 0x2cUL);
    target.addResource(fmt, 0x40UL);
    target.addResource(txdata, 0x48UL);
    target.addResource(rxdata, 0x4cUL);
    target.addResource(txmark, 0x50UL);
    target.addResource(rxmark, 0x54UL);
    target.addResource(fctrl, 0x60UL);
    target.addResource(ffmt, 0x64UL);
    target.addResource(ie, 0x70UL);
    target.addResource(ip, 0x74UL);
}

#endif // _SPI_REGS_H_
