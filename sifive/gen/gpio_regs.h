/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _GPIO_REGS_H_
#define _GPIO_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class gpio_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    uint32_t r_value;

    uint32_t r_input_en;

    uint32_t r_output_en;

    uint32_t r_port;

    uint32_t r_pue;

    uint32_t r_ds;

    uint32_t r_rise_ie;

    uint32_t r_rise_ip;

    uint32_t r_fall_ie;

    uint32_t r_fall_ip;

    uint32_t r_high_ie;

    uint32_t r_high_ip;

    uint32_t r_low_ie;

    uint32_t r_low_ip;

    uint32_t r_iof_en;

    uint32_t r_iof_sel;

    uint32_t r_out_xor;

    // register declarations
    scc::sc_register<uint32_t> value;
    scc::sc_register<uint32_t> input_en;
    scc::sc_register<uint32_t> output_en;
    scc::sc_register<uint32_t> port;
    scc::sc_register<uint32_t> pue;
    scc::sc_register<uint32_t> ds;
    scc::sc_register<uint32_t> rise_ie;
    scc::sc_register<uint32_t> rise_ip;
    scc::sc_register<uint32_t> fall_ie;
    scc::sc_register<uint32_t> fall_ip;
    scc::sc_register<uint32_t> high_ie;
    scc::sc_register<uint32_t> high_ip;
    scc::sc_register<uint32_t> low_ie;
    scc::sc_register<uint32_t> low_ip;
    scc::sc_register<uint32_t> iof_en;
    scc::sc_register<uint32_t> iof_sel;
    scc::sc_register<uint32_t> out_xor;

    gpio_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::gpio_regs::gpio_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(value, r_value, 0, *this)
, NAMED(input_en, r_input_en, 0, *this)
, NAMED(output_en, r_output_en, 0, *this)
, NAMED(port, r_port, 0, *this)
, NAMED(pue, r_pue, 0, *this)
, NAMED(ds, r_ds, 0, *this)
, NAMED(rise_ie, r_rise_ie, 0, *this)
, NAMED(rise_ip, r_rise_ip, 0, *this)
, NAMED(fall_ie, r_fall_ie, 0, *this)
, NAMED(fall_ip, r_fall_ip, 0, *this)
, NAMED(high_ie, r_high_ie, 0, *this)
, NAMED(high_ip, r_high_ip, 0, *this)
, NAMED(low_ie, r_low_ie, 0, *this)
, NAMED(low_ip, r_low_ip, 0, *this)
, NAMED(iof_en, r_iof_en, 0, *this)
, NAMED(iof_sel, r_iof_sel, 0, *this)
, NAMED(out_xor, r_out_xor, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::gpio_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(value, 0x0UL);
    target.addResource(input_en, 0x4UL);
    target.addResource(output_en, 0x8UL);
    target.addResource(port, 0xcUL);
    target.addResource(pue, 0x10UL);
    target.addResource(ds, 0x14UL);
    target.addResource(rise_ie, 0x18UL);
    target.addResource(rise_ip, 0x1cUL);
    target.addResource(fall_ie, 0x20UL);
    target.addResource(fall_ip, 0x24UL);
    target.addResource(high_ie, 0x28UL);
    target.addResource(high_ip, 0x2cUL);
    target.addResource(low_ie, 0x30UL);
    target.addResource(low_ip, 0x34UL);
    target.addResource(iof_en, 0x38UL);
    target.addResource(iof_sel, 0x3cUL);
    target.addResource(out_xor, 0x40UL);
}

inline void vpvper::sifive::gpio_regs::trace(sc_core::sc_trace_file *tf) const {
    value.trace(tf);
    input_en.trace(tf);
    output_en.trace(tf);
    port.trace(tf);
    pue.trace(tf);
    ds.trace(tf);
    rise_ie.trace(tf);
    rise_ip.trace(tf);
    fall_ie.trace(tf);
    fall_ip.trace(tf);
    high_ie.trace(tf);
    high_ip.trace(tf);
    low_ie.trace(tf);
    low_ip.trace(tf);
    iof_en.trace(tf);
    iof_sel.trace(tf);
    out_xor.trace(tf);
}

#endif // _GPIO_REGS_H_
