/*
 * spimem.h
 *
 *  Created on: Mar 2, 2025
 *      Author: eyck
 */

#ifndef VPVPER_GENERIC_SPI_MEM_H_
#define VPVPER_GENERIC_SPI_MEM_H_

#include <cci_configuration>
#include <scc/utilities.h>
#include <spi/spi_tlm.h>
#include <sysc/kernel/sc_module.h>
#include <tlm/nw/target_mixin.h>
#include <util/sparse_array.h>

namespace vpvper {
namespace generic {

class spi_mem : public sc_core::sc_module {
public:
    tlm::nw::target_mixin<spi::spi_pkt_target_socket<>> tsck{"tsck"};

    cci::cci_param<unsigned> slave_id{"slave_id", 0, "SPI slave id aka id of the slave select"};

    cci::cci_param<std::string> mem_file{"mem_file", "", "Memory content pre-load file"};

    spi_mem(const sc_core::sc_module_name& nm);

    virtual ~spi_mem();

private:
    util::sparse_array<uint8_t, 4_GB> mem;
};

} /* namespace generic */
} // namespace vpvper
#endif /* VPVPER_GENERIC_SPI_MEM_H_ */
