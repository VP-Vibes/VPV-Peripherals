/*
 * Copyright (c) 2019 -2023 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _ACLINT_REGS_H_
#define _ACLINT_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>

namespace vpvper {
namespace minres {

enum class device_type{MTIMER, MSIP, SSWI};

template <unsigned MTIMECMP_COUNT, unsigned MSIP_COUNT, unsigned SSWI_COUNT>
class aclint_regs : public sc_core::sc_module, public scc::resetable {

    static_assert(MTIMECMP_COUNT < 4096);
    static_assert(MSIP_COUNT < 4096);
    static_assert(SSWI_COUNT < 4096);

    public:
        // storage declarations
        uint64_t r_mtime;
        std::array<uint64_t, MTIMECMP_COUNT> r_mtimecmp{std::numeric_limits<uint64_t>::max()};
        std::array<uint32_t, MSIP_COUNT> r_msip;
        std::array<uint32_t, SSWI_COUNT> r_sswi;
        // register declarations
        scc::sc_register<uint64_t> mtime;
        scc::sc_register_indexed<uint64_t, MTIMECMP_COUNT> mtimecmp;
        scc::sc_register_indexed<uint32_t, MSIP_COUNT> msip;
        scc::sc_register_indexed<uint32_t, SSWI_COUNT> sswi;

        aclint_regs(sc_core::sc_module_name nm);

        //initialize registers
        template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target, device_type device);
    };
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////
template <unsigned MTIMECMP_COUNT, unsigned MSIP_COUNT, unsigned SSWI_COUNT>
inline vpvper::minres::aclint_regs<MTIMECMP_COUNT,MSIP_COUNT,SSWI_COUNT>::aclint_regs(sc_core::sc_module_name nm)
:sc_core::sc_module(nm)
,NAMED(mtime, r_mtime, 0, *this)
,NAMED(mtimecmp, r_mtimecmp, std::numeric_limits<uint64_t>::max(), *this)
,NAMED(msip, r_msip, 0, *this, 0xffffffff, 0x00000001)
,NAMED(sswi, r_sswi, 0, *this, 0xffffffff, 0x00000001)
{}

template <unsigned MTIMECMP_COUNT, unsigned MSIP_COUNT, unsigned SSWI_COUNT>
template <unsigned BUSWIDTH> inline void vpvper::minres::aclint_regs<MTIMECMP_COUNT,MSIP_COUNT,SSWI_COUNT>::registerResources(scc::tlm_target<BUSWIDTH> &target, device_type device){
    switch(device){
        case device_type::MTIMER:
            target.addResource(mtime,0x0000UL);
            target.addResource(mtimecmp, 0x0008UL);
            break;
        case device_type::MSIP:
            target.addResource(msip, 0x0000UL);
            break;
        case device_type::SSWI:
            target.addResource(sswi, 0x0000UL);    
        break;
    }
}
#endif // _ACLINT_REGS_H_
