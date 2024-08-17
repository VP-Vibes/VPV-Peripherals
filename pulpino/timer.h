/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_TIMER_H__
#define __VPVPER_PULPINO_TIMER_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

#include <queue>

namespace vpvper {
namespace pulpino {

/////////////////////////////////////////////////////////////////////////////
/// \class TimerRegs
class TimerRegs : public scc::tlm_target_bfs_register_base<TimerRegs> {
public:
    std::array<scc::bitfield_register<uint32_t>, 6> registers{{{"TIMERA", 0x00},      // Timer Value
                                                               {"TIMERA_CTRL", 0x04}, // Timer Control
                                                               {"TIMERA_CMP", 0x08},  // Timer Compare
                                                               {"TIMERB", 0x10},      // TimerB:
                                                               {"TIMERB_CTRL", 0x14},
                                                               {"TIMERB_CMP", 0x18}}}; ///< Timer registers
    std::array<scc::bitfield<uint32_t>, 8> bitfields{
        {{getRegister("TIMERA"), "TIMERA", 0, 32, "timer.TIMERA"},
         {getRegister("TIMERA_CTRL"), "ENABLE_A", 0, 1, "timer.ENA"},
         {getRegister("TIMERA_CTRL"), "PRESCALER_A", 3, 3, "timer.PREA"},
         {getRegister("TIMERA_CMP"), "COMPARE_A", 0, 32, "timer.CMPA"},
         {getRegister("TIMERB"), "TIMERB", 0, 32, "timer.TIMERB"},
         {getRegister("TIMERB_CTRL"), "ENABLE_B", 0, 1, "timer.ENB"},
         {getRegister("TIMERB_CTRL"), "PRESCALER_B", 3, 3, "timer.PREB"},
         {getRegister("TIMERB_CMP"), "COMPARE_B", 0, 32, "timer.CMPB"}}}; ///< Timer register bitfields
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    TimerRegs(sc_core::sc_module_name name)
    : tlm_target_bfs_register_base<TimerRegs>{name} {};
};

/////////////////////////////////////////////////////////////////////////////
/// \class Timer
/// \brief tlm_target_bfs based Timer peripheral. Losely follows Pulp platform APB Timer:
/// https://github.com/pulp-platform/apb_timer
template <typename owner_t> class Timer : public scc::tlm_target_bfs<TimerRegs, owner_t> {
    typedef scc::tlm_target_bfs<TimerRegs, owner_t> bfs_t;
    constexpr static const char* ID_TIMERA{"timer a"};
    constexpr static const char* ID_TIMERB{"timer b"};
    SC_HAS_PROCESS(Timer);
    /////////////////////////////////////////////////////////////////////////////
    /// \brief interrupt events for send interrupt processes
    typedef enum TIMER_EVENTS {
        UPDATE_TIME,
        RESET_INT,
        SET_INT,
    } timer_events_t;

public:
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    Timer(sc_core::sc_module_name, scc::tlm_target_bfs_params&&, owner_t* owner = nullptr,
          uint64_t cpu_period_ps = 1000000 /*f~1MHz*/);

protected:
    uint64_t cpu_period_ps_;
    ///< cpu period in pico seconds

    scc::bitfield<uint32_t>& timer_a_{bfs_t::regs->getBitfield("TIMERA", "TIMERA", "timer.TIMERA")};
    scc::bitfield<uint32_t>& en_a_{bfs_t::regs->getBitfield("TIMERA_CTRL", "ENABLE_A", "timer.ENA")};
    scc::bitfield<uint32_t>& pre_a_{bfs_t::regs->getBitfield("TIMERA_CTRL", "PRESCALER_A", "timer.PREA")};
    scc::bitfield<uint32_t>& cmp_a_{bfs_t::regs->getBitfield("TIMERA_CMP", "COMPARE_A", "timer.CMPA")};

    scc::bitfield<uint32_t>& timer_b_{bfs_t::regs->getBitfield("TIMERB", "TIMERB", "timer.TIMERB")};
    scc::bitfield<uint32_t>& en_b_{bfs_t::regs->getBitfield("TIMERB_CTRL", "ENABLE_B", "timer.ENB")};
    scc::bitfield<uint32_t>& pre_b_{bfs_t::regs->getBitfield("TIMERB_CTRL", "PRESCALER_B", "timer.PREB")};
    scc::bitfield<uint32_t>& cmp_b_{bfs_t::regs->getBitfield("TIMERB_CMP", "COMPARE_B", "timer.CMPB")};

    sc_core::sc_event a_event_;
    ///< systemc timer channel A event to remove clock based processes
    std::queue<timer_events_t> a_event_q_;
    ///< event queue of timer_events_t for event based process timer channel process A
    sc_core::sc_event b_event_;
    ///< systemc timer channel B event to remove clock based processes
    std::queue<timer_events_t> b_event_q_;
    ///< event queue of timer_events_t for event based process timer channel process B

    sc_core::sc_time sct_wait_entry_a_{0, sc_core::SC_PS};
    ///< contains the systemc timestamp of the last call before sc_wait(forever) in timer channel process A, used to
    ///< calculate next events.
    sc_core::sc_time sct_wait_entry_b_{0, sc_core::SC_PS};
    ///< contains the systemc timestamp of the last call before sc_wait(forever) in timer channel process B, used to
    ///< calculate next events.

private:
    sc_core::sc_time prescale_clk(scc::bitfield<uint32_t>&, sc_core::sc_time& clk_period);

    uint64_t get_act_time_a(const sc_core::sc_time& passed_time);
    uint64_t get_act_time_b(const sc_core::sc_time& passed_time);
    void reset(void);
    void timer_a_process(void);
    void timer_b_process(void);
};

/////////////////////////////////////////////////////////////////////////////
// Timer<> methods definition:

template <class owner_t>
inline Timer<owner_t>::Timer(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner,
                             uint64_t cpu_period_ps)
: bfs_t(name, std::move(params), owner)
, cpu_period_ps_(cpu_period_ps) {
    reset();

    timer_a_.setReadCallback([this](auto&&) {
        sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_a_);
        return (get_act_time_a(_passed_time));
    });

    timer_b_.setReadCallback([this](auto&&) {
        sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_b_);
        return (get_act_time_b(_passed_time));
    });

    cmp_a_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        cmp_a_ = valueToWrite;

        SC_REPORT_INFO(ID_TIMERA, "New Compare Value Set, Timer A set to 0");

        a_event_q_.push(UPDATE_TIME);
        a_event_.notify();
    });

    cmp_b_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        cmp_b_ = valueToWrite; // darf das compare Bitfield immer beschrieben werden?

        SC_REPORT_INFO(ID_TIMERB, "New Compare Value Set, Timer B set to 0");

        b_event_q_.push(UPDATE_TIME);
        b_event_.notify();
    });

    en_a_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        en_a_ = valueToWrite;

        SC_REPORT_INFO(ID_TIMERA, "New Compare Value Set, Timer A set to 0");
        a_event_q_.push(UPDATE_TIME);
        a_event_.notify();
    });

    en_b_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        en_b_ = valueToWrite;

        SC_REPORT_INFO(ID_TIMERB, "New Compare Value Set, Timer B set to 0");
        b_event_q_.push(UPDATE_TIME);
        b_event_.notify();
    });

    pre_a_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        pre_a_ = valueToWrite;

        SC_REPORT_INFO(ID_TIMERA, "New Compare Value Set, Timer A set to 0");
        a_event_q_.push(UPDATE_TIME);
        a_event_.notify();
    });

    pre_b_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        pre_b_ = valueToWrite;

        SC_REPORT_INFO(ID_TIMERB, "New Compare Value Set, Timer B set to 0");
        b_event_q_.push(UPDATE_TIME);
        b_event_.notify();
    });

    SC_THREAD(timer_a_process);
    SC_THREAD(timer_b_process);
}

template <class owner_t> void Timer<owner_t>::timer_a_process(void) {
    auto base_clk_period = sc_core::sc_time(static_cast<double>(cpu_period_ps_), sc_core::SC_PS);

    static const uint32_t _OVERFLOW_ = 0xFFFFFFFF;
    sc_core::sc_time _sc_time_wait =
        pre_a_.read() ? 0xFFFFFFFF * base_clk_period * (pre_a_ ? pre_a_ : 1) : 0xFFFFFFFF * base_clk_period;

    while(true) {
        sct_wait_entry_a_ = sc_core::sc_time_stamp();
        if(!en_a_) {
            wait(a_event_);
        } else {
            wait(_sc_time_wait, a_event_);
        }
        if(a_event_q_.empty()) { // timer event ->
            // clean timer event -> make out whether compare or overflow
            if(timer_a_ < cmp_a_) {
                // compare
                timer_a_ = cmp_a_.read();
                (*bfs_t::irq_out_)[1] = 1;
                _sc_time_wait = (_OVERFLOW_ - cmp_a_) * base_clk_period * pre_a_ - base_clk_period;
                wait(base_clk_period);
                (*bfs_t::irq_out_)[1] = 0;
            } else {
                // overflow
                timer_a_ = 0;
                (*bfs_t::irq_out_)[0] = 1;
                _sc_time_wait = cmp_a_.read() ? cmp_a_ * base_clk_period * (pre_a_ ? pre_a_ : 1) - base_clk_period
                                              : base_clk_period * (pre_a_ ? pre_a_ : 1);
                wait(base_clk_period);
                (*bfs_t::irq_out_)[0] = 0;
            }
        } else {
            sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_a_);
            while(!(a_event_q_.empty())) {
                timer_events_t x = a_event_q_.front();
                switch(x) {
                case UPDATE_TIME:
                    timer_a_ = get_act_time_a(_passed_time);
                    if(timer_a_ >= cmp_a_)
                        _sc_time_wait = sc_core::sc_time(0, sc_core::SC_PS);
                    else
                        _sc_time_wait = (cmp_a_ - timer_a_) * base_clk_period * (pre_a_ ? pre_a_ : 1);
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
                a_event_q_.pop();
            }
        }
    }
}

template <class owner_t> void Timer<owner_t>::timer_b_process(void) {
    auto base_clk_period = sc_core::sc_time(static_cast<double>(cpu_period_ps_), sc_core::SC_PS);

    static const uint32_t _OVERFLOW_ = 0xFFFFFFFF;
    sc_core::sc_time _sc_time_wait =
        pre_b_.read() ? 0xFFFFFFFF * base_clk_period * (pre_b_ ? pre_b_ : 1) : 0xFFFFFFFF * base_clk_period;

    while(true) {
        sct_wait_entry_b_ = sc_core::sc_time_stamp();
        if(!en_b_) {
            wait(b_event_);
        } else {
            wait(_sc_time_wait, b_event_);
        }
        if(b_event_q_.empty()) { // timer event ->
            // clean timer event -> make out whether compare or overflow
            if(timer_b_ < cmp_b_) {
                // compare
                timer_b_ = cmp_b_.read();
                (*bfs_t::irq_out_)[3] = 1;
                _sc_time_wait = (_OVERFLOW_ - cmp_b_) * base_clk_period * (pre_b_ ? pre_b_ : 1) - base_clk_period;
                wait(base_clk_period);
                (*bfs_t::irq_out_)[3] = 0;
            } else {
                // overflow
                timer_b_ = 0;
                (*bfs_t::irq_out_)[2] = 1;
                _sc_time_wait = cmp_b_.read() ? cmp_b_ * base_clk_period * (pre_b_ ? pre_b_ : 1) - base_clk_period
                                              : base_clk_period * (pre_b_ ? pre_b_ : 1);
                wait(base_clk_period);
                (*bfs_t::irq_out_)[2] = 0;
            }
        } else {
            sc_core::sc_time _passed_time = (sc_core::sc_time_stamp() - sct_wait_entry_b_);
            while(!(b_event_q_.empty())) {
                timer_events_t x = b_event_q_.front();
                switch(x) {
                case UPDATE_TIME:
                    timer_b_ = get_act_time_b(_passed_time);
                    if(timer_b_ >= cmp_b_)
                        _sc_time_wait = sc_core::sc_time(0, sc_core::SC_PS);
                    else
                        _sc_time_wait = (cmp_b_ - timer_b_) * base_clk_period * (pre_b_ ? pre_b_ : 1);
                    break;
                case RESET_INT:
                    (*bfs_t::irq_out_)[3] = 0;
                    break;
                case SET_INT:
                    (*bfs_t::irq_out_)[3] = 1;
                    break;
                default:
                    break;
                }
                b_event_q_.pop();
            }
        }
    }
}

template <class owner_t> uint64_t Timer<owner_t>::get_act_time_b(const sc_core::sc_time& passed_time) {
    uint64_t x = timer_a_;
    auto base_clk_period = sc_core::sc_time(static_cast<double>(cpu_period_ps_), sc_core::SC_PS);
    x += static_cast<sc_dt::uint64>(passed_time / base_clk_period * pre_a_);
    return (x);
}

template <class owner_t> uint64_t Timer<owner_t>::get_act_time_a(const sc_core::sc_time& passed_time) {
    uint64_t x = timer_b_;
    auto base_clk_period = sc_core::sc_time(static_cast<double>(cpu_period_ps_), sc_core::SC_PS);
    x += static_cast<sc_dt::uint64>(passed_time / base_clk_period * pre_b_);
    return (x);
}

template <class owner_t> void Timer<owner_t>::reset(void) {
    for(auto& bf : bfs_t::regs->bitfields) {
        bf = 0;
    }
}

template <class owner_t>
sc_core::sc_time Timer<owner_t>::prescale_clk(scc::bitfield<uint32_t>& pre, sc_core::sc_time& clk_period) {
    if(pre) {
        return clk_period * pre;
    } else {
        return clk_period;
    }
}

} // namespace pulpino
} // namespace vpvper

#endif // __VPVPER_PULPINO_TIMER_H__
