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
namespace minres {

class gpio_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    uint32_t r_pin_in;

    uint32_t r_pin_out;

    uint32_t r_out_en;

    // register declarations
    scc::sc_register<uint32_t> pin_in;
    scc::sc_register<uint32_t> pin_out;
    scc::sc_register<uint32_t> out_en;

    gpio_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);

    void trace(sc_core::sc_trace_file *tf) const override;
};
} /* namespace minres */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::minres::gpio_regs::gpio_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(pin_in, r_pin_in, 0, *this)
, NAMED(pin_out, r_pin_out, 0, *this)
, NAMED(out_en, r_out_en, 0, *this)
 {}

template <unsigned BUSWIDTH> inline void vpvper::minres::gpio_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(pin_in, 0x0UL);
    target.addResource(pin_out, 0x4UL);
    target.addResource(out_en, 0x8UL);
}

inline void vpvper::minres::gpio_regs::trace(sc_core::sc_trace_file *tf) const {
    pin_in.trace(tf);
    pin_out.trace(tf);
    out_en.trace(tf);
}

#endif // _GPIO_REGS_H_
