/*
 * Copyright (c) 2019 -2021 MINRES Technolgies GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef _SPI_H_
#define _SPI_H_

#include <sysc/utils/sc_vector.h>
#include <tlm/scc/tlm_signal.h>

namespace vpvper {
namespace sifive {

namespace spi_impl {
class beh;
class rtl;
}

class spi : public sc_core::sc_module {
public:
    template <typename TYPE>
    static std::unique_ptr<spi> create(sc_core::sc_module_name nm);

    template <typename T> using tlm_in = tlm::scc::tlm_signal_opt_target_socket<T>;
    template <typename T> using tlm_out = tlm::scc::tlm_signal_opt_initiator_socket<T>;

    tlm::tlm_target_socket<> socket;
    sc_core::sc_in<sc_core::sc_time> clk_i;
    sc_core::sc_in<bool> rst_i;
    tlm_out<bool> sck_o;
    tlm_out<bool> mosi_o;
    tlm_in<bool> miso_i;
    sc_core::sc_vector<tlm_out<bool>> scs_o;

    sc_core::sc_out<bool> irq_o;

    spi(spi &other) = delete;

    spi(spi &&other) = delete;

    spi &operator=(spi &other) = delete;

    spi &operator=(spi &&other) = delete;

    ~spi() override = default;

protected:
    spi(sc_core::sc_module_name nm)
    : sc_core::sc_module(nm)
    , NAMED(clk_i)
    , NAMED(rst_i)
    , NAMED(sck_o)
    , NAMED(mosi_o)
    , NAMED(miso_i)
    , NAMED(scs_o, 4)
    , NAMED(irq_o){};
};

} /* namespace sifive */
} /* namespace vpvper */

#endif /* _SPI_H_ */
