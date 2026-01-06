/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_PLIC_H__
#define __VPVPER_PULPINO_PLIC_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

#include <queue>

#ifdef DEBUG_BUILD
#include <iomanip>
#include <iostream>
#endif

namespace vpvper {
namespace pulpino {

/////////////////////////////////////////////////////////////////////////////
/// \class PlicRegs
/// \brief static plic registers (32 sources, 4 targets)
class PlicRegs : public scc::tlm_target_bfs_register_base<PlicRegs> {
public:
    std::array<scc::bitfield_register<uint32_t>, 32 + 1 + 3 * 4> registers{{
        {"PRIO_SOURCE0", 0x0000},        // RESERVED, i.e. Priority interrupt source 0 does not exist. No source, or source 0
                                         // ID, means no interrupt
        {"PRIO_SOURCE1", 0x0000 + 0x04}, // Priority Interrupt source 1
        {"PRIO_SOURCE2", 0x0000 + 0x08}, // Priority Interrupt source 2
        {"PRIO_SOURCE3", 0x0000 + 0x0C}, // Priority Interrupt source 3
        {"PRIO_SOURCE4", 0x0000 + 0x10}, // Priority Interrupt source 4
        {"PRIO_SOURCE5", 0x0000 + 0x14}, // Priority Interrupt source 5
        {"PRIO_SOURCE6", 0x0000 + 0x18}, // Priority Interrupt source 6
        {"PRIO_SOURCE7", 0x0000 + 0x1C}, // Priority Interrupt source 7
        {"PRIO_SOURCE8", 0x0000 + 0x20}, // ...
        {"PRIO_SOURCE9", 0x0000 + 0x24},
        {"PRIO_SOURCE10", 0x0000 + 0x28},
        {"PRIO_SOURCE11", 0x0000 + 0x2C},
        {"PRIO_SOURCE12", 0x0000 + 0x30},
        {"PRIO_SOURCE13", 0x0000 + 0x34},
        {"PRIO_SOURCE14", 0x0000 + 0x38},
        {"PRIO_SOURCE15", 0x0000 + 0x3C},
        {"PRIO_SOURCE16", 0x0000 + 0x40},
        {"PRIO_SOURCE17", 0x0000 + 0x44},
        {"PRIO_SOURCE18", 0x0000 + 0x48},
        {"PRIO_SOURCE19", 0x0000 + 0x4C},
        {"PRIO_SOURCE20", 0x0000 + 0x50},
        {"PRIO_SOURCE21", 0x0000 + 0x54},
        {"PRIO_SOURCE22", 0x0000 + 0x58},
        {"PRIO_SOURCE23", 0x0000 + 0x5C},
        {"PRIO_SOURCE24", 0x0000 + 0x60},
        {"PRIO_SOURCE25", 0x0000 + 0x64},
        {"PRIO_SOURCE26", 0x0000 + 0x68},
        {"PRIO_SOURCE27", 0x0000 + 0x6C},
        {"PRIO_SOURCE28", 0x0000 + 0x70},
        {"PRIO_SOURCE29", 0x0000 + 0x74},
        {"PRIO_SOURCE30", 0x0000 + 0x78},
        {"PRIO_SOURCE31", 0x0000 + 0x7C},
        /* No more interrupt sources supported Readonly, yet WARL*/
        {"IP_SOURCE31-0", 0x1000}, // Interrupt pending for sources 31 downto 0
        /* Context-specific interrupt enable bits. Context means, hart context, i.e. a hart's priviledge level. For
           Ariane we have 1 hart and 2 priviledge levels (S)upervised (M)achine, giving us 2 contexts, where context 0
           (ctx0) is hart0:M and context 1 (ctx1) is hart0:S. This reflects ariane's interrupt notification (irq[0:1]
           wires to core), where  irq[0] maps to machine irq and irq[1] to supervisor irq (see decoder stage).*/
        {"IE_SOURCE31-0_CTX0", 0x2000}, // Interrupt enable for sources 31 downto 0 for context 0
        {"IE_SOURCE31-0_CTX1", 0x2080}, // Interrupt enable for sources 31 downto 0 for context 1
        {"IE_SOURCE31-0_CTX2", 0x2100}, // Interrupt enable for sources 31 downto 0 for context 2
        {"IE_SOURCE31-0_CTX3", 0x2180}, // Interrupt enable for sources 31 downto 0 for context 3
        /* Context-specific priority threshold: The Plic will permit all interrupts which have a priority of less or
           equal than the value. To the respective context */
        {"PRIOTH31-0_CTX0", 0x200000}, // Interrupt priority threshold for context 0
        {"PRIOTH31-0_CTX1", 0x201000}, // Interrupt priority threshold for context 1
        {"PRIOTH31-0_CTX2", 0x202000}, // Interrupt priority threshold for context 2
        {"PRIOTH31-0_CTX3", 0x203000}, // Interrupt priority threshold for context 3
        /* Context-specific interrupt claim/complete process registers. A read to this register claims and Plic should
           provide the associated interrupt source's ID. */
        {"CLAIM-COMPLETE_CTX0", 0x200004}, // Interrupt claim/complete for context 0.
        {"CLAIM-COMPLETE_CTX1", 0x201004}, // Interrupt claim/complete for context 1.
        {"CLAIM-COMPLETE_CTX2", 0x202004}, // Interrupt claim/complete for context 2.
        {"CLAIM-COMPLETE_CTX3", 0x203004}  // Interrupt claim/complete for context 3.
    }};                                    ///< Plic registers

    std::array<scc::bitfield<uint32_t>, 32 + 1 + 3 * 4> bitfields{{
        {getRegister("IP_SOURCE31-0"), "IP_SOURCE31-0", 0, 32, "plic.ip31-0"},

        {getRegister("IE_SOURCE31-0_CTX0"), "IE_CTX0", 0, 32, "plic.ctx0.ie"},
        {getRegister("IE_SOURCE31-0_CTX1"), "IE_CTX1", 0, 32, "plic.ctx1.ie"},
        {getRegister("IE_SOURCE31-0_CTX2"), "IE_CTX2", 0, 32, "plic.ctx2.ie"},
        {getRegister("IE_SOURCE31-0_CTX3"), "IE_CTX3", 0, 32, "plic.ctx3.ie"},

        {getRegister("PRIOTH31-0_CTX0"), "PRIOTH_CTX0", 0, 32, "plic.ctx0.pth"},
        {getRegister("PRIOTH31-0_CTX1"), "PRIOTH_CTX1", 0, 32, "plic.ctx1.pth"},
        {getRegister("PRIOTH31-0_CTX2"), "PRIOTH_CTX2", 0, 32, "plic.ctx2.pth"},
        {getRegister("PRIOTH31-0_CTX3"), "PRIOTH_CTX3", 0, 32, "plic.ctx3.pth"},

        {getRegister("PRIO_SOURCE0"), "PRIO0", 0, 32, "plic.prio0"},
        {getRegister("PRIO_SOURCE1"), "PRIO1", 0, 32, "plic.prio1"},
        {getRegister("PRIO_SOURCE2"), "PRIO2", 0, 32, "plic.prio2"},
        {getRegister("PRIO_SOURCE3"), "PRIO3", 0, 32, "plic.prio3"},
        {getRegister("PRIO_SOURCE4"), "PRIO4", 0, 32, "plic.prio4"},
        {getRegister("PRIO_SOURCE5"), "PRIO5", 0, 32, "plic.prio5"},
        {getRegister("PRIO_SOURCE6"), "PRIO6", 0, 32, "plic.prio6"},
        {getRegister("PRIO_SOURCE7"), "PRIO7", 0, 32, "plic.prio7"},
        {getRegister("PRIO_SOURCE8"), "PRIO8", 0, 32, "plic.prio8"},
        {getRegister("PRIO_SOURCE9"), "PRIO9", 0, 32, "plic.prio9"},
        {getRegister("PRIO_SOURCE10"), "PRIO10", 0, 32, "plic.prio10"},
        {getRegister("PRIO_SOURCE11"), "PRIO11", 0, 32, "plic.prio11"},
        {getRegister("PRIO_SOURCE12"), "PRIO12", 0, 32, "plic.prio12"},
        {getRegister("PRIO_SOURCE13"), "PRIO13", 0, 32, "plic.prio13"},
        {getRegister("PRIO_SOURCE14"), "PRIO14", 0, 32, "plic.prio14"},
        {getRegister("PRIO_SOURCE15"), "PRIO15", 0, 32, "plic.prio15"},
        {getRegister("PRIO_SOURCE16"), "PRIO16", 0, 32, "plic.prio16"},
        {getRegister("PRIO_SOURCE17"), "PRIO17", 0, 32, "plic.prio17"},
        {getRegister("PRIO_SOURCE18"), "PRIO18", 0, 32, "plic.prio18"},
        {getRegister("PRIO_SOURCE19"), "PRIO19", 0, 32, "plic.prio19"},
        {getRegister("PRIO_SOURCE20"), "PRIO20", 0, 32, "plic.prio20"},
        {getRegister("PRIO_SOURCE21"), "PRIO21", 0, 32, "plic.prio21"},
        {getRegister("PRIO_SOURCE22"), "PRIO22", 0, 32, "plic.prio22"},
        {getRegister("PRIO_SOURCE23"), "PRIO23", 0, 32, "plic.prio23"},
        {getRegister("PRIO_SOURCE24"), "PRIO24", 0, 32, "plic.prio24"},
        {getRegister("PRIO_SOURCE25"), "PRIO25", 0, 32, "plic.prio25"},
        {getRegister("PRIO_SOURCE26"), "PRIO26", 0, 32, "plic.prio26"},
        {getRegister("PRIO_SOURCE27"), "PRIO27", 0, 32, "plic.prio27"},
        {getRegister("PRIO_SOURCE28"), "PRIO28", 0, 32, "plic.prio28"},
        {getRegister("PRIO_SOURCE29"), "PRIO29", 0, 32, "plic.prio29"},
        {getRegister("PRIO_SOURCE30"), "PRIO30", 0, 32, "plic.prio30"},
        {getRegister("PRIO_SOURCE31"), "PRIO31", 0, 32, "plic.prio31"},

        {getRegister("CLAIM-COMPLETE_CTX0"), "CC_CTX0", 0, 32, "plic.ctx0.cc"},
        {getRegister("CLAIM-COMPLETE_CTX1"), "CC_CTX1", 0, 32, "plic.ctx1.cc"},
        {getRegister("CLAIM-COMPLETE_CTX2"), "CC_CTX2", 0, 32, "plic.ctx2.cc"},
        {getRegister("CLAIM-COMPLETE_CTX3"), "CC_CTX3", 0, 32, "plic.ctx3.cc"},
    }}; ///< Plic register bitfields

    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    PlicRegs(sc_core::sc_module_name name)
    : scc::tlm_target_bfs_register_base<PlicRegs>{name} {}
};

////////////////////////////////////////////////////////////////////////////////
///\class Plic
///\brief Platform level interrupt controller
///\details only up to 32 sources [31:0] for 4 [3:0] targets supported. Limited due to scc::tlm_target_bfs_register_base
/// not supporting dynamic construction of registers (move=)
template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK = 0,
          size_t N_LINEREGS = (N_SOURCE >> 5) + ((N_SOURCE % 32) ? 1 : 0)>
class Plic : public scc::tlm_target_bfs<PlicRegs, owner_t> {
    SC_HAS_PROCESS(Plic);
    typedef scc::tlm_target_bfs<PlicRegs, owner_t> bfs_t;
    constexpr static const char* ID_PLIC{"plic"};

protected:
    ////////////////////////////////////////////////////////////////////////////////
    ///\class Plic_gateway
    ///\brief Platform level interrupt controller gateway module
    class Plic_gateway {
    private:
        virtual void source(const uint32_t srcs_flags);

    public:
        uint32_t set_{0};
        ///< calculated level/edge senstive input sources, a '1' bit signals a new pending interrupt line state
        uint32_t ia_{0};
        ///< calculated active (already registered by gateway) interrupt input lines
        const uint32_t le_cfg_;
        ///< level (0) / edge (1) sensitive interrupt input mask
        scc::bitfield<uint32_t>& ip_;
        ///< pending interrupt register (from Plic)

    public:
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief source process
        virtual void source(std::array<sc_core::sc_in<bool>, N_SOURCE>& srcs_i);
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief claim process
        void claim(const uint32_t claim_flags);
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief complete process
        void complete(const uint32_t complete_flags);
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Reset method
        void reset(void) {
            set_ = 0;
            ia_ = 0;
        }
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Constructor
        Plic_gateway(scc::bitfield<uint32_t>& ip, const uint32_t le_cfg = 0x00000000)
        : le_cfg_(le_cfg)
        , ip_(ip) {}

        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Destructor
        virtual ~Plic_gateway() {}
    };

    ////////////////////////////////////////////////////////////////////////////////
    ///\class Plic_target
    ///\brief Platform level interrupt controller Target module
    class Plic_target {
    protected:
        uint32_t id_{0};
        ///< id of currently active interrupt (0) is no active interrupt since id=0 is not valid
        scc::bitfield<uint32_t>& ip_;
        ///< pending interrupts
        scc::bitfield<uint32_t>& ie_;
        ///< interrupt enable
        scc::bitfield<uint32_t>& pth_;
        ///< interrupt priority threshold
        std::array<scc::bitfield<uint32_t>*, N_SOURCE>& prios_;
        ///< input interrupt priorities

    public:
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Get Id of active interrupt line
        uint32_t get_activeId(void) const { return (id_); }
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief process current state and return whether an interrupt has to be signaled to target
        ///\details can called in a synchronous (clocked) or an event-based scan method
        bool process(void);
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Reset method
        void reset(void) {}
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Constructor
        Plic_target(scc::bitfield<uint32_t>& ip, scc::bitfield<uint32_t>& ie, scc::bitfield<uint32_t>& pth,
                    std::array<scc::bitfield<uint32_t>*, N_SOURCE>& prios)
        : ip_(ip)
        , ie_(ie)
        , pth_(pth)
        , prios_(prios) {}
        ////////////////////////////////////////////////////////////////////////////////
        ///\brief Destructor
        virtual ~Plic_target() {}
    };

public:
    std::unique_ptr<Plic_gateway> gw_;
    ///< gateway: scans input lines and sets pendings
    std::array<std::unique_ptr<Plic_target>, N_TARGET> targets_;
    ///< target: sorts pendings by prio and routes to context, e.g. hart0:M
    std::array<scc::bitfield<uint32_t>*, N_LINEREGS> ips_{};
    ///< input interrupt pending register fields
    std::array<scc::bitfield<uint32_t>*, N_TARGET> ies_{};
    ///< input interrupt enable register fields (for each context one field)
    std::array<scc::bitfield<uint32_t>*, N_TARGET> pths_{};
    ///< interrupt priority threshold register field (for each context one field)
    std::array<scc::bitfield<uint32_t>*, N_SOURCE> prios_{};
    ///< interrupt sources priority register field (for each source one 32-bit prio)
    std::array<scc::bitfield<uint32_t>*, N_TARGET> ccs_{};
    ///< claim/complete register. A read to this register returns the id of the (highest prio) interrupt. A write with
    ///< enabled bits resets (=completes) the respective pending interrupts more than the active interrupt can be
    ///< completed with one write (for each context one field)
    std::array<sc_core::sc_in<bool>, N_SOURCE> irq_srcs_i_{};
    ///< input interrupt source lines
public:
    ////////////////////////////////////////////////////////////////////////////////
    ///\brief Constructor
    Plic(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner = nullptr);
    ////////////////////////////////////////////////////////////////////////////////
    ///\brief Destructor
    virtual ~Plic(void) {}
    ////////////////////////////////////////////////////////////////////////////////
    ///\brief reset method
    void reset(void) {}
    ////////////////////////////////////////////////////////////////////////////////
    ///\brief Scan interrupt lines and invoke gateway and target processors
    ///\details is sensitive to interrupt source input lines
    void scan_int_lines(void);
};

/////////////////////////////////////////////////////////////////////////////
// Plic<> methods definition:

template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
inline Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params,
                                                                        owner_t* owner)
: bfs_t(name, std::move(params), owner) {
    reset();

    if(N_SOURCE > 31) {
        SC_REPORT_ERROR(ID_PLIC, "PLIC config: Maximum # of SOURCES exceeded");
    }
    if(N_TARGET > 4) {
        SC_REPORT_ERROR(ID_PLIC, "PLIC config: Maximum # of TARGETS exceeded");
    }

    for(size_t i = 0; i < N_LINEREGS; ++i) {
        std::string _regname = (std::string("IP_SOURCE")) + std::to_string(i * 32 + 31) + std::string("-") + std::to_string(i * 32);
        // std::string _name 		= (std::string("PRIO")) + std::to_string(i);
        std::string _urid = (std::string("plic.ip")) + std::to_string(i * 32 + 31) + std::string("-") + std::to_string(i * 32);
        ips_[i] = &(bfs_t::regs->getBitfield(_regname, _regname, _urid));
    }
    for(size_t i = 1; i < N_SOURCE; ++i) {
        std::string _regname = (std::string("PRIO_SOURCE")) + std::to_string(i);
        std::string _name = (std::string("PRIO")) + std::to_string(i);
        std::string _urid = (std::string("plic.prio")) + std::to_string(i);
        prios_[i] = &(bfs_t::regs->getBitfield(_regname, _name, _urid));
    }
    for(size_t i = 0; i < N_TARGET; ++i) {
        std::string _regname = (std::string("IE_SOURCE31-0_CTX")) + std::to_string(i);
        std::string _name = (std::string("IE_CTX")) + std::to_string(i);
        std::string _urid = (std::string("plic.ctx")) + std::to_string(i) + std::string(".ie");
        ies_[i] = &(bfs_t::regs->getBitfield(_regname, _name, _urid));
    }
    for(size_t i = 0; i < N_TARGET; ++i) {
        std::string _regname = (std::string("PRIOTH31-0_CTX")) + std::to_string(i);
        std::string _name = (std::string("PRIOTH_CTX")) + std::to_string(i);
        std::string _urid = (std::string("plic.ctx")) + std::to_string(i) + std::string(".pth");
        pths_[i] = &(bfs_t::regs->getBitfield(_regname, _name, _urid));
    }
    for(size_t i = 0; i < N_TARGET; ++i) {
        std::string _regname = (std::string("CLAIM-COMPLETE_CTX")) + std::to_string(i);
        std::string _name = (std::string("CC_CTX")) + std::to_string(i);
        std::string _urid = (std::string("plic.ctx")) + std::to_string(i) + std::string(".cc");
        ccs_[i] = &(bfs_t::regs->getBitfield(_regname, _name, _urid));
    }

    gw_ = std::make_unique<Plic_gateway>(*(ips_[0]), LE_SRC_MASK);

    for(size_t i = 0; i < N_TARGET; ++i) {
        targets_[i] = std::make_unique<Plic_target>(*(ips_[0]), *(ies_[i]), *(pths_[i]), prios_);
    }

    for(size_t i = 1; i < N_SOURCE; ++i) {
        prios_[i]->setWriteCallback([this, i](auto&&, uint32_t& valueToWrite) {
            *(prios_[i]) = valueToWrite;
            SC_REPORT_INFO(ID_PLIC, "Plic:prio new cfg");
        });
        prios_[i]->setReadCallback([this, i](auto&&) { return (prios_[i]->get()); });
    }
    // -------------------------------------------------------------------------------
    for(size_t i = 0; i < N_LINEREGS; ++i) {
        ips_[i]->setWriteCallback([this, i](auto&&, uint32_t& valueToWrite) { *(ips_[i]) = valueToWrite; });
        ips_[i]->setReadCallback([this, i](auto&&) { return (ips_[i]->get()); });
    }
    // -------------------------------------------------------------------------------
    for(size_t i = 0; i < N_TARGET; ++i) {
        ies_[i]->setWriteCallback([this, i](auto&&, uint32_t& valueToWrite) {
            static std::string MSG = (std::string("Plic:ctx") + std::to_string(i) + std::string(" interrupt enable config"));
            *(ies_[i]) = valueToWrite;
            SC_REPORT_INFO(ID_PLIC, MSG.c_str());
        });
        ies_[i]->setReadCallback([this, i](auto&&) { return (ies_[i]->get()); });
    }
    // -------------------------------------------------------------------------------
    for(size_t i = 0; i < N_TARGET; ++i) {
        pths_[i]->setWriteCallback([this, i](auto&&, uint32_t& valueToWrite) {
            static std::string MSG = (std::string("Plic:ctx") + std::to_string(i) + std::string(" priority threshold config"));
            *(pths_[i]) = valueToWrite;
            SC_REPORT_INFO(ID_PLIC, MSG.c_str());
        });
        pths_[i]->setReadCallback([this, i](auto&&) { return (pths_[i]->get()); });
    }
    // -------------------------------------------------------------------------------
    for(size_t i = 0; i < N_TARGET; ++i) {
        ccs_[i]->setWriteCallback([this, i](auto&&, uint32_t& valueToWrite) {
            static std::string MSG = (std::string("Plic:ctx") + std::to_string(i) + std::string(" complete"));
            *(ccs_[i]) = valueToWrite;   // should only write one?
            gw_->complete(valueToWrite); // all flags, as irq handler can clear waiting source interrupts even though
                                         // they were never messaged
            SC_REPORT_INFO(ID_PLIC, MSG.c_str());
        });
        ccs_[i]->setReadCallback([this, i](auto&&) {
            static std::string MSG = (std::string("Plic:ctx") + std::to_string(i) + std::string(" claim"));
            uint32_t x = targets_[i]->get_activeId();
            if(x) {
                gw_->claim(0x1 << x);
            }
            return (x);
            SC_REPORT_INFO(ID_PLIC, MSG.c_str());
        });
    }

    SC_METHOD(scan_int_lines);
    for(auto& irq : irq_srcs_i_)
        bfs_t::sensitive << irq;
}

template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
void Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::scan_int_lines(void) {

#ifdef DEBUG_BUILD
    std::cout << "\nirq_in: ";
    for(size_t i = 0; i < N_SOURCE; ++i)
        std::cout << irq_srcs_i_[i].read();

    std::cout << "\n(0)" << std::hex << "set:" << gw_->set_ << "|ia:" << gw_->ia_ << "|ip:" << gw_->ip_.get() << std::endl;
#endif

    /* scan input lines */
    gw_->source(irq_srcs_i_);
#ifdef DEBUG_BUILD
    std::cout << "(1)" << std::hex << "set:" << gw_->set_ << "|ia:" << gw_->ia_ << "|ip:" << gw_->ip_.get() << std::endl;
#endif

    /* make an empty claim: this calculates pending interrupts */
    gw_->claim(0);
#ifdef DEBUG_BUILD
    std::cout << "(2)" << std::hex << "set:" << gw_->set_ << "|ia:" << gw_->ia_ << "|ip:" << gw_->ip_.get() << std::endl;
#endif

    for(size_t i = 0; i < N_TARGET; ++i) {
        if(targets_[i]->process())
            (*bfs_t::irq_out_)[i] = 1;
        else {
            (*bfs_t::irq_out_)[i] = 0;
        }
    }

    /* make an empty complete: this sets active interrupts, thus blocks unclaimed pending interrupts to be fired again*/
    gw_->complete(0);
#ifdef DEBUG_BUILD
    std::cout << "(3)" << std::hex << "set:" << gw_->set_ << "|ia:" << gw_->ia_ << "|ip:" << gw_->ip_.get() << std::endl;
#endif
}

// Plic<>::Plic_target methods definition:
template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
bool Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic_target::process(void) {
    uint32_t max_prio = pth_ + 1;
    bool irq_next = 0;
    for(size_t i = 1; i < N_SOURCE; ++i) {
        if(((ip_ & ie_) & (0x1 << i)) && *(prios_[i]) >= max_prio) {
            max_prio = *(prios_[i]);
            id_ = i; // src=0 reserved
            irq_next = 1;
        }
    }
    return (irq_next);
}

// Plic<>::Plic_gateway methods definition:
template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
void Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic_gateway::source(const uint32_t srcs_flags) {
    static uint32_t srcs_d = 0;
    set_ = 0;
    for(auto i = 1; i < N_SOURCE; ++i) {
        uint32_t mask = 0x1 << i;
        if(le_cfg_ & mask) { // source is edge sensitive
            set_ |= (srcs_flags & mask) ? ((srcs_d & mask) == 0) << i : 0;
        } else { // level sensitive
            set_ |= (srcs_flags)&mask;
        }
    }
    srcs_d = srcs_flags;
}

template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
void Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic_gateway::source(std::array<sc_core::sc_in<bool>, N_SOURCE>& srcs_i) {
    uint32_t _srcs_flags = 0;
    for(auto i = 0; i < srcs_i.size(); ++i) {
        _srcs_flags |= ((srcs_i[i].read() & 0x1) << i);
    }
    source(_srcs_flags);
}

template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
inline void Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic_gateway::claim(const uint32_t claim_flags) {
    // Verilog: ip <= (ip | (set & ~ia & ~ip)) & (~claim);
    uint32_t _ip = ip_;
    _ip = (_ip | (set_ & ~ia_ & ~_ip)) & (~claim_flags);
    ip_ = _ip;
}

template <typename owner_t, size_t N_TARGET, size_t N_SOURCE, uint32_t LE_SRC_MASK, size_t N_LINEREGS>
inline void Plic<owner_t, N_TARGET, N_SOURCE, LE_SRC_MASK, N_LINEREGS>::Plic_gateway::complete(const uint32_t complete_flags) {
    // Verilog: ia <= (ia | (set & ~ia)) & (~complete);
    uint32_t _ia = ia_;
    _ia = (_ia | (set_ & ~_ia)) & (~complete_flags);
    ia_ = _ia;
}

} // namespace pulpino
} // namespace vpvper

#endif //__VPVPER_PULPINO_PLIC_H__
