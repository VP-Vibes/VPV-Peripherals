#ifndef _PULPISSIMO_SOC_CTRL_H_
#define _PULPISSIMO_SOC_CTRL_H_
#include <scc/tlm_target.h>

namespace pulpissimo {
namespace gen {
class soc_ctrl_regs;
}

class soc_ctrl : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i{"clk_i"};
    sc_core::sc_in<bool> rst_i{"rst_i"};
    soc_ctrl(sc_core::sc_module_name nm);
    virtual ~soc_ctrl() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::soc_ctrl_regs> regs;
};

} /* namespace pulpissimo */

#endif /* _PULPISSIMO_SOC_CTRL_H_ */
