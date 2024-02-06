#include "aclint.h"
#include "gen/Apb3AClint_regs.h"
#include <cstdint>

namespace vpvper {
namespace minres {
using namespace sc_core;
const int lfclk_mutiplier = 10; //hardcoded for unit test

aclint::aclint(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, tlm_target<>(clk_period)
,regs(scc::make_unique<Apb3AClint_regs>("regs")) 
{
    SC_HAS_PROCESS(aclint);
    regs->registerResources(*this);
    
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    regs->mtime_hi.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t data, sc_core::sc_time& d) -> bool{
        return false;
    });
    regs->mtime_hi.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time& d) -> bool{ 
        uint64_t elapsed_clks = (sc_time_stamp() + d - last_updt)/clk_period;
        data = regs->in_reset() ? 0 : regs->r_mtime_hi + elapsed_clks;
        return true;
    });

    regs->mtime_lo.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t data, sc_core::sc_time& d) -> bool{
        return false;
    });
    regs->mtime_lo.set_read_cb([this](const scc::sc_register<uint32_t> &reg, uint32_t &data, sc_core::sc_time& d) -> bool{ 
        uint64_t elapsed_clks = (sc_time_stamp() + d - last_updt)/clk_period;
        data = regs->in_reset() ? 0 : regs->r_mtime_lo + elapsed_clks;
        return true;
    });

    regs->mtimecmp0hi.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        if (!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        } 
        return false;
    });
    
    regs->mtimecmp0lo.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        if (!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        } 
        return false;
    });

    regs->msip0.set_write_cb([this](const scc::sc_register<uint32_t> &reg, const uint32_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        if (!regs->in_reset()) {
            reg.put(data);
            msip_int_o->write(data&1);
            return true;
        } 
        return false;
    });
    SC_METHOD(update_mtime);
    sensitive << mtime_evt;
    dont_initialize(); 
}
void aclint::reset_cb(){
    if (rst_i.read()) {
        regs->reset_start();
    } else if(rst_i.event()){
        regs->reset_stop();
    }
    update_mtime();
}

void aclint::update_mtime(){
    if(clk_period > SC_ZERO_TIME){
        uint64_t mtime = static_cast<uint64_t>(regs->r_mtime_hi)<<32 | regs->r_mtime_lo;
        uint64_t mtimecmp = static_cast<uint64_t>(regs->r_mtimecmp0hi)<<32 | regs->r_mtimecmp0lo;
        //update mtime register
        uint64_t elapsed_clks = (sc_time_stamp() - last_updt)/clk_period;
        last_updt += elapsed_clks * clk_period;
        if(elapsed_clks){
            mtime += elapsed_clks;
            regs->mtime_hi = mtime >> 32;
            regs->mtime_lo = static_cast<uint32_t>(mtime);
            }

        //check for and handle interrupts
        mtime_evt.cancel();
        uint64_t smallest = std::numeric_limits<uint64_t>::max();
            if(mtimecmp <= mtime)
                mtime_int_o.write(true);
            else{
                mtime_int_o.write(false);
                smallest = std::min(smallest, mtimecmp);
            }      
        
        if(smallest < std::numeric_limits<uint64_t>::max() && smallest > 0){
            sc_time nexttrigger = clk_period * (smallest-mtime);
            mtime_evt.notify(nexttrigger);
        }
    }
    else last_updt = sc_time_stamp();
}
aclint::~aclint() = default;
} /* namespace sifive */
} /* namespace vpvper */
