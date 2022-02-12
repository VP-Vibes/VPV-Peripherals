#ifndef _SYSC_GPIO_H_
#define _SYSC_GPIO_H_
#include "scc/tlm_target.h"

namespace sysc {
namespace gen {
class gpio_regs;
}

class gpio : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    gpio(sc_core::sc_module_name nm);
    virtual ~gpio() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::gpio_regs> regs;
};

} /* namespace sysc */

#endif /* _SYSC_GPIO_H_ */
