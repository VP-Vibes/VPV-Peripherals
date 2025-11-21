#include "aclint.h"
#include "gen/aclint_regs.h"
#include <cstdint>

namespace vpvper {
namespace minres {
using namespace sc_core;
using namespace scc;
const int lfclk_mutiplier = 10; // hardcoded for unit test

aclint::aclint(sc_module_name nm)
: sc_module(nm)
, tlm_target<>(clk_period)
, regs(make_unique<aclint_regs>("regs")) {
    SC_HAS_PROCESS(aclint);
    regs->registerResources(*this);

    SC_METHOD(reset_cb);
    sensitive << rst_i;
    regs->mtime_hi.set_read_cb([this](const sc_register<uint32_t>& reg, uint32_t& data, sc_time& d) -> bool {
        uint64_t elapsed_clks = mtime_clk_period.value() ? (sc_time_stamp() + d - last_updt) / mtime_clk_period : 0;
        data = regs->in_reset() ? 0 : regs->r_mtime_hi + (elapsed_clks >> 32);
        return true;
    });
    regs->mtime_lo.set_read_cb([this](const sc_register<uint32_t>& reg, uint32_t& data, sc_time& d) -> bool {
        uint64_t elapsed_clks = mtime_clk_period.value() ? (sc_time_stamp() + d - last_updt) / mtime_clk_period : 0;
        data = regs->in_reset() ? 0 : regs->r_mtime_lo + elapsed_clks;
        return true;
    });
    auto write_cb = [this](const sc_register<uint32_t>& reg, const uint32_t data, sc_time& d) -> bool {
        if(!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        }
        return false;
    };
    regs->mtime_hi.set_write_cb(write_cb);
    regs->mtime_lo.set_write_cb(write_cb);
    auto write_cb_with_wait = [this](const sc_register<uint32_t>& reg, const uint32_t data, sc_time& d) -> bool {
        if(d.value())
            wait(d);
        if(!regs->in_reset()) {
            reg.put(data);
            this->update_mtime();
            return true;
        }
        return false;
    };
    regs->mtimecmp0hi.set_write_cb(write_cb_with_wait);
    regs->mtimecmp0lo.set_write_cb(write_cb_with_wait);
    regs->msip0.set_write_cb(write_cb_with_wait);
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
    if(mtime_clk_period > SC_ZERO_TIME) {
        uint64_t mtime = static_cast<uint64_t>(regs->r_mtime_hi) << 32 | regs->r_mtime_lo;
        uint64_t mtimecmp = static_cast<uint64_t>(regs->r_mtimecmp0hi) << 32 | regs->r_mtimecmp0lo;
        // update mtime register
        uint64_t elapsed_clks = (sc_time_stamp() - last_updt) / mtime_clk_period;
        last_updt += elapsed_clks * mtime_clk_period;
        if(elapsed_clks) {
            mtime += elapsed_clks;
            regs->mtime_hi = mtime >> 32;
            regs->mtime_lo = static_cast<uint32_t>(mtime);
            if(mtime_o.get_interface())
                mtime_o->write(mtime);
        }

        // check for and handle interrupts
        uint64_t smallest = std::numeric_limits<uint64_t>::max();
        mtime_int_o.write(mtimecmp <= mtime);
        mtime_evt.cancel();
        if(mtime_o.get_interface()) {
            mtime_evt.notify(mtime_clk_period);
        } else if(mtimecmp > mtime) {
            sc_time nexttrigger = mtime_clk_period * (mtimecmp - mtime);
            mtime_evt.notify(nexttrigger);
        }
    } else
        last_updt = sc_time_stamp();
}
aclint::~aclint() = default;
} // namespace minres
} /* namespace vpvper */
