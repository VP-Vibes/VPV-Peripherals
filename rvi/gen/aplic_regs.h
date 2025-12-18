/*
* Copyright (c) 2023 - 2025 MINRES Technologies GmbH
*
* SPDX-License-Identifier: Apache-2.0
*
* Generated at 2025-12-23 11:13:45 UTC 
* by peakrdl_mnrs version 1.2.10
*/

#ifndef _APLIC_H_
#define _APLIC_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace rvi {

class aplic_regs : public sc_core::sc_module, public scc::resetable {
public:
    //////////////////////////////////////////////////////////////////////////////
    // storage declarations
    //////////////////////////////////////////////////////////////////////////////
        BEGIN_BF_DECL(domaincfg_t, uint32_t);
        BF_FIELD(deliverymode, 2, 1);
        BF_FIELD(interruptenable, 8, 1);
        END_BF_DECL() r_domaincfg;
    
        BEGIN_BF_DECL(sourcecfg_t, uint32_t);
        BF_FIELD(sourcemode, 0, 3);
        END_BF_DECL();
        std::array<sourcecfg_t, 1023> r_sourcecfg; 
    
        std::array<uint32_t, 32> r_pending; 
    
        uint32_t r_pendingnum;
    
        std::array<uint32_t, 32> r_clrpending; 
    
        uint32_t r_clripnum;
    
        std::array<uint32_t, 32> r_enabled; 
    
        uint32_t r_enablednum;
    
        std::array<uint32_t, 32> r_clrenabled; 
    
        uint32_t r_clrenablednum;
    
        BEGIN_BF_DECL(targetcfg_t, uint32_t);
        BF_FIELD(interruptpriority, 0, 8);
        END_BF_DECL();
        std::array<targetcfg_t, 1023> r_targetcfg; 
    
        uint32_t r_delivery;
    
        uint32_t r_threshold;
    
        BEGIN_BF_DECL(top_t, uint32_t);
        BF_FIELD(interruptpriority, 0, 8);
        BF_FIELD(interruptidentity, 16, 10);
        END_BF_DECL() r_top;
    
        BEGIN_BF_DECL(claim_t, uint32_t);
        BF_FIELD(interruptpriority, 0, 8);
        BF_FIELD(interruptidentity, 16, 10);
        END_BF_DECL() r_claim;
    

    //////////////////////////////////////////////////////////////////////////////
    // register declarations
    //////////////////////////////////////////////////////////////////////////////
    
        scc::sc_register<domaincfg_t> domaincfg;
            scc::sc_register_indexed<sourcecfg_t, 1023> sourcecfg; 
            scc::sc_register_indexed<uint32_t, 32> pending; 
        scc::sc_register<uint32_t> pendingnum;
            scc::sc_register_indexed<uint32_t, 32> clrpending; 
        scc::sc_register<uint32_t> clripnum;
            scc::sc_register_indexed<uint32_t, 32> enabled; 
        scc::sc_register<uint32_t> enablednum;
            scc::sc_register_indexed<uint32_t, 32> clrenabled; 
        scc::sc_register<uint32_t> clrenablednum;
            scc::sc_register_indexed<targetcfg_t, 1023> targetcfg; 
        scc::sc_register<uint32_t> delivery;
        scc::sc_register<uint32_t> threshold;
        scc::sc_register<top_t> top;
        scc::sc_register<claim_t> claim;

    aplic_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};


//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline aplic_regs::aplic_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(domaincfg, r_domaincfg, 0, *this)
, NAMED(sourcecfg, r_sourcecfg, 0, *this)
, NAMED(pending, r_pending, 0, *this)
, NAMED(pendingnum, r_pendingnum, 0, *this)
, NAMED(clrpending, r_clrpending, 0, *this)
, NAMED(clripnum, r_clripnum, 0, *this)
, NAMED(enabled, r_enabled, 0, *this)
, NAMED(enablednum, r_enablednum, 0, *this)
, NAMED(clrenabled, r_clrenabled, 0, *this)
, NAMED(clrenablednum, r_clrenablednum, 0, *this)
, NAMED(targetcfg, r_targetcfg, 0, *this)
, NAMED(delivery, r_delivery, 0, *this)
, NAMED(threshold, r_threshold, 0, *this)
, NAMED(top, r_top, 0, *this)
, NAMED(claim, r_claim, 0, *this) {}

template <unsigned BUSWIDTH> inline void aplic_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(domaincfg, 0x0UL);
    target.addResource(sourcecfg, 0x4UL);
    target.addResource(pending, 0x1c00UL);
    target.addResource(pendingnum, 0x1cdcUL);
    target.addResource(clrpending, 0x1d00UL);
    target.addResource(clripnum, 0x1ddcUL);
    target.addResource(enabled, 0x1e00UL);
    target.addResource(enablednum, 0x1edcUL);
    target.addResource(clrenabled, 0x1f00UL);
    target.addResource(clrenablednum, 0x1fdcUL);
    target.addResource(targetcfg, 0x3004UL);
    target.addResource(delivery, 0x4000UL);
    target.addResource(threshold, 0x4008UL);
    target.addResource(top, 0x4018UL);
    target.addResource(claim, 0x401cUL);
}
}//namespace rvi
}//namespace vpvper
#endif // _APLIC_H_