#include "aclint.h"
#include "aclint_regs.h"

namespace vpvper {
namespace minres {
using namespace sc_core;
const int lfclk_mutiplier = 1;

aclint::aclint(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMEDD(regs, aclint_regs<>)
{
     SC_HAS_PROCESS(aclint);
    regs->registerResources(*mtime_target, device_type::MTIME);
    regs->registerResources(*msip_target, device_type::MSIP);
    regs->registerResources(*sswi_target, device_type::SSWI);
    SC_METHOD(clock_cb);
    sensitive << lfclk_i;
    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();

    regs->mtime.set_write_cb([this](scc::sc_register<uint64_t> &reg, uint64_t data, sc_core::sc_time& d) -> bool{
        return false;
    });
    regs->mtime.set_read_cb([this](const scc::sc_register<uint64_t> &reg, uint64_t data, sc_core::sc_time& d) -> bool{ 
        uint64_t elapsed_clks = (sc_time_stamp() + d - last_updt)/clk;
        data = regs->r_mtime + elapsed_clks;
        return true;
    });
    regs->mtimecmp.set_write_cb([this](size_t idx, scc::sc_register<uint64_t> &reg, uint64_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        if (!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
        }
        return true;
    });
    regs->msip.set_write_cb([this](size_t idx, scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);
        reg.put(data);
        msip_int_o.write(regs->r_msip[idx]);
        return true;
    });
    regs->sswi.set_write_cb([this](size_t idx, scc::sc_register<uint32_t> &reg, uint32_t data, sc_core::sc_time& d) -> bool{
        if(d.value()) wait(d);        
        reg.put(data);
        sswi_int_o.write(regs->r_sswi[idx]);
        return true;
    });
    SC_METHOD(update_mtime);
    sensitive << mtime_evt;
    dont_initialize(); 
}
void aclint::reset_cb(){
    if (rst_i.read()) {
        regs->reset_start();
        msip_int_o.write(false);
        mtime_int_o.write(false);
    } else
        regs->reset_stop();
}
void aclint::clock_cb(){
    update_mtime();
    clk = lfclk_i.read();
    update_mtime();
}
void aclint::update_mtime(){
    if(clk > SC_ZERO_TIME){

        //update mtime register
        uint64_t elapsed_clks = (sc_time_stamp() - last_updt)/clk;
        last_updt += elapsed_clks * clk;
        if(elapsed_clks) regs->r_mtime += elapsed_clks;

        //check for and handle interrupts
        mtime_evt.cancel();
        uint64_t smallest = 0;
        if(std::all_of(regs->r_mtimecmp.begin(), regs->r_mtimecmp.end(), [this, &smallest](uint64_t value){if(value <= smallest) smallest = value; return value > regs->r_mtime;})){
            mtime_int_o.write(false);
            sc_time nexttrigger = lfclk_i * lfclk_mutiplier * (smallest-regs->r_mtime);
            mtime_evt.notify(nexttrigger);
        }
        else{
            mtime_int_o.write(true);
        }

    }
    else last_updt = sc_time_stamp();
}
} /* namespace sifive */
} /* namespace vpvper */
