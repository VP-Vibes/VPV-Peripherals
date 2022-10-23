

#pragma once

#include "scc/memory.h"
//
#include "systemc.h"

namespace vpvper::pulpissimo {

using l2mem_t = scc::memory<512_kB, 32>;

inline uint32_t kL2MemBaseAddr{0x1c000000};

inline std::function<bool(const scc::sc_register<uint32_t> &, uint32_t &, sc_core::sc_time)> simple_read{
    [](const scc::sc_register<uint32_t> &reg, uint32_t &v, sc_core::sc_time d) -> bool {
      v = reg.get();
      return true;
    }};

inline std::function<bool(scc::sc_register<uint32_t> &, uint32_t, sc_core::sc_time)> simple_write{
    [](scc::sc_register<uint32_t> &reg, uint32_t v, sc_core::sc_time d) -> bool {
      reg.put(v);
      return true;
    }};

}  // namespace vpvper::pulpissimo
