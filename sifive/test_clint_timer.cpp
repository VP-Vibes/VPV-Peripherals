/*
 * Regression test for CLINT mtime interrupt correctness.
 *
 * With the bug (4096x clock multiplier), the interrupt fires ~40.96 ms
 * after mtimecmp is written.  With the fix, it fires at ~10 µs.
 * We wait 15 µs and assert the interrupt was seen.
 */

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <iostream>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <scc/report.h>
#ifndef SC_SIGNAL_IF
#include <tlm/scc/signal_target_mixin.h>
#include <tlm/scc/tlm_signal.h>
#endif

#include "sifive/clint.h"

struct TestBench : sc_core::sc_module {
    SC_HAS_PROCESS(TestBench); // NOLINT

    sc_core::sc_signal<sc_core::sc_time> tlclk_sig{"tlclk"}, lfclk_sig{"lfclk"};
    sc_core::sc_signal<bool> rst_sig{"rst"};
    vpvper::sifive::clint dut;
    tlm_utils::simple_initiator_socket<TestBench, 0> isock;
#ifdef SC_SIGNAL_IF
    sc_core::sc_signal<bool> mtime_int_sig{"mtime_int"}, msip_int_sig{"msip_int"};
#else
    tlm::scc::tlm_signal_bool_in mtime_int_in{"mtime_int_in"};
    tlm::scc::tlm_signal_bool_in msip_int_in{"msip_int_in"};
#endif
    bool passed{false};

    TestBench(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm)
    , dut("dut")
    , isock("isock") {
        dut.tlclk_i(tlclk_sig);
        dut.lfclk_i(lfclk_sig);
        dut.rst_i(rst_sig);
#ifdef SC_SIGNAL_IF
        dut.mtime_int_o(mtime_int_sig);
        dut.msip_int_o(msip_int_sig);
#else
        dut.mtime_int_o(mtime_int_in);
        dut.msip_int_o(msip_int_in);
        msip_int_in.error_if_no_callback = false;
        mtime_int_in.register_nb_transport(
            [this](tlm::scc::tlm_signal_gp<bool>& gp,
                   tlm::tlm_phase&, sc_core::sc_time&) -> tlm::tlm_sync_enum {
                if(gp.get_value()) passed = true;
                return tlm::TLM_COMPLETED;
            });
#endif
        isock.bind(dut.socket);
        SC_THREAD(run);
    }

    void run() {
        rst_sig.write(true);
        wait(sc_core::SC_ZERO_TIME);
        rst_sig.write(false);
        wait(sc_core::SC_ZERO_TIME);

        lfclk_sig.write(sc_core::sc_time(1.0, sc_core::SC_US));
        tlclk_sig.write(sc_core::sc_time(1.0, sc_core::SC_US));
        wait(sc_core::SC_ZERO_TIME);

        // Write mtimecmp = 10 (via TLM at offset 0x4000)
        // With fix:  next_trigger = 1µs * 10 = 10µs  → interrupt fires at ~10µs
        // With bug:  next_trigger = 1µs * 4096 * 10 = 40.96ms → no interrupt within 15µs
        uint64_t mtimecmp_val = 10;
        tlm::tlm_generic_payload trans;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(0x4000ULL);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&mtimecmp_val));
        trans.set_data_length(sizeof(mtimecmp_val));
        trans.set_streaming_width(sizeof(mtimecmp_val));
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
        isock->b_transport(trans, delay);
        // Clear any spurious interrupt that fired at startup (mtime == mtimecmp == 0).
        // Only the scheduled mtime_evt should be able to set passed=true from here.
        passed = false;

        // With fix:  event at clk * (mtimecmp - mtime) ≈ 10µs → fires within 15µs
        // With bug:  event at clk * 4096 * (mtimecmp - mtime) ≈ 40.96ms → misses window
        wait(15, sc_core::SC_US);
#ifdef SC_SIGNAL_IF
        passed = mtime_int_sig.read();
#endif
        sc_core::sc_stop();
    }
};

int sc_main(int /*argc*/, char* /*argv*/[]) {
    scc::init_logging(scc::log::WARNING);
    TestBench tb("tb");
    sc_core::sc_start();

    if(!tb.passed) {
        std::cerr << "FAIL: mtime interrupt did not assert within 15us" << std::endl;
        return 1;
    }
    return 0;
}
