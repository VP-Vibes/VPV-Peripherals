#ifndef DMA_H
#define DMA_H

#include "gen/dma_regs.h"
#include <cci_configuration>
#include <scc/clock_if_mixins.h>
#include <scc/tlm_target.h>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

namespace vpvper {
namespace minres {

class dma : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<bool> rst_i{"rst_i"};     // Reset signal
    sc_core::sc_out<bool> irq_o{"irq_o"};    // Interrupt output
    sc_core::sc_event ch0_enable_event;      // Event for register bit
    sc_core::sc_event ch1_enable_event;      // Event for register bit

    // TLM Sockets
    tlm_utils::simple_initiator_socket<dma, scc::LT> initiator_socket;

    dma(sc_core::sc_module_name nm);
    virtual ~dma() override;

    void set_clock_period(sc_core::sc_time period) { clk_period = period; }

protected:
    void reset_cb();
    void transfer_process_ch0();
    void transfer_process_ch1();
    void update_irq();

    sc_core::sc_time clk_period;
    std::unique_ptr<dma_regs> regs;
};

using dma_tl = scc::tickless_clock<dma>;
using dma_tc = scc::ticking_clock<dma>;

} // namespace minres
} // namespace vpvper

#endif // DMA_H
