#include "aclint.h"
#include "aclint_regs.h"

namespace vpvper {
namespace minres {
using namespace sc_core;
const int lfclk_mutiplier = 10; //hardcoded for unit test

aclint::aclint(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
,regs(scc::make_unique<aclint_regs<2,2,2>>("regs")) //hardcoded for unit test
{
    SC_HAS_PROCESS(aclint);
    regs->registerResources(mtime_target, device_type::MTIMER);
    regs->registerResources(msip_target, device_type::MSIP);
    regs->registerResources(sswi_target, device_type::SSWI);
    mtime.bind(mtime_target.socket);
    msip.bind(msip_target.socket);
    sswi.bind(sswi_target.socket);

    SC_METHOD(tl_clock_cb);
    sensitive << tlclk_i;
    SC_METHOD(clock_reset_cb);
    sensitive << lfclk_i << rst_i;


    regs->mtime.set_write_cb([this](scc::sc_register<uint64_t> &reg, uint64_t data, sc_core::sc_time& d) -> bool{
        return false;
    });
    regs->mtime.set_read_cb([this](const scc::sc_register<uint64_t> &reg, uint64_t &data, sc_core::sc_time& d) -> bool{ 
        uint64_t elapsed_clks = (sc_time_stamp() + d - last_updt)/clk;
        data = regs->in_reset() ? 0 : regs->r_mtime + elapsed_clks;
        return true;
    });
    regs->mtimecmp.set_write_cb([this](size_t idx, scc::sc_register<uint64_t> &reg, uint64_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        if (!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        } 
        return false;
    });
    regs->msip.set_write_cb([this](size_t idx, scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time& d) -> bool{
        if (!regs->in_reset()) {
            if(d.value()) wait(d);
            reg.put(data);
            msip_int_o[idx].write(data&1);
            return true;
        }
        return false;
    });
    regs->sswi.set_write_cb([this](size_t idx, scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time& d) -> bool{
        if (!regs->in_reset()) {
            if(d.value()) wait(d);        
            reg.put(data);
            sswi_int_o[idx].write(data&1);
            return true;
        }
        return false;
    });
    SC_METHOD(update_mtime);
    sensitive << mtime_evt;
    dont_initialize(); 
}
void aclint::clock_reset_cb(){
    if (rst_i.read()) {
        regs->reset_start();
        for(auto &p : msip_int_o) p.write(false);
        for(auto &p : sswi_int_o) p.write(false);
    } else if(rst_i.event()){
        regs->reset_stop();
    }
    update_mtime();
    clk = lfclk_i.read();
    update_mtime();
}
void aclint::tl_clock_cb(){
    tlclk = tlclk_i.read();
}
void aclint::update_mtime(){
    if(clk > SC_ZERO_TIME){

        //update mtime register
        uint64_t elapsed_clks = (sc_time_stamp() - last_updt)/clk;
        last_updt += elapsed_clks * clk;
        if(elapsed_clks) regs->r_mtime += elapsed_clks;

        //check for and handle interrupts
        mtime_evt.cancel();
        uint64_t smallest = std::numeric_limits<uint64_t>::max();
        for(auto i=0; i < mtime_int_o.size(); ++i){
            if(regs->r_mtimecmp[i] <= regs->r_mtime)
                mtime_int_o[i].write(true);
            else{
                mtime_int_o[i].write(false);
                smallest = std::min(smallest, regs->r_mtimecmp[i]);
            }      
        }
        if(smallest < std::numeric_limits<uint64_t>::max() && smallest > 0){
            sc_time nexttrigger = lfclk_i * (smallest-regs->r_mtime);
            mtime_evt.notify(nexttrigger);
        }
    }
    else last_updt = sc_time_stamp();
}
aclint::~aclint() = default;
} /* namespace sifive */
} /* namespace vpvper */
