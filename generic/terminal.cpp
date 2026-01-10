/*
 * Copyright (c) 2019 -2021 MINRES Technologies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "terminal.h"

#include <scc/report.h>
#if defined(HAS_WEB_SOCKETS)
#include "sc_comm_singleton.h"
#endif
#include "tlm_extensions.h"
#include <string>

namespace vpvper {
namespace generic {
using namespace std;

terminal::terminal()
: terminal(sc_core::sc_gen_unique_name("terminal")) {}

terminal::terminal(const sc_core::sc_module_name& nm)
: sc_core::sc_module(nm)
, NAMED(tx_o)
, NAMED(rx_i)
, NAMED(write_to_ws, false) {
    rx_i.register_nb_transport(
        [this](tlm::scc::tlm_signal_gp<sc_dt::sc_logic>& gp, tlm::tlm_phase& phase, sc_core::sc_time& delay) -> tlm::tlm_sync_enum {
            this->receive(gp, delay);
            return tlm::TLM_COMPLETED;
        });
}

terminal::~terminal() = default;

void terminal::before_end_of_elaboration() {
#if defined(HAS_WEB_SOCKETS)
    if(write_to_ws.get_value()) {
        auto end_point = std::string{"/ws/"} + name();
        SCCTRACE() << "Adding WS handler for " << end_point;
        handler = std::make_shared<WsHandler>();
        sc_comm_singleton::inst().registerWebSocketHandler(end_point.c_str(), handler);
    }
#endif
}

void terminal::receive(tlm::scc::tlm_signal_gp<sc_dt::sc_logic>& gp, sc_core::sc_time& delay) {
    tlm_signal_uart_extension* ext;
    gp.get_extension(ext);
    if(ext && ext->start_time != last_tx_start) {
        auto txdata = static_cast<uint8_t>(ext->tx.data);
        last_tx_start = ext->start_time;
        if(txdata != '\r')
            queue.push_back(txdata);
        if(queue.size() && (txdata == '\n' || txdata == 0)) {
            std::string msg(queue.begin(), queue.end() - 1);
            sc_core::sc_time now = sc_core::sc_time_stamp();
#if defined(HAS_WEB_SOCKETS)
            if(handler)
                sc_comm_singleton::inst().execute([this, msg, now]() {
                    std::stringstream os;
                    os << R"({"time":")" << now << R"(","message":")" << msg << R"("})";
                    this->handler->send(os.str());
                });
            else
#endif
                SCCINFO(this->name()) << " receive: '" << msg << "'";
            queue.clear();
        }
    }
}
} // namespace generic
} // namespace vpvper
