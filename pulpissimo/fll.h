#ifndef _PULPISSIMO_FLL_H_
#define _PULPISSIMO_FLL_H_
#include "scc/tlm_target.h"

namespace pulpissimo {
namespace gen {
class fll_regs;
}

class fll : public sc_core::sc_module, public scc::tlm_target<> {
public:
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    fll(sc_core::sc_module_name nm);
    virtual ~fll() override;

protected:
    void clock_cb();
    void reset_cb();
    sc_core::sc_time clk;
    std::unique_ptr<gen::fll_regs> regs;
};

} /* namespace pulpissimo */

#endif /* _PULPISSIMO_FLL_H_ */
