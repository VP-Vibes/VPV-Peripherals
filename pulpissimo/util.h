

#pragma once

#include "scc/memory.h"
#include "scc/register.h"
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

// The pulpissimo peripherals expect their owner (i.e. SoC in which they would be integrated) to have a certain
// structure hence use following base class to write your SoC class that require these pulpissimo peripherals
class SoC : public sc_core::sc_module {
 public:
  SoC(sc_core::sc_module_name);
  virtual ~SoC() = default;
  SoC(const SoC &) = delete;
  SoC(SoC &&) = delete;
  SoC &operator=(const SoC &) = delete;
  SoC &operator=(SoC &&) = delete;

  virtual void readMemory(unsigned char *, uint32_t, size_t) = 0;
  virtual void writeMemory(unsigned char *, uint32_t, size_t) = 0;
  virtual void transmitSPIMSocket(size_t, tlm::tlm_generic_payload &, sc_core::sc_time &) = 0;
  virtual void transmitI2SSocket(size_t, tlm::tlm_generic_payload &, sc_core::sc_time &) = 0;
  virtual void setEvent(size_t) = 0;
  virtual void raiseInterrupt(size_t) = 0;
};

}  // namespace vpvper::pulpissimo
