/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cci_configuration>
#include <scc/utilities.h>
#include <systemc>

namespace vpvper {
namespace generic {

class rst_gen : public sc_core::sc_module {
    SC_HAS_PROCESS(rst_gen);

public:
    sc_core::sc_out<bool> rst_o{"rst_o"};

    cci::cci_param<sc_core::sc_time> duration{"duration", 100_ns, "Duration of the reset being active"};

    cci::cci_param<bool> active_level{"active_level", true, "Duration of the reset being active"};

    rst_gen(sc_core::sc_module_name const& nm)
    : sc_core::sc_module(nm) {
        SC_THREAD(run);
    }

    rst_gen(sc_core::sc_module_name const& nm, bool active_level)
    : sc_core::sc_module(nm)
    , active_level{"active_level", active_level, "Duration of the reset being active"} {
        SC_THREAD(run);
    }

private:
    void run() {
        rst_o.write(active_level.get_value());
        wait(duration.get_value());
        rst_o.write(!active_level.get_value());
    }
};

} /* namespace generic */
} /* namespace vpvper */
