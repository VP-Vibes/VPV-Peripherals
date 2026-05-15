/*
 * Copyright (c) MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * SystemC testbench for vpvper::rvi::plic<NUM_SOURCES, NUM_CONTEXTS>.
 *
 * Exercises: basic delivery, claim/complete protocol, threshold filtering,
 * priority arbitration, context isolation, multiple-pending queuing, reset,
 * and the interrupt index-to-source-ID mapping.
 */

#define SC_INCLUDE_DYNAMIC_PROCESSES
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

#include "rvi/plic.h"

static constexpr uint32_t NS = 8;
static constexpr uint32_t NC = 2;
using Plic = vpvper::rvi::plic<NS, NC>;

// Register address helpers
static constexpr uint64_t PRIO(uint32_t s)    { return 0x0000UL + s * 4; }
static constexpr uint64_t PENDING(uint32_t i) { return 0x1000UL + i * 4; }
static constexpr uint64_t ENABLE(uint32_t c)  { return 0x2000UL + c * 0x80UL; }
static constexpr uint64_t THRESH(uint32_t c)  { return 0x200000UL + c * 0x1000UL; }
static constexpr uint64_t CLAIM(uint32_t c)   { return 0x200004UL + c * 0x1000UL; }

struct TestBench : sc_core::sc_module {
    SC_HAS_PROCESS(TestBench); // NOLINT

    sc_core::sc_signal<sc_core::sc_time> clk_sig{"clk"};
    sc_core::sc_signal<bool>             rst_sig{"rst"};
    Plic                                 dut{"dut"};
    tlm_utils::simple_initiator_socket<TestBench, 0> isock{"isock"};

#ifdef SC_SIGNAL_IF
    sc_core::sc_vector<sc_core::sc_signal<bool>> irq_src{"irq_src", NS};
    sc_core::sc_vector<sc_core::sc_signal<bool>> irq_out{"irq_out", NC};
#else
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_out> irq_src{"irq_src", NS};
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_in>  irq_out{"irq_out", NC};
    std::array<bool, NC> ctx_irq{};
#endif

    int failures{0};

    TestBench(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm) {
        dut.clk_i(clk_sig);
        dut.rst_i(rst_sig);

        for (uint32_t i = 0; i < NS; ++i) {
#ifdef SC_SIGNAL_IF
            dut.interrupts_i[i](irq_src[i]);
#else
            irq_src[i](dut.interrupts_i[i]);
#endif
        }
        for (uint32_t i = 0; i < NC; ++i) {
            dut.interrupts_o[i](irq_out[i]);
#ifndef SC_SIGNAL_IF
            const auto idx = i;
            irq_out[i].register_nb_transport(
                [this, idx](tlm::scc::tlm_signal_gp<bool>& gp,
                            tlm::tlm_phase&, sc_core::sc_time&) -> tlm::tlm_sync_enum {
                    ctx_irq[idx] = gp.get_value();
                    return tlm::TLM_COMPLETED;
                });
#endif
        }

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

    // Fire (or lower) an interrupt source and advance two delta cycles so
    // the PLIC callback and its resulting output write both propagate.
    void fire_irq(uint32_t src, bool val) {
#ifdef SC_SIGNAL_IF
        irq_src[src].write(val);
        wait(sc_core::SC_ZERO_TIME);  // signal propagates to PLIC
        wait(sc_core::SC_ZERO_TIME);  // PLIC output write propagates
#else
        tlm::scc::tlm_signal_gp<bool> gp;
        gp.set_value(val);
        sc_core::sc_time tt = sc_core::SC_ZERO_TIME;
        tlm::tlm_phase  p  = tlm::BEGIN_REQ;
        irq_src[src]->nb_transport_fw(gp, p, tt);
        wait(sc_core::SC_ZERO_TIME);
#endif
    }

    bool get_irq(uint32_t ctx) {
#ifdef SC_SIGNAL_IF
        return irq_out[ctx].read();
#else
        return ctx_irq[ctx];
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
    clk_sig.write(sc_core::sc_time(10, sc_core::SC_NS));
    wait(sc_core::SC_ZERO_TIME);
    do_reset();

    // -----------------------------------------------------------------------
    // T1: Basic interrupt delivery
    //   source 1, priority 1, enabled for ctx0 only, threshold 0 → ctx0 fires
    // -----------------------------------------------------------------------
    write32(PRIO(1),   1);
    write32(ENABLE(0), 0x2u);  // bit 1 → source 1
    write32(THRESH(0), 0);
    fire_irq(0, true);
    CHECK( get_irq(0), "T1: ctx0 asserts when source 1 fires");
    CHECK(!get_irq(1), "T1: ctx1 stays low (source 1 not enabled for ctx1)");

    // -----------------------------------------------------------------------
    // T2: Claim returns the source ID, clears its pending bit, and de-asserts
    //     the IRQ output immediately when no further pending interrupts remain.
    // -----------------------------------------------------------------------
    const uint32_t claimed_t2 = read32(CLAIM(0));
    CHECK(claimed_t2 == 1, "T2: claim register returns source 1");
    CHECK((read32(PENDING(0)) & 0x2u) == 0, "T2: pending bit 1 cleared after claim");
    // handle_pending_irq is called inside claim_complete_read_cb; with no remaining
    // pending interrupts it de-asserts the output immediately.
    CHECK(!get_irq(0), "T2: ctx0 de-asserts after claim (no further pending interrupts)");

    // -----------------------------------------------------------------------
    // T3: Complete marks the source cleared so it can re-fire; the IRQ output
    //     was already de-asserted by the T2 claim.
    // -----------------------------------------------------------------------
    fire_irq(0, false);        // lower source line before completing
    write32(CLAIM(0), 1);      // complete: sets src_irq_cleared[0]=true so source can re-fire
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T3: ctx0 stays de-asserted after complete (no new trigger)");

    // Re-fire: source cleared by complete → rising edge delivers the interrupt
    fire_irq(0, true);
    CHECK(get_irq(0), "T3: ctx0 re-fires after complete + re-trigger");

    // Clean up for the next test
    fire_irq(0, false);
    const uint32_t reclaim = read32(CLAIM(0));   // claim: clears pending bit, de-asserts output
    write32(CLAIM(0), reclaim);                  // complete: marks source 1 cleared
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T3: ctx0 stays de-asserted after cleanup complete");

    write32(ENABLE(0), 0);
    write32(PRIO(1), 0);
    do_reset();

    // -----------------------------------------------------------------------
    // T4: Threshold filtering — priority equal to threshold is blocked
    //   The PLIC requires prio > threshold (strict), so prio=1, thresh=1 → blocked
    // -----------------------------------------------------------------------
    write32(PRIO(2),   1);
    write32(ENABLE(0), 0x4u);  // bit 2 → source 2
    write32(THRESH(0), 1);
    fire_irq(1, true);
    CHECK(!get_irq(0), "T4: prio==threshold blocks delivery (prio 1, thresh 1)");

    // Raise priority above threshold → still no retroactive delivery (no re-trigger)
    write32(PRIO(2), 2);
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T4: changing priority alone does not retrigger delivery");

    fire_irq(1, false);
    do_reset();  // flush the stuck pending bit

    // -----------------------------------------------------------------------
    // T5: Priority arbitration — claim returns highest-priority pending source
    //   source 3 (prio 2) and source 4 (prio 3) both pending;
    //   claim must return 4 first, then 3 after completing 4.
    // -----------------------------------------------------------------------
    write32(PRIO(3),   2);
    write32(PRIO(4),   3);
    write32(ENABLE(0), 0x18u);  // bits 3 and 4
    write32(THRESH(0), 0);

    fire_irq(2, true);   // ctx0 asserts (first pending interrupt)
    fire_irq(3, true);   // ctx0 already pending; no second pulse

    CHECK(get_irq(0), "T5: ctx0 asserts with sources 3 and 4 both pending");

    const uint32_t c5a = read32(CLAIM(0));
    CHECK(c5a == 4, "T5: claim returns source 4 (highest priority)");

    fire_irq(2, false);
    fire_irq(3, false);
    write32(CLAIM(0), 4);       // complete source 4
    wait(sc_core::SC_ZERO_TIME);
    // output stays asserted: handle_pending_irq found source 3 still pending during the claim of source 4
    CHECK(get_irq(0), "T5: ctx0 remains asserted for source 3 after completing source 4");

    const uint32_t c5b = read32(CLAIM(0));
    CHECK(c5b == 3, "T5: claim returns source 3 after source 4 completed");

    write32(CLAIM(0), 3);       // complete source 3
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T5: ctx0 de-asserts after both sources completed");

    write32(ENABLE(0), 0);
    write32(PRIO(3), 0);
    write32(PRIO(4), 0);

    // -----------------------------------------------------------------------
    // T6: Context isolation — source enabled for ctx0 only, ctx1 unaffected
    // -----------------------------------------------------------------------
    write32(PRIO(5),   1);
    write32(ENABLE(0), 0x20u);  // bit 5 → source 5 for ctx0
    write32(ENABLE(1), 0);      // nothing for ctx1
    write32(THRESH(0), 0);
    write32(THRESH(1), 0);

    fire_irq(4, true);
    CHECK( get_irq(0), "T6: ctx0 asserts for source 5");
    CHECK(!get_irq(1), "T6: ctx1 stays low (source 5 not enabled for ctx1)");

    fire_irq(4, false);
    write32(CLAIM(0), read32(CLAIM(0)));  // claim then complete source 5
    wait(sc_core::SC_ZERO_TIME);
    write32(CLAIM(0), 5);
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T6: ctx0 de-asserts after complete");

    write32(ENABLE(0), 0);
    write32(PRIO(5), 0);

    // -----------------------------------------------------------------------
    // T7: Multiple pending — second interrupt does not re-pulse ctx;
    //     interrupts are delivered sequentially via claim/complete.
    //   source 6 (prio 1), source 7 (prio 2)
    // -----------------------------------------------------------------------
    write32(PRIO(6),   1);
    write32(PRIO(7),   2);
    write32(ENABLE(0), 0xC0u);  // bits 6 and 7
    write32(THRESH(0), 0);

    fire_irq(5, true);
    CHECK(get_irq(0), "T7: ctx0 asserts on source 6");

    fire_irq(6, true);   // ctx0 already asserted; handle_pending_irq re-evaluates but output stays high
    CHECK(get_irq(0), "T7: ctx0 still asserted with source 7 also pending");

    fire_irq(5, false);
    fire_irq(6, false);

    const uint32_t c7a = read32(CLAIM(0));
    CHECK(c7a == 7, "T7: claim returns source 7 (higher priority)");

    write32(CLAIM(0), 7);       // complete source 7
    wait(sc_core::SC_ZERO_TIME);
    // output stays asserted: handle_pending_irq found source 6 still pending during the claim of source 7
    CHECK(get_irq(0), "T7: ctx0 remains asserted for source 6 after completing source 7");

    const uint32_t c7b = read32(CLAIM(0));
    CHECK(c7b == 6, "T7: claim returns source 6 next");

    write32(CLAIM(0), 6);       // complete source 6
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T7: ctx0 fully de-asserts after both sources completed");

    write32(ENABLE(0), 0);
    write32(PRIO(6), 0);
    write32(PRIO(7), 0);

    // -----------------------------------------------------------------------
    // T8: Reset clears all register state
    // -----------------------------------------------------------------------
    write32(PRIO(1),   7);
    write32(ENABLE(0), 0xFFu);
    write32(THRESH(0), 3);
    do_reset();
    CHECK(read32(PRIO(1))   == 0, "T8: priority[1] zeroed after reset");
    CHECK(read32(ENABLE(0)) == 0, "T8: enable[ctx0] zeroed after reset");
    CHECK(read32(THRESH(0)) == 0, "T8: threshold[ctx0] zeroed after reset");

    // -----------------------------------------------------------------------
    // T9: interrupts_i[0] maps to PLIC source ID 1; source ID 0 is reserved
    //     and unreachable via any interrupt input. Verify the index offset is
    //     correct by firing index 0 and confirming source 1 is claimed.
    // -----------------------------------------------------------------------
    write32(PRIO(1),   1);
    write32(ENABLE(0), 0x2u);   // bit 1 → source 1
    write32(THRESH(0), 0);
    fire_irq(0, true);           // index 0 → source ID 1
    CHECK(get_irq(0), "T9: interrupts_i[0] triggers ctx0 (maps to source ID 1)");
    const uint32_t c9 = read32(CLAIM(0));
    CHECK(c9 == 1, "T9: claim returns source ID 1, confirming index-to-ID offset");
    fire_irq(0, false);
    write32(CLAIM(0), 1);
    wait(sc_core::SC_ZERO_TIME);
    CHECK(!get_irq(0), "T9: ctx0 de-asserts after completing source 1");

    sc_core::sc_stop();
}

int sc_main(int /*argc*/, char* /*argv*/[]) {
    scc::init_logging(scc::log::WARNING);
    TestBench tb{"tb"};
    sc_core::sc_start();

    if (tb.failures > 0) {
        std::cerr << tb.failures << " PLIC test(s) FAILED\n";
        return 1;
    }
    std::cout << "All PLIC tests PASSED\n";
    return 0;
}
