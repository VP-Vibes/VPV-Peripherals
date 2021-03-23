/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _PWM_REGS_H_
#define _PWM_REGS_H_

#include <scc/register.h>
#include <scc/tlm_target.h>
#include <scc/utilities.h>
#include <util/bit_field.h>

namespace vpvper {
namespace sifive {

class pwm_regs : public sc_core::sc_module, public scc::resetable {
public:
    // storage declarations
    BEGIN_BF_DECL(pwmcfg_t, uint32_t);
    BF_FIELD(pwmscale, 0, 4);
    BF_FIELD(pwmsticky, 8, 1);
    BF_FIELD(pwmzerocmp, 9, 1);
    BF_FIELD(pwmdeglitch, 10, 1);
    BF_FIELD(pwmenalways, 12, 1);
    BF_FIELD(pwmenoneshot, 13, 1);
    BF_FIELD(pwmcmp0center, 16, 1);
    BF_FIELD(pwmcmp1center, 17, 1);
    BF_FIELD(pwmcmp2center, 18, 1);
    BF_FIELD(pwmcmp3center, 19, 1);
    BF_FIELD(pwmcmp0gang, 24, 1);
    BF_FIELD(pwmcmp1gang, 25, 1);
    BF_FIELD(pwmcmp2gang, 26, 1);
    BF_FIELD(pwmcmp3gang, 27, 1);
    BF_FIELD(pwmcmp0ip, 28, 1);
    BF_FIELD(pwmcmp1ip, 29, 1);
    BF_FIELD(pwmcmp2ip, 30, 1);
    BF_FIELD(pwmcmp3ip, 31, 1);
    END_BF_DECL() r_pwmcfg;

    BEGIN_BF_DECL(pwmcount_t, uint32_t);
    BF_FIELD(pwmcount, 0, 31);
    END_BF_DECL() r_pwmcount;

    BEGIN_BF_DECL(pwms_t, uint32_t);
    BF_FIELD(pwms, 0, 16);
    END_BF_DECL() r_pwms;

    BEGIN_BF_DECL(pwmcmp0_t, uint32_t);
    BF_FIELD(pwmcmp0, 0, 16);
    END_BF_DECL() r_pwmcmp0;

    BEGIN_BF_DECL(pwmcmp1_t, uint32_t);
    BF_FIELD(pwmcmp0, 0, 16);
    END_BF_DECL() r_pwmcmp1;

    BEGIN_BF_DECL(pwmcmp2_t, uint32_t);
    BF_FIELD(pwmcmp0, 0, 16);
    END_BF_DECL() r_pwmcmp2;

    BEGIN_BF_DECL(pwmcmp3_t, uint32_t);
    BF_FIELD(pwmcmp0, 0, 16);
    END_BF_DECL() r_pwmcmp3;

    // register declarations
    scc::sc_register<pwmcfg_t> pwmcfg;
    scc::sc_register<pwmcount_t> pwmcount;
    scc::sc_register<pwms_t> pwms;
    scc::sc_register<pwmcmp0_t> pwmcmp0;
    scc::sc_register<pwmcmp1_t> pwmcmp1;
    scc::sc_register<pwmcmp2_t> pwmcmp2;
    scc::sc_register<pwmcmp3_t> pwmcmp3;

    pwm_regs(sc_core::sc_module_name nm);

    template <unsigned BUSWIDTH = 32> void registerResources(scc::tlm_target<BUSWIDTH> &target);
};
} /* namespace sifive */
} /* namespace vpvper */

//////////////////////////////////////////////////////////////////////////////
// member functions
//////////////////////////////////////////////////////////////////////////////

inline vpvper::sifive::pwm_regs::pwm_regs(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, NAMED(pwmcfg, r_pwmcfg, 0, *this)
, NAMED(pwmcount, r_pwmcount, 0, *this)
, NAMED(pwms, r_pwms, 0, *this)
, NAMED(pwmcmp0, r_pwmcmp0, 0, *this)
, NAMED(pwmcmp1, r_pwmcmp1, 0, *this)
, NAMED(pwmcmp2, r_pwmcmp2, 0, *this)
, NAMED(pwmcmp3, r_pwmcmp3, 0, *this) {}

template <unsigned BUSWIDTH> inline void vpvper::sifive::pwm_regs::registerResources(scc::tlm_target<BUSWIDTH> &target) {
    target.addResource(pwmcfg, 0x0UL);
    target.addResource(pwmcount, 0x8UL);
    target.addResource(pwms, 0x10UL);
    target.addResource(pwmcmp0, 0x20UL);
    target.addResource(pwmcmp1, 0x24UL);
    target.addResource(pwmcmp2, 0x28UL);
    target.addResource(pwmcmp3, 0x2cUL);
}

#endif // _PWM_REGS_H_
