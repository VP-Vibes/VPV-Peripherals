/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_CLINT_H__
#define __VPVPER_PULPINO_CLINT_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

#include <queue>

namespace vpvper {
namespace pulpino {

/////////////////////////////////////////////////////////////////////////////
/// \class ClintRegs
class ClintRegs : public scc::tlm_target_bfs_register_base<ClintRegs> {
public:
    std::array<scc::bitfield_register<uint32_t>, 5> registers{{
        {"MSIP", 0x0000},             // Interprocess Interrupt Pending
        {"MTIMECMPLO", 0x4000},       // Timer compare register low word  ( 31:0 )
        {"MTIMECMPHI", (0x4000 + 4)}, // Timer compare register high word ( 63:32 )
        {"MTIMELO", 0xBFF8},          // Timer (count) register low word  ( 31:0 )
        {"MTIMEHI", (0xBFF8 + 4)}     // Timer (count) register high word ( 63:32 )
    }};                               ///< Clint registers

    std::array<scc::bitfield<uint32_t>, 5> bitfields{
        {{getRegister("MSIP"), "MSIP", 0, 1, "clint.MSIP"},
         {getRegister("MTIMECMPLO"), "MTIMECMPLO", 0, 32, "clint.MTIMECMPLO"},
         {getRegister("MTIMECMPHI"), "MTIMECMPHI", 0, 32, "clint.MTIMECMPHI"},
         {getRegister("MTIMELO"), "MTIMELO", 0, 32, "clint.MTIMELO"},
         {getRegister("MTIMEHI"), "MTIMEHI", 0, 32, "clint.MTIMEHI"}}}; ///< Clint register bitfields

    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    ClintRegs(sc_core::sc_module_name name)
    : tlm_target_bfs_register_base<ClintRegs>{name} {};
};

/////////////////////////////////////////////////////////////////////////////
/// \class Clint
/// \brief RISC-V Core-local Interrupt Controller (CLINT).
/// \details A RISC-V privilege spec. 1.11 compatible CLINT (Adapted from Pulp Ariane CLINT:
/// https://github.com/pulp-platform/ariane/tree/master/src/clint)
template <typename owner_t> class Clint : public scc::tlm_target_bfs<ClintRegs, owner_t> {
    SC_HAS_PROCESS(Clint);
    typedef scc::tlm_target_bfs<ClintRegs, owner_t> bfs_t;
    constexpr static const char* ID_CLINT{"clint"};
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Event codes used to signal real-time clock process
    typedef enum RTC_EVENTS {
        UPDATE_TIME,
        RESET_INT,
        SET_INT,
    } rtc_events_t;

public:
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Clint(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner = nullptr,
          uint64_t rtcPeriod_ps = 30517578 /*f~32.768kHz*/);
    ////////////////////////////////////////////////////////////////////////////////
    ///\brief Destructor
    virtual ~Clint(void) {}

protected:
    scc::bitfield<uint32_t>& msip_{bfs_t::regs->getBitfield("MSIP", "MSIP", "clint.MSIP")};
    scc::bitfield<uint32_t>& mtimecmplo_{bfs_t::regs->getBitfield("MTIMECMPLO", "MTIMECMPLO", "clint.MTIMECMPLO")};
    scc::bitfield<uint32_t>& mtimecmphi_{bfs_t::regs->getBitfield("MTIMECMPHI", "MTIMECMPHI", "clint.MTIMECMPHI")};
    scc::bitfield<uint32_t>& mtimelo_{bfs_t::regs->getBitfield("MTIMELO", "MTIMELO", "clint.MTIMELO")};
    scc::bitfield<uint32_t>& mtimehi_{bfs_t::regs->getBitfield("MTIMEHI", "MTIMEHI", "clint.MTIMEHI")};

    const sc_core::sc_time rtcPeriod_ps_;
    ///< systemc time for real-time clock
    sc_core::sc_event rtc_event_;
    ///< systemc timer event to remove clock based processes
    std::queue<rtc_events_t> rtc_event_q_;
    ///< event queue of rtc_events_t for event based process rtc_process()
    sc_core::sc_time sct_wait_entry_{0, sc_core::SC_PS};
    ///< contains the systemc timestamp of the last call before sc_wait(forever) in rtc_process, used to calculate next
    ///< events.

private:
    uint64_t get_act_mtime(const sc_core::sc_time& passed_time);
    void reset();
    void rtc_process();
};

/////////////////////////////////////////////////////////////////////////////
// Clint<> methods definition:

template <class owner_t>
inline Clint<owner_t>::Clint(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner,
                             uint64_t rtcPeriod_ps)
: bfs_t(name, std::move(params), owner)
, rtcPeriod_ps_(sc_core::sc_time(rtcPeriod_ps, sc_core::SC_PS)) {
    reset();

    msip_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        msip_ = valueToWrite;
        if(msip_ & 0x1) {
            (*bfs_t::irq_out_)[0] = 1;
        } else {
            (*bfs_t::irq_out_)[0] = 0;
        }
        SC_REPORT_INFO(ID_CLINT, "New MSIP (IPI) Set");
    });

    mtimecmplo_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        mtimecmplo_ = valueToWrite;
        SC_REPORT_INFO(ID_CLINT, "New Compare Value Set (low word)");
        uint64_t tmp1 = 0, tmp2 = 0;
        tmp1 = uint64_t(mtimelo_) | (uint64_t(mtimehi_) << 32);
        tmp2 = uint64_t(mtimecmplo_) | (uint64_t(mtimecmphi_) << 32);
        if(tmp1 >= tmp2) {
            rtc_event_q_.push(SET_INT);
        } else {
            rtc_event_q_.push(RESET_INT);
        }
        rtc_event_q_.push(UPDATE_TIME);
        rtc_event_.notify();
    });

    mtimecmphi_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        mtimecmphi_ = valueToWrite;
        SC_REPORT_INFO(ID_CLINT, "New Compare Value Set (high word)");
        uint64_t tmp1 = 0, tmp2 = 0;
        tmp1 = uint64_t(mtimelo_) | (uint64_t(mtimehi_) << 32);
        tmp2 = uint64_t(mtimecmplo_) | (uint64_t(mtimecmphi_) << 32);
        if(tmp1 >= tmp2) {
            rtc_event_q_.push(SET_INT);
        } else {
            rtc_event_q_.push(RESET_INT);
        }
        rtc_event_q_.push(UPDATE_TIME);
        rtc_event_.notify();
    });

    mtimelo_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        mtimelo_ = valueToWrite;
        SC_REPORT_INFO(ID_CLINT, "New Timer Value Set (low word)");
        uint64_t tmp1 = 0, tmp2 = 0;
        tmp1 = uint64_t(mtimelo_) | (uint64_t(mtimehi_) << 32);
        tmp2 = uint64_t(mtimecmplo_) | (uint64_t(mtimecmphi_) << 32);
        if(tmp1 >= tmp2) {
            rtc_event_q_.push(SET_INT);
        } else {
            rtc_event_q_.push(RESET_INT);
        }
        rtc_event_q_.push(UPDATE_TIME);
        rtc_event_.notify();
    });

    mtimehi_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        mtimehi_ = valueToWrite;
        SC_REPORT_INFO(ID_CLINT, "New Timer Value Set (high word)");
        uint64_t tmp1 = 0, tmp2 = 0;
        tmp1 = uint64_t(mtimelo_) | (uint64_t(mtimehi_) << 32);
        tmp2 = uint64_t(mtimecmplo_) | (uint64_t(mtimecmphi_) << 32);
        if(tmp1 >= tmp2) {
            rtc_event_q_.push(SET_INT);
        } else {
            rtc_event_q_.push(RESET_INT);
        }
        rtc_event_q_.push(UPDATE_TIME);
        rtc_event_.notify();
    });

    mtimelo_.setReadCallback([this](auto&&) {
        sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_);
        return (get_act_mtime(_passed_time));
    });

    mtimehi_.setReadCallback([this](auto&&) {
        sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_);
        return ((get_act_mtime(_passed_time)) >> 32);
    });

    SC_THREAD(rtc_process);
}

template <class owner_t> void Clint<owner_t>::reset(void) {
    for(auto& bf : bfs_t::regs->bitfields) {
        bf = 0;
    }
    mtimecmplo_ = 0xFFFFFFFF;
    mtimecmphi_ = 0xFFFFFFFF;
}

template <class owner_t> void Clint<owner_t>::rtc_process(void) {
    static const uint64_t _OVERFLOW_ = 0xFFFFFFFFFFFFFFFF;
    sc_core::sc_time _sc_time_wait = _OVERFLOW_ * rtcPeriod_ps_;

    while(true) {
        sct_wait_entry_ = sc_core::sc_time_stamp();
        wait(_sc_time_wait, rtc_event_);

        uint64_t _ctr = uint64_t(mtimelo_) | (uint64_t(mtimehi_) << 32);
        uint64_t _cmp = uint64_t(mtimecmplo_) | (uint64_t(mtimecmphi_) << 32);

        if(rtc_event_q_.empty()) { // timer event ->
            // clean timer event -> make out whether compare or overflow
            if(_ctr < _cmp) {
                // compare
                mtimelo_ = uint64_t(mtimecmplo_);
                mtimehi_ = uint64_t(mtimecmphi_);
                (*bfs_t::irq_out_)[1] = 1;
                _sc_time_wait = (_OVERFLOW_ - _cmp) * rtcPeriod_ps_;
            } else {
                // overflow
                mtimelo_ = 0;
                mtimehi_ = 0;
                (*bfs_t::irq_out_)[1] = 0;
                _sc_time_wait = _cmp ? _cmp * rtcPeriod_ps_ : rtcPeriod_ps_;
            }
        } else {
            uint64_t _mtime = 0;
            sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_);

            while(!(rtc_event_q_.empty())) {
                rtc_events_t x = rtc_event_q_.front();
                switch(x) {
                case UPDATE_TIME:
                    _mtime = get_act_mtime(_passed_time);
                    mtimelo_ = _mtime;
                    mtimehi_ = _mtime >> 32;
                    if(_ctr >= _cmp)
                        _sc_time_wait = sc_core::sc_time(0, sc_core::SC_PS);
                    else
                        _sc_time_wait = (_cmp - _ctr) * rtcPeriod_ps_;
                    break;
                case RESET_INT:
                    (*bfs_t::irq_out_)[1] = 0;
                    break;
                case SET_INT:
                    (*bfs_t::irq_out_)[1] = 1;
                    break;
                default:
                    break;
                }
                rtc_event_q_.pop();
            }
        }
    }
}

template <class owner_t> uint64_t Clint<owner_t>::get_act_mtime(const sc_core::sc_time& passed_time) {
    uint64_t x = ((uint64_t(mtimehi_) << 32) | mtimelo_);
    x += static_cast<sc_dt::uint64>(passed_time / rtcPeriod_ps_);
    return (x);
}

} // namespace pulpino
} // namespace vpvper

#endif //__VPVPER_PULPINO_CLINT_H__
