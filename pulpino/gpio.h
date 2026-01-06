/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_GPIO_H__
#define __VPVPER_PULPINO_GPIO_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

namespace vpvper {
namespace pulpino {

/////////////////////////////////////////////////////////////////////////////
/// \class GPIORegs
class GPIORegs : public scc::tlm_target_bfs_register_base<GPIORegs> {
public:
    std::array<scc::bitfield_register<uint32_t>, 6 + 8> registers{{{"PADDIR", 0x00},
                                                                   {"PADIN", 0x04},
                                                                   {"PADOUT", 0x08},
                                                                   {"INTEN", 0x0c},
                                                                   {"INTTYPE", 0x10},
                                                                   {"INTSTATUS", 0x18, 0, 0},
                                                                   REGISTER_ARRAY("PADCFG_reg", 0x20, 4, 8)}}; ///< GPIO registers
    std::array<scc::bitfield<uint32_t>, 6 * 32 + 1> bitfields{
        {BITFIELD_ARRAY_POSTFIX("PADDIR", , 0, 1, "gpio.pads_", ".dir", 32),
         BITFIELD_ARRAY_POSTFIX("PADIN", , 0, 1, "gpio.pads_", ".in", 32),
         BITFIELD_ARRAY_POSTFIX("PADOUT", , 0, 1, "gpio.pads_", ".out", 32),
         BITFIELD_ARRAY_POSTFIX("INTEN", , 0, 1, "gpio.pads_", ".intEn", 32),
         BITFIELD_ARRAY_POSTFIX("INTTYPE", , 0, 2, "gpio.pads_", ".intType", 32),
         {getRegister("INTSTATUS"), "INTSTATUS", 0, 32, "gpio.intStatus"},
         {getRegister("PADCFG_reg0"), "PADCFG0", 0, 5, "gpio.pads_0.cfg"},
         {getRegister("PADCFG_reg0"), "PADCFG1", 8, 5, "gpio.pads_1.cfg"},
         {getRegister("PADCFG_reg0"), "PADCFG2", 16, 5, "gpio.pads_2.cfg"},
         {getRegister("PADCFG_reg0"), "PADCFG3", 24, 5, "gpio.pads_3.cfg"},
         {getRegister("PADCFG_reg1"), "PADCFG4", 0, 5, "gpio.pads_4.cfg"},
         {getRegister("PADCFG_reg1"), "PADCFG5", 8, 5, "gpio.pads_5.cfg"},
         {getRegister("PADCFG_reg1"), "PADCFG6", 16, 5, "gpio.pads_6.cfg"},
         {getRegister("PADCFG_reg1"), "PADCFG7", 24, 5, "gpio.pads_7.cfg"},
         {getRegister("PADCFG_reg2"), "PADCFG8", 0, 5, "gpio.pads_8.cfg"},
         {getRegister("PADCFG_reg2"), "PADCFG9", 8, 5, "gpio.pads_9.cfg"},
         {getRegister("PADCFG_reg2"), "PADCFG10", 16, 5, "gpio.pads_10.cfg"},
         {getRegister("PADCFG_reg2"), "PADCFG11", 24, 5, "gpio.pads_11.cfg"},
         {getRegister("PADCFG_reg3"), "PADCFG12", 0, 5, "gpio.pads_12.cfg"},
         {getRegister("PADCFG_reg3"), "PADCFG13", 8, 5, "gpio.pads_13.cfg"},
         {getRegister("PADCFG_reg3"), "PADCFG14", 16, 5, "gpio.pads_14.cfg"},
         {getRegister("PADCFG_reg3"), "PADCFG15", 24, 5, "gpio.pads_15.cfg"},
         {getRegister("PADCFG_reg4"), "PADCFG16", 0, 5, "gpio.pads_16.cfg"},
         {getRegister("PADCFG_reg4"), "PADCFG17", 8, 5, "gpio.pads_17.cfg"},
         {getRegister("PADCFG_reg4"), "PADCFG18", 16, 5, "gpio.pads_18.cfg"},
         {getRegister("PADCFG_reg4"), "PADCFG19", 24, 5, "gpio.pads_19.cfg"},
         {getRegister("PADCFG_reg5"), "PADCFG20", 0, 5, "gpio.pads_20.cfg"},
         {getRegister("PADCFG_reg5"), "PADCFG21", 8, 5, "gpio.pads_21.cfg"},
         {getRegister("PADCFG_reg5"), "PADCFG22", 16, 5, "gpio.pads_22.cfg"},
         {getRegister("PADCFG_reg5"), "PADCFG23", 24, 5, "gpio.pads_23.cfg"},
         {getRegister("PADCFG_reg6"), "PADCFG24", 0, 5, "gpio.pads_24.cfg"},
         {getRegister("PADCFG_reg6"), "PADCFG25", 8, 5, "gpio.pads_25.cfg"},
         {getRegister("PADCFG_reg6"), "PADCFG26", 16, 5, "gpio.pads_26.cfg"},
         {getRegister("PADCFG_reg6"), "PADCFG27", 24, 5, "gpio.pads_27.cfg"},
         {getRegister("PADCFG_reg7"), "PADCFG28", 0, 5, "gpio.pads_28.cfg"},
         {getRegister("PADCFG_reg7"), "PADCFG29", 8, 5, "gpio.pads_29.cfg"},
         {getRegister("PADCFG_reg7"), "PADCFG30", 16, 5, "gpio.pads_30.cfg"},
         {getRegister("PADCFG_reg7"), "PADCFG31", 24, 5, "gpio.pads_31.cfg"}}}; ///< GPIO register bitfields
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    GPIORegs(sc_core::sc_module_name name)
    : scc::tlm_target_bfs_register_base<GPIORegs>{name} {}
};

/////////////////////////////////////////////////////////////////////////////
/// \class GPIO
/// \brief tlm_target_bfs based GPIO peripheral. Losely follows Pulp platform APB GPIO:
/// https://github.com/pulp-platform/apb_gpio
template <typename owner_t> class GPIO : public scc::tlm_target_bfs<GPIORegs, owner_t> {
    typedef scc::tlm_target_bfs<GPIORegs, owner_t> bfs_t;
    constexpr static const char* ID_GPIO{"gpio"};
    SC_HAS_PROCESS(GPIO);
    enum IntType { Level1, Level0, Rise, Fall };

public:
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    GPIO(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner = nullptr);

    std::array<sc_core::sc_in<bool>, 32> in;
    ///< input model
    std::array<sc_core::sc_out<bool>, 32> out;
    ///< output model
    std::array<sc_core::sc_out<bool>, 32> dir;
    ///< directions

private:
    std::array<scc::bitfield<uint32_t>*, 32> padin;
    void in_cb() {
        for(size_t i = 0; i < 32; i++) {
            *padin[i] = in[i];
        }
    }

    void setupPadout(void);
    void setupPaddir(void);
    void setupIntstatus();
    sc_core::sc_event intChanged;
    std::array<scc::bitfield<uint32_t>*, 32> inten;
    std::array<scc::bitfield<uint32_t>*, 32> inttype;
    scc::bitfield<uint32_t>* intstatus;
    void intstatus_cb();
};

// GPIO<> methods definition:

template <class owner_t>
inline GPIO<owner_t>::GPIO(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner)
: bfs_t(name, std::move(params), owner) {
    SC_METHOD(in_cb);
    for(size_t i = 0; i < 32; i++) {
        sc_core::sc_module::sensitive << in[i];
        padin[i] = &bfs_t::regs->getBitfield("PADIN", std::to_string(i), "gpio.pads_" + std::to_string(i) + ".in");
    }
    setupPadout();
    setupPaddir();
    setupIntstatus();
}

template <class owner_t> void GPIO<owner_t>::setupPadout(void) {
    for(size_t i = 0; i < 32; i++) {
        sc_core::sc_out<bool>& signal = out[i];
        auto& bf = bfs_t::regs->getBitfield("PADOUT", std::to_string(i), "gpio.pads_" + std::to_string(i) + ".out");
        bf.setWriteCallback([&](auto&, uint32_t& value) { signal = value; });
    }
}

template <class owner_t> void GPIO<owner_t>::setupPaddir(void) {
    for(size_t i = 0; i < 32; i++) {
        auto& bf = bfs_t::regs->getBitfield("PADDIR", std::to_string(i), "gpio.pads_" + std::to_string(i) + ".dir");
        sc_core::sc_out<bool>& signal = dir[i];
        bf.setWriteCallback([&](auto&, uint32_t& value) { signal = value; });
    }
}

template <class owner_t> void GPIO<owner_t>::setupIntstatus(void) {
    intstatus = &bfs_t::regs->getBitfield("INTSTATUS", "INTSTATUS", "gpio.intStatus");
    intstatus->setReadCallback([this](auto&) {
        uint32_t result = *intstatus;
        *intstatus = 0;
        (*bfs_t::irq_out_)[0] = 0;
        return result;
    });
    for(size_t i = 0; i < 32; i++) {
        inten[i] = &bfs_t::regs->getBitfield("INTEN", std::to_string(i), "gpio.pads_" + std::to_string(i) + ".intEn");
        inten[i]->setWriteCallback([this](auto&, auto) { intChanged.notify(); });
        inttype[i] = &bfs_t::regs->getBitfield("INTTYPE", std::to_string(i), "gpio.pads_" + std::to_string(i) + ".intType");
        inttype[i]->setWriteCallback([this](auto&, auto) { intChanged.notify(); });
    }

    SC_METHOD(intstatus_cb);
    for(size_t i = 0; i < 32; i++) {
        sc_core::sc_module::sensitive << in[i];
    }
    sc_core::sc_module::sensitive << intChanged;
}

template <class owner_t> void GPIO<owner_t>::intstatus_cb(void) {
    for(size_t i = 0; i < 32; i++) {
        bool isInterrupt = false;
        if(*inttype[i] == IntType::Level1)
            isInterrupt = in[i].negedge(); // TODO
        else if(*inttype[i] == IntType::Level0)
            isInterrupt = in[i].posedge(); // TODO
        else if(*inttype[i] == IntType::Rise)
            isInterrupt = in[i].posedge();
        else if(*inttype[i] == IntType::Fall)
            isInterrupt = in[i].negedge();

        bool signalInterrupt = isInterrupt && *inten[i];
        *intstatus = *intstatus | (signalInterrupt << i);
    }
    // TODO: use PripheralBase::bindIRQ?
    (*bfs_t::irq_out_)[0] = *intstatus;
}

} // namespace pulpino
} // namespace vpvper

#endif // __VPVPER_PULPINO_GPIO_H__
