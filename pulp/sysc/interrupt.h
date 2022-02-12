#ifndef _SYSC_INTERRUPT_H_
#define _SYSC_INTERRUPT_H_
#include "scc/tlm_target.h"

namespace sysc {
namespace gen {
class interrupt_regs;
}

class interrupt : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    interrupt(sc_core::sc_module_name nm);
    virtual ~interrupt() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::interrupt_regs> regs;
};

} /* namespace sysc */

#endif /* _SYSC_INTERRUPT_H_ */
