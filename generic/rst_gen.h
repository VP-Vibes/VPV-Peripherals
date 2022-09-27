/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <systemc>

namespace vpvper {
namespace generic {

class rst_gen : public sc_core::sc_module {
    SC_HAS_PROCESS(rst_gen);
public:
    rst_gen(sc_core::sc_module_name const& nm) {
        SC_THREAD(run);
    }
    sc_core::sc_out<bool> rst_n{"rst_n"};
private:
    void run(){
        rst_n.write(false);
        wait(100_ns);
        rst_n.write(true);
    }
};

} /* namespace generic */
} /* namespace vpvper */
