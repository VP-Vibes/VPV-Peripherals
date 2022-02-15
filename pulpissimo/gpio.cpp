/*
 * Copyright (c) 2019 -2022 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio.h"
#include "gen/gpio.h"

#include <scc/utilities.h>
#include <scc/report.h>

namespace pulpissimo {
SC_HAS_PROCESS(gpio);// NOLINT

gpio::gpio(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk)
, NAMEDD(regs, gen::gpio_regs) {
    regs->registerResources(*this);
    SC_METHOD(clock_cb);
    sensitive << clk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    SC_METHOD(in_31_0_cb);
    for(auto i = 0U; i<32; ++i)
        sensitive<<in[i];
    SC_METHOD(in_63_32_cb);
    for(auto i = 0U; i<32; ++i)
        sensitive<<in[i+32];

    auto ro_wr_cb = [this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time t)-> bool {return true;};

    regs->PADIN_00_31.set_write_cb(ro_wr_cb);
    regs->PADIN_32_63.set_write_cb(ro_wr_cb);
    regs->PADOUT_00_31.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time t)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1)
            out[i].write(v&m);
        return false;
    });
    regs->PADOUT_32_63.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time t)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1)
            out[i+32].write(v&m);
        return false;
    });
    regs->PADDIR_00_31.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time t)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1)
            dir[i].write(v&m);
        return false;
    });
    regs->PADDIR_32_63.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time t)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1)
            out[i+32].write(v&m);
        return false;
    });
    regs->PADOUTSET_00_31.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1) {
            if(v&m) {
                out[i].write(true);
                regs->r_PADOUT_00_31|=m;
            }
        }
        return true;
    });
    regs->PADOUTSET_32_63.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1) {
            if(v&m) {
                out[i+32].write(true);
                regs->r_PADOUT_32_63|=m;
            }
        }
        return true;
    });
    regs->PADOUTCLR_00_31.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1) {
            if(v&m) {
                out[i].write(false);
                regs->r_PADOUT_00_31&=~m;
            }
        }
        return true;
    });
    regs->PADOUTCLR_32_63.set_write_cb([this](scc::sc_register<uint32_t>&, const uint32_t& v, sc_core::sc_time)-> bool {
        for(auto i=0u, m=1u; i<32; ++i, m<<=1) {
            if(v&m) {
                out[i+32].write(false);
                regs->r_PADOUT_32_63&=~m;
            }
        }
        return true;
    });
    regs->INTSTATUS_00_31.set_write_cb(ro_wr_cb);
    regs->INTSTATUS_00_31.set_read_cb([this](scc::sc_register<uint32_t> const&, uint32_t& v, sc_core::sc_time)-> bool {
        v = regs->r_INTSTATUS_00_31;
        regs->r_INTSTATUS_00_31=0;
        return true;
    });
    regs->INTSTATUS_32_63.set_write_cb(ro_wr_cb);
    regs->INTSTATUS_32_63.set_read_cb([this](scc::sc_register<uint32_t> const&, uint32_t& v, sc_core::sc_time)-> bool {
        v = regs->r_INTSTATUS_32_63;
        regs->r_INTSTATUS_32_63=0;
        return true;
    });
}

gpio::~gpio() {} // NOLINT

void gpio::clock_cb() { this->clk = clk_i.read(); }

void gpio::reset_cb() {
    if (rst_i.read()) {
        regs->reset_start();
    } else {
        regs->reset_stop();
    }
}

void gpio::in_31_0_cb() {
    for(auto i = 0U; i<32; ++i) {
        auto const& p = in[i];
        if(p.event()) {
            if(p.read())
                regs->r_PADIN_00_31 |= 1u<<i;
            else
                regs->r_PADIN_00_31 &= ~(1u<<i);
            check_irq(i, p.read());
        }
    }
}

void gpio::in_63_32_cb() {
    for(auto i = 0U; i<32; ++i) {
        auto const& p = in[32+i];
        if(p.event()) {
            if(p.read())
                regs->r_PADIN_32_63 |= 1u<<i;
            else
                regs->r_PADIN_32_63 &= ~(1u<<i);
            check_irq(i, p.read());
        }
    }
}

inline unsigned gpio::get_int_type(unsigned bit) {
    if(bit<16){
        return (regs->r_INTTYPE_00_15>>(2*(bit&0xf))) & 0x3;
    }else if(bit<32){
        return (regs->r_INTTYPE_16_31>>(2*(bit&0xf))) & 0x3;
    }else if(bit<48){
        return (regs->r_INTTYPE_32_47>>(2*(bit&0xf))) & 0x3;
    }else {
        return (regs->r_INTTYPE_48_63>>(2*(bit&0xf))) & 0x3;
    }
}
void gpio::check_irq(unsigned bit, bool new_val) {
    sc_assert(bit<64);
    if(bit<32) {
        auto mask = 1UL<<bit;
        //if(regs->r_INTEN_00_31&mask){
            switch(get_int_type(bit)) {
            case 0: // falling edge
                if(!new_val) regs->r_INTSTATUS_00_31|= mask;
                break;
            case 1: // rising edge
                if(new_val) regs->r_INTSTATUS_00_31|= mask;
                break;
            case 2: // all edges
                regs->r_INTSTATUS_00_31|= mask;
                break;
            default:
                break;
            }
        //}
        if(regs->r_INTSTATUS_00_31& regs->INTEN_00_31)
            irq_o.write(true);
    } else {
        auto mask = 1UL<<(bit-32);
        //if(regs->r_INTEN_32_63&mask){
            switch(get_int_type(bit)) {
            case 0: // falling edge
                if(!new_val) regs->r_INTSTATUS_32_63|= mask;
                break;
            case 1: // rising edge
                if(new_val) regs->r_INTSTATUS_32_63|= mask;
                break;
            case 2: // all edges
                regs->r_INTSTATUS_32_63|= mask;
                break;
            default:
                break;
            }
        //}
        if(regs->r_INTSTATUS_32_63& regs->r_INTEN_32_63)
            irq_o.write(true);
    }
}

} /* namespace pulpissimo */
