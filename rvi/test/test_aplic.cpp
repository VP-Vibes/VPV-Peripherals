/*
 * Copyright (c) MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * SystemC testbench for vpvper::rvi::aplic.
 * 
 */


#include <iostream>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <scc/report.h>
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#include <tlm/scc/tlm_signal.h>
#endif

#include "rvi/aplic.h"

using APlic = vpvper::rvi::aplic;

// Register address helpers
static constexpr uint64_t DOMAINCFG()  { return 0x0000UL; }
static constexpr uint64_t SOURCECFG(uint32_t i) { return 0x4UL + i * 4; }
static constexpr uint64_t SETIP(uint32_t c)  { return 0x1c00UL + c * 4; }
static constexpr uint64_t CLRIP(uint32_t c)  { return 0x1d00UL + c * 4; }
static constexpr uint64_t SETIE(uint32_t c)   { return 0x1e00UL + c * 4; }
static constexpr uint64_t CLRIE(uint32_t c)   { return 0x1f00UL + c * 4; }
static constexpr uint64_t TARGETCFG(uint32_t c)   { return 0x3004UL + c * 4; }
static constexpr uint64_t DELIVERY()   { return 0x4000UL; }
static constexpr uint64_t THRESHOLD()   { return 0x4008UL; }
static constexpr uint64_t TOP()   { return 0x4018UL; }
static constexpr uint64_t CLAIM()   { return 0x401cUL; }



struct TestBench : sc_core::sc_module {
    SC_HAS_PROCESS(TestBench); // NOLINT

    sc_core::sc_signal<bool>             rst_sig{"rst"};
    APlic                                 dut{"dut"};
    tlm_utils::simple_initiator_socket<TestBench, 0> isock{"isock"};

#ifdef SC_SIGNAL_IF
    sc_core::sc_signal<bool> irq_out{"irq_out"};
#else
    tlm::scc::tlm_signal_bool_in  irq_out{"irq_out"};
#endif
    bool ctx_irq{false};

    int failures{0};

    TestBench(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm) {
        dut.rst_i(rst_sig);

        dut.core_interrupt_o(irq_out);
#ifndef SC_SIGNAL_IF
        irq_out.register_nb_transport(
            [this](tlm::scc::tlm_signal_gp<bool>& gp,
                        tlm::tlm_phase&, sc_core::sc_time&) -> tlm::tlm_sync_enum {
                std::cout << "register_nb_transport" << gp.get_value() << std::endl;
                ctx_irq = gp.get_value();
                return tlm::TLM_COMPLETED;
            });
#endif

        isock.bind(dut.socket);
        SC_THREAD(run);
    }

    void write32(uint64_t addr, uint32_t val) {
        tlm::tlm_generic_payload t;
        t.set_command(tlm::TLM_WRITE_COMMAND);
        t.set_address(addr);
        t.set_data_ptr(reinterpret_cast<unsigned char*>(&val));
        t.set_data_length(4);
        t.set_streaming_width(4);
        t.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        sc_core::sc_time d;
        isock->b_transport(t, d);
    }

    uint32_t read32(uint64_t addr) {
        uint32_t val{};
        tlm::tlm_generic_payload t;
        t.set_command(tlm::TLM_READ_COMMAND);
        t.set_address(addr);
        t.set_data_ptr(reinterpret_cast<unsigned char*>(&val));
        t.set_data_length(4);
        t.set_streaming_width(4);
        t.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        sc_core::sc_time d;
        isock->b_transport(t, d);
        return val;
    }

    bool get_irq() {
#ifdef SC_SIGNAL_IF
        return irq_out.read();
#else
        return ctx_irq;
#endif
    }

    void do_reset() {
        rst_sig.write(true);
        wait(sc_core::SC_ZERO_TIME);
        rst_sig.write(false);
        wait(sc_core::SC_ZERO_TIME);
    }

    #define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "FAIL [line " << __LINE__ << "]: " << (msg) << "\n"; \
            ++failures; \
        } else { \
            std::cout << "PASS: " << (msg) << "\n"; \
        } \
    } while (0)

    void run();
};

void TestBench::run() {

    do_reset();

    static constexpr uint32_t INTERRUPT_NO = 192;

    const uint32_t interrupt_register = INTERRUPT_NO / 32;

    // -----------------------------------------------------------------------
    // T1: Basic interrupt delivery
    //   source EDGE, Interrupt delivery enable, domaincfg: Interrupt enable  Interrupt(192): enable, priority =1
    // -----------------------------------------------------------------------
    write32(DOMAINCFG(),0x104);
    write32(DELIVERY(),0x1);
    write32(SOURCECFG(INTERRUPT_NO - 1),0x4);
    write32(TARGETCFG(INTERRUPT_NO - 1),0x1);
    write32(SETIE(interrupt_register),0x1);
    write32(SETIP(interrupt_register),0x1);

    CHECK(get_irq(), "T1: Interrupt asserts");
    uint32_t top = read32(TOP());
    CHECK(top != 0, "T1: top register contain highest-priority pending-and-enabled interrupt information");
    write32(CLRIP(interrupt_register),0x1);
    CHECK(!get_irq(), "T1: Interrupt deasserts");

     

    /// -----------------------------------------------------------------------
    // T2: interrupt not assert
    //   source EDGE, Interrupt delivery enable, domaincfg: Interrupt disbale,  Interrupt(192): enable, priority =1
    // -----------------------------------------------------------------------

    write32(DOMAINCFG(),0x0);
    write32(DELIVERY(),0x1);
    write32(SOURCECFG(INTERRUPT_NO - 1),0x4);
    write32(TARGETCFG(INTERRUPT_NO - 1),0x1);
    write32(SETIE(interrupt_register),0x1);
    write32(SETIP(interrupt_register),0x1);

    CHECK(!get_irq(), "T2: Interrupt not asserts");
    top = read32(TOP());
    CHECK(top != 0, "T2: top register contain highest-priority pending-and-enabled interrupt information");

    /// -----------------------------------------------------------------------
    // T3: interrupt assert when domaincfg: Interrupt enable
    // -----------------------------------------------------------------------
    write32(DOMAINCFG(),0x104);
    CHECK(get_irq(), "T3: Interrupt asserts");
    write32(CLRIP(interrupt_register),0x1);
    CHECK(!get_irq(), "T3: Interrupt deasserts");



    /// -----------------------------------------------------------------------
    // T4: interrupt not assert
    //   source EDGE, Interrupt delivery disable , domaincfg: Interrupt enable  Interrupt(192): enable, priority =1
    // -----------------------------------------------------------------------


    write32(DOMAINCFG(),0x104);
    write32(DELIVERY(),0x0);
    write32(SOURCECFG(INTERRUPT_NO - 1),0x4);
    write32(TARGETCFG(INTERRUPT_NO - 1),0x1);
    write32(SETIE(interrupt_register),0x1);
    write32(SETIP(interrupt_register),0x1);
    CHECK(!get_irq(), "T4: Interrupt not asserts");
    top = read32(TOP());
    CHECK(top != 0, "T4: top register contain highest-priority pending-and-enabled interrupt information");


    /// -----------------------------------------------------------------------
    // T5: interrupt assert when delivery enable
    // -----------------------------------------------------------------------

    write32(DELIVERY(),0x1);
    CHECK(get_irq(), "T5: Interrupt asserts");
    write32(CLRIP(interrupt_register),0x1);
    CHECK(!get_irq(), "T5: Interrupt deasserts");

    /// -----------------------------------------------------------------------
    // T6: interrupt not assert if Interrupt(192): disable
    // source EDGE, Interrupt delivery disable , domaincfg: Interrupt enable, priority =1
    // ----------------------------------------------------------------------

    write32(DOMAINCFG(),0x104);
    write32(DELIVERY(),0x1);
    write32(SOURCECFG(INTERRUPT_NO - 1),0x4);
    write32(TARGETCFG(INTERRUPT_NO - 1),0x1);
    write32(CLRIE(interrupt_register),0x1);
    write32(SETIP(interrupt_register),0x1);
    CHECK(!get_irq(), "T6: Interrupt not asserts");
    top = read32(TOP());
    CHECK(top == 0, "T6: top register value should be zero");


    sc_core::sc_stop();
}

int sc_main(int /*argc*/, char* /*argv*/[]) {
    scc::init_logging(scc::log::DEBUG);
    TestBench tb{"tb"};
    sc_core::sc_start();

    if (tb.failures > 0) {
        std::cerr << tb.failures << " APLIC test(s) FAILED\n";
        return 1;
    }
    std::cout << "All APLIC tests PASSED\n";
    return 0;
}
