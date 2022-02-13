#ifndef _PULPISSIMO_GPIO_H_
#define _PULPISSIMO_GPIO_H_
#include "scc/tlm_target.h"

namespace pulpissimo {
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

} /* namespace pulpissimo */

#endif /* _PULPISSIMO_GPIO_H_ */
