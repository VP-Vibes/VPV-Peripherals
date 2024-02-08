/*
* Copyright (c) 2024 MINRES Technologies GmbH
*
* Generated at 2024-02-08 12:22:11 UTC 
* by peakrdl_mnrs version 1.2.1
*/

#ifndef _APB3ACLINT_H_
#define _APB3ACLINT_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace minres {

class Apb3AClint_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(msip0_t, uint32_t);
        BF_FIELD(msip, 0, 1);
        END_BF_DECL() r_msip0;
    
        uint32_t r_mtimecmp0lo;
    
        uint32_t r_mtimecmp0hi;
    
        uint32_t r_mtime_lo;
    
        uint32_t r_mtime_hi;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<msip0_t> msip0;
        scc::sc_register<uint32_t> mtimecmp0lo;
        scc::sc_register<uint32_t> mtimecmp0hi;
        scc::sc_register<uint32_t> mtime_lo;
        scc::sc_register<uint32_t> mtime_hi;

    Apb3AClint_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline Apb3AClint_regs::Apb3AClint_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(msip0, r_msip0, 0, *this)
, NAMED(mtimecmp0lo, r_mtimecmp0lo, 0, *this)
, NAMED(mtimecmp0hi, r_mtimecmp0hi, 0, *this)
, NAMED(mtime_lo, r_mtime_lo, 0, *this)
, NAMED(mtime_hi, r_mtime_hi, 0, *this) {}

template <unsigned BUSWIDTH> inline void Apb3AClint_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(msip0, 0x0UL);
    target.addResource(mtimecmp0lo, 0x4000UL);
    target.addResource(mtimecmp0hi, 0x4004UL);
    target.addResource(mtime_lo, 0xbff8UL);
    target.addResource(mtime_hi, 0xbffcUL);
}
}//namespace minres
}//namespace vpvper
#endif // _APB3ACLINT_H_