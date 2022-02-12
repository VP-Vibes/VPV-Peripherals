#ifndef _SYSC_ADV_TIMER_H_
#define _SYSC_ADV_TIMER_H_
#include "scc/tlm_target.h"

namespace sysc {
namespace gen {
class adv_timer_regs;
}

class adv_timer : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    adv_timer(sc_core::sc_module_name nm);
    virtual ~adv_timer() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::adv_timer_regs> regs;
};

} /* namespace sysc */

#endif /* _SYSC_ADV_TIMER_H_ */
