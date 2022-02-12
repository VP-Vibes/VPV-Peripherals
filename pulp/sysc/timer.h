#ifndef _SYSC_TIMER_H_
#define _SYSC_TIMER_H_
#include "scc/tlm_target.h"

namespace sysc {
namespace gen {
class timer_regs;
}

class timer : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    timer(sc_core::sc_module_name nm);
    virtual ~timer() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::timer_regs> regs;
};

} /* namespace sysc */

#endif /* _SYSC_TIMER_H_ */
