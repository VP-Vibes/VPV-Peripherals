#include "aclint.h"
#include "gen/aclint_regs.h"
#include <cstdint>
#include <limits>
#include <sysc/kernel/sc_time.h>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace scc;
const int lfclk_mutiplier = 10; // hardcoded for unit test

aclint::aclint(sc_module_name nm, size_t num_cpus)
: sc_module(nm)
, tlm_target<>(clk_period)
, mtime_int_o{"mtime_int_o", num_cpus}
, msip_int_o{"msip_int_o", num_cpus}
, regs(make_unique<aclint_regs>("regs", num_cpus)) {
    SC_HAS_PROCESS(aclint);
    regs->registerResources(*this);

    SC_METHOD(reset_cb);
    sensitive << rst_i;
    regs->mtime.set_read_cb([this](const sc_register<uint64_t>& reg, uint64_t& data, sc_time& d) -> bool {
        uint64_t elapsed_clks = mtime_clk_period.value() ? (sc_time_stamp() + d - last_updt) / mtime_clk_period : 0;
        data = regs->in_reset() ? 0 : regs->r_mtime + elapsed_clks;
        return true;
    });
    regs->mtime.set_write_cb([this](const sc_register<uint64_t>& reg, const uint64_t data, sc_time& d) -> bool {
        if(!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        }
        return false;
    });
    auto mtime_write_cb_with_wait = [this](const sc_register<uint64_t>& reg, const uint64_t data, sc_time& d) -> bool {
        if(d.value())
            wait(d);
        if(!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        }
        return false;
    };
    for(auto i = 0u; i < regs->mtimecmp.size(); ++i) {
        regs->mtimecmp[i].set_write_cb(mtime_write_cb_with_wait);
        regs->msip[i].set_write_cb([this, i](const sc_register<uint32_t>& reg, const uint32_t data, sc_time& d) -> bool {
            if(d.value())
                wait(d);
            if(!regs->in_reset()) {
                reg.put(data);
                if(i < this->msip_int_o.size())
                    this->msip_int_o[i].write(reg.get() & 0x1);
                return true;
            }
            return false;
        });
    }
    SC_METHOD(update_mtime);
    sensitive << mtime_evt;
    dont_initialize();
    SC_METHOD(update_mtime_clk);
    sensitive << mtime_clk_i;
}
void aclint::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
    } else if(rst_i.event()) {
        regs->reset_stop();
    }
    update_mtime();
}

void aclint::update_mtime() {
    if(sc_core::sc_time_stamp() > SC_ZERO_TIME && mtime_clk_period > SC_ZERO_TIME) {
        // update mtime register
        uint64_t elapsed_clks = (sc_time_stamp() - last_updt) / mtime_clk_period;
        last_updt += elapsed_clks * mtime_clk_period;
        if(elapsed_clks) {
            regs->r_mtime += elapsed_clks;
            if(mtime_o.get_interface())
                mtime_o->write(regs->r_mtime);
        }
        // check all mtimecmp register if there is an interrupt
        uint64_t mtimecmp_min = std::numeric_limits<uint64_t>::max();
        for(auto i = 0u; i < regs->mtimecmp.size(); ++i) {
            // check for and handle interrupts
            uint64_t smallest = std::numeric_limits<uint64_t>::max();
            mtime_int_o[i].write(regs->r_mtimecmp[i] <= regs->r_mtime);
            mtimecmp_min = std::min(mtimecmp_min, regs->r_mtimecmp[i]);
        }
        // calculate next invocation
        mtime_evt.cancel();
        if(mtime_o.get_interface()) {
            mtime_evt.notify(mtime_clk_period);
        } else if(mtimecmp_min > regs->r_mtime) {
            sc_time nexttrigger = mtime_clk_period * (mtimecmp_min - regs->r_mtime);
            mtime_evt.notify(nexttrigger);
        }
    }
    last_updt = sc_time_stamp();
}
aclint::~aclint() = default;
} // namespace minres
} /* namespace vpvper */
