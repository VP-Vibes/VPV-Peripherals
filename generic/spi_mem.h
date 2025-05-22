/*
 * spimem.h
 *
 *  Created on: Mar 2, 2025
 *      Author: eyck
 */

#ifndef VPVPER_GENERIC_SPI_MEM_H_
#define VPVPER_GENERIC_SPI_MEM_H_

#include <cci_configuration>
#include <scc/memory.h>
#include <scc/utilities.h>
#include <spi/spi_tlm.h>
#include <sysc/kernel/sc_module.h>
#include <tlm/nw/initiator_mixin.h>
#include <tlm/nw/target_mixin.h>
#include <tlm/scc/initiator_mixin.h>
#include <util/sparse_array.h>

namespace vpvper {
namespace generic {

class spi_mem : public sc_core::sc_module {
public:
    tlm::nw::target_mixin<spi::spi_pkt_target_socket<>> spi_t{"spi_t"};

    cci::cci_param<unsigned> slave_id{"slave_id", 0, "SPI slave id aka id of the slave select"};

    cci::cci_param<std::string> mem_file{"mem_file", "", "Memory content pre-load file"};

    cci::cci_param<int64_t> mem_offset{"mem_offset", 0, "Address offset to be used when loading the mem_file"};

    spi_mem(const sc_core::sc_module_name& nm);

    virtual ~spi_mem();

private:
    scc::peq<unsigned> cmd;
    void start_of_simulation() override;
    void cmd_cb();
    scc::memory<256_MB, scc::LT> mem{"mem"};
    tlm::scc::initiator_mixin<tlm::tlm_initiator_socket<scc::LT>> sckt{"sckt"};
    bool wren{false};
};

} /* namespace generic */
} // namespace vpvper
#endif /* VPVPER_GENERIC_SPI_MEM_H_ */
