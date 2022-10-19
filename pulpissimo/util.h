

#pragma once

#include "systemc.h"

namespace vpvper::pulpissimo {


inline std::function<bool(const scc::sc_register<uint32_t> &, uint32_t &, sc_core::sc_time)> simple_read{
  [](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time d) -> bool {
    v = reg.get();
    return true;
  }
};

inline std::function<bool(scc::sc_register<uint32_t> &, uint32_t, sc_core::sc_time)> simple_write{
  [](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
    reg.put(v);
    return true;
  }
};


} // vpvper::pulpissimo
