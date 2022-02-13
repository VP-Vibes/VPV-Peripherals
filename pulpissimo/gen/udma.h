////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2017-2022, MINRES Technologies GmbH
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Created on: Sun Feb 13 12:33:59 CET 2022
//             *      udma.h Author: <RDL Generator>
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _PULPISSIMO_GEN_UDMA_H_
#define _PULPISSIMO_GEN_UDMA_H_

#include <scc/utilities.h>
#include <util/bit_field.h>
#include <scc/register.h>
#include <scc/tlm_target.h>
#include "uart_channel.h"
#include "spi_channel.h"
#include "i2c_channel.h"
#include "i2c_channel.h"
#include "i2s_channel.h"
#include "cam_channel.h"
#include "filter.h"

namespace pulpissimo {
namespace gen {

class udma_regs :
        public sc_core::sc_module,
        public scc::resetable
{
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
    BEGIN_BF_DECL(CTRL_CFG_CG_t, uint32_t);
        BF_FIELD(CG_FILTER, 7, 1);
        BF_FIELD(CG_CAM, 6, 1);
        BF_FIELD(CG_I2S, 5, 1);
        BF_FIELD(CG_SDIO, 4, 1);
        BF_FIELD(CG_I2C1, 3, 1);
        BF_FIELD(CG_I2C0, 2, 1);
        BF_FIELD(CG_SPIM, 1, 1);
        BF_FIELD(CG_UART, 1, 1);
    END_BF_DECL() r_CTRL_CFG_CG;
    BEGIN_BF_DECL(CTRL_CFG_EVENT_t, uint32_t);
        BF_FIELD(CMP_EVT3, 24, 8);
        BF_FIELD(CMP_EVT2, 16, 8);
        BF_FIELD(CMP_EVT1, 8, 8);
        BF_FIELD(CMP_EVT0, 0, 8);
    END_BF_DECL() r_CTRL_CFG_EVENT;
    uint32_t r_CTRL_CFG_RST;
    uart_channel_regs i_uart{"uart"};
    spi_channel_regs i_spi{"spi"};
    i2c_channel_regs i_i2c0{"i2c0"};
    i2c_channel_regs i_i2c1{"i2c1"};
    i2s_channel_regs i_i2s{"i2s"};
    cam_channel_regs i_cam{"cam"};
    filter_regs i_filt{"filt"};
    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    scc::sc_register<CTRL_CFG_CG_t> CTRL_CFG_CG;
    scc::sc_register<CTRL_CFG_EVENT_t> CTRL_CFG_EVENT;
    scc::sc_register<uint32_t> CTRL_CFG_RST;
    // scc::sc_register_file<uart_channel_regs> uart;
    // scc::sc_register_file<spi_channel_regs> spi;
    // scc::sc_register_file<i2c_channel_regs> i2c0;
    // scc::sc_register_file<i2c_channel_regs> i2c1;
    // scc::sc_register_file<i2s_channel_regs> i2s;
    // scc::sc_register_file<cam_channel_regs> cam;
    // scc::sc_register_file<filter_regs> filt;
    
    udma_regs(sc_core::sc_module_name nm);

    template<unsigned BUSWIDTH=32>
    void registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset=0);
};
} // namespace gen
} // namespace pulpissimo
//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline pulpissimo::gen::udma_regs::udma_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(CTRL_CFG_CG, r_CTRL_CFG_CG, 0, *this)
, NAMED(CTRL_CFG_EVENT, r_CTRL_CFG_EVENT, 0, *this)
, NAMED(CTRL_CFG_RST, r_CTRL_CFG_RST, 0, *this)
// , NAMED(uart, i_uart, 0, *this)
// , NAMED(spi, i_spi, 0, *this)
// , NAMED(i2c0, i_i2c0, 0, *this)
// , NAMED(i2c1, i_i2c1, 0, *this)
// , NAMED(i2s, i_i2s, 0, *this)
// , NAMED(cam, i_cam, 0, *this)
// , NAMED(filt, i_filt, 0, *this)
{
}

template<unsigned BUSWIDTH>
inline void pulpissimo::gen::udma_regs::registerResources(scc::tlm_target<BUSWIDTH>& target, uint64_t offset) {
    target.addResource(CTRL_CFG_CG, 0UL);
    target.addResource(CTRL_CFG_EVENT, 0x4UL);
    target.addResource(CTRL_CFG_RST, 0x8UL);
    i_uart.registerResources(target, 0x80UL+offset);
    i_spi.registerResources(target, 0x100UL+offset);
    i_i2c0.registerResources(target, 0x180UL+offset);
    i_i2c1.registerResources(target, 0x200UL+offset);
    i_i2s.registerResources(target, 0x300UL+offset);
    i_cam.registerResources(target, 0x380UL+offset);
    i_filt.registerResources(target, 0x400UL+offset);
}

#endif // _PULPISSIMO_GEN_UDMA_H_
