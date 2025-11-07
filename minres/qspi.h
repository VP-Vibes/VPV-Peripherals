/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _QSPI_H_
#define _QSPI_H_

#include <deque>
#include <scc/clock_if_mixins.h>
#include <scc/memory.h>
#include <scc/peq.h>
#include <scc/tlm_target.h>
#include <spi/spi_tlm.h>
#include <systemc>
#include <tlm/nw/initiator_mixin.h>
#include <tlm/nw/target_mixin.h>

namespace vpvper {
namespace minres {

class apb3spi_regs;

class qspi : public sc_core::sc_module, public scc::tlm_target<> {
public:
    SC_HAS_PROCESS(qspi); // NOLINT

    sc_core::sc_in<bool> rst_i{"rst_i"};

    tlm::nw::initiator_mixin<spi::spi_pkt_initiator_socket<>> spi_i{"spi_i"};

    sc_core::sc_out<bool> irq_o{"irq_o"};

    tlm::scc::target_mixin<tlm::tlm_target_socket<scc::LT>> xip_sck{"xip_sck"};

    qspi(sc_core::sc_module_name nm);

    virtual ~qspi() override; // need to keep it in source file because of fwd declaration of spi_regs

    void set_clock_period(sc_core::sc_time period) { clk_period = period; }

protected:
    void reset_cb();
    void peq_cb();
    sc_core::sc_time clk_period;
    std::unique_ptr<apb3spi_regs> regs;
    unsigned sel_slv_id{std::numeric_limits<unsigned>::max()};
    scc::peq<unsigned> cmd;
    std::deque<unsigned> rsp;
};

using qspi_tl = scc::tickless_clock<qspi>;
using qspi_tc = scc::ticking_clock<qspi>;

} /* namespace minres */
} /* namespace vpvper */

#endif /* _QSPI_H_ */
