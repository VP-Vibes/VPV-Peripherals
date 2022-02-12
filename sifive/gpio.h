/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include <cci_configuration>
#include <tlm/scc/signal_initiator_mixin.h>
#include <tlm/scc/signal_target_mixin.h>
#include <scc/tlm_target.h>
#include <memory>
#include <tlm/scc/tlm_signal.h>

namespace vpvper {
namespace generic {
class WsHandler;
}
namespace sifive {

class gpio_regs;

class gpio : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(gpio);// NOLINT
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    // sc_core::sc_inout_rv<32> pins_io;

    sc_core::sc_vector<tlm::scc::tlm_signal_logic_out> pins_o;
    sc_core::sc_vector<tlm::scc::tlm_signal_logic_in> pins_i;

    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_out> iof0_o;
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_out> iof1_o;
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_in> iof0_i;
    sc_core::sc_vector<tlm::scc::tlm_signal_bool_opt_in> iof1_i;

    gpio(sc_core::sc_module_name nm);
    virtual ~gpio() override; // need to keep it in source file because of fwd declaration of gpio_regs

    cci::cci_param<bool> write_to_ws;

protected:
    void clock_cb();
    void reset_cb();
    void update_pins(uint32_t changed_bits);
    void before_end_of_elaboration() override;
    void pin_input(unsigned int tag, tlm::scc::tlm_signal_gp<sc_dt::sc_logic> &gp, sc_core::sc_time &delay);
    void forward_pin_input(unsigned int tag, tlm::scc::tlm_signal_gp<sc_dt::sc_logic> &gp);
    void iof_input(unsigned int tag, unsigned iof_idx, tlm::scc::tlm_signal_gp<> &gp, sc_core::sc_time &delay);
    sc_core::sc_time clk;
    std::array<bool, 32> last_iof0, last_iof1;
    std::unique_ptr<gpio_regs> regs;
    std::shared_ptr<vpvper::generic::WsHandler> handler;

private:
    tlm::tlm_phase write_output(tlm::scc::tlm_signal_gp<sc_dt::sc_logic> &gp, size_t i, sc_dt::sc_logic val);
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _GPIO_H_ */
