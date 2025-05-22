/*
 * spimem.cpp
 *
 *  Created on: Mar 2, 2025
 *      Author: eyck
 */

#include "spi_mem.h"
#include <tlm/scc/tlm_extensions.h>
#include <elfio/elfio.hpp>
#include <util/logging.h>
#include <util/ihex_parser.h>
#include <limits>

namespace vpvper {
namespace generic {
namespace {
bool read_elf_file(std::string const& name, uint8_t expected_elf_class, std::function<bool(uint64_t, uint64_t, const char*)> wr_func) {
    // Create elfio reader
    ELFIO::elfio reader;
    // Load ELF data
    if(reader.load(name)) {
        // check elf properties
        if(reader.get_class() != expected_elf_class) {
            CPPLOG(ERR) << "ISA missmatch, selected XLEN does not match supplied file ";
            return false;
        }
        if(reader.get_type() != ELFIO::ET_EXEC)
            return false;
        if(reader.get_machine() != ELFIO::EM_RISCV)
            return false;
        auto entry_address = reader.get_entry();
        for(const auto& pseg : reader.segments) {
            const auto fsize = pseg->get_file_size(); // 0x42c/0x0
            const auto seg_data = pseg->get_data();
            const auto type = pseg->get_type();
            if(type == ELFIO::PT_LOAD && fsize > 0) {
                if(!wr_func(pseg->get_physical_address(), fsize,  seg_data))
                    CPPLOG(ERR) << "problem writing " << fsize << "bytes to 0x" << std::hex << pseg->get_physical_address();
            }
        }
        return true;
    }
    return false;
}
}

spi_mem::spi_mem(sc_core::sc_module_name const& nm) {
    spi_t.register_nb_transport_fw(
        [this](spi::spi_packet_payload& trans, tlm::tlm_phase& ph, sc_core::sc_time& t) -> tlm::tlm_sync_enum {
            if(ph == tlm::nw::INDICATION) {
                t += 8 * trans.get_sender_clk_period();
                cmd.notify(*trans.get_data().data(), sc_core::SC_ZERO_TIME);
            }
            return tlm::TLM_ACCEPTED;
        });
    spi_t.register_b_transport([this](spi::spi_packet_payload& gp, sc_core::sc_time& t) {
        auto gp_ext = gp.get_extension<tlm::scc::tlm_payload_extension>();
        if(!gp_ext) {
            gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
            return;
        }
        sckt->b_transport(*gp_ext->gp, t);
    });
    sckt(mem.target);
    SC_HAS_PROCESS(spi_mem);
    SC_THREAD(cmd_cb);
}

spi_mem::~spi_mem() = default;

void spi_mem::start_of_simulation() {
    if(mem_file.get_value().length()) {
        std::ifstream ifs{mem_file.get_value()};
        if(ifs.is_open()) {
            auto pos = ifs.tellg();
            std::array<char, 4> buf;
            ifs.read(buf.data(), 4 );
            ifs.seekg(pos);
            if(buf[0]==0x7f && buf[1]==0x45  && buf[2]==0x4c  && buf[3]==0x46 ){
                if(!read_elf_file(mem_file.get_value(), ELFIO::ELFCLASS32, [this](uint64_t addr, uint64_t size, const char* data) -> bool{
                    tlm::tlm_generic_payload gp;
                    gp.set_command(tlm::TLM_WRITE_COMMAND);
                    gp.set_address(addr+mem_offset.get_value());
                    gp.set_data_length(size);
                    gp.set_data_ptr(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(data)));
                    gp.set_streaming_width(size);
                    return sckt->transport_dbg(gp)==size;
                }))
                    SCCERR(SCMOD)<<"Could not load ELF file "<<mem_file.get_value();
            } else if(buf[0]==':') {
                std::vector<uint8_t> write_data;
                uint64_t last_addr{std::numeric_limits<uint64_t>::max()};
                if(!util::ihex_parser::parse(ifs, [this, &write_data, &last_addr](uint64_t addr, uint64_t size, const uint8_t* data) -> bool{
                    tlm::tlm_generic_payload gp;
                    gp.set_command(tlm::TLM_WRITE_COMMAND);
                    gp.set_address(addr+mem_offset.get_value());
                    gp.set_data_length(size);
                    gp.set_data_ptr(const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(data)));
                    gp.set_streaming_width(size);
                    return sckt->transport_dbg(gp)==size;
                }))
                    SCCERR(SCMOD)<<"Could not load IHEX file "<<mem_file.get_value();
            }
        } else
            SCCERR(SCMOD)<<"Could not open file "<<mem_file.get_value();
    }
}

void spi_mem::cmd_cb() {
    wait(sc_core::SC_ZERO_TIME);
    while(true) {
        auto e = cmd.get();
        switch(e) {
        case 0x90: { // S25FL128S identify
            std::vector<uint8_t> addr(3);
            for(auto& a : addr)
                a = cmd.get();
            auto gp0 = spi::spi_pkt_mm::get().allocate();
            gp0->get_data().push_back(0x01);
            tlm::tlm_phase ph = tlm::nw::RESPONSE;
            sc_core::sc_time t;
            spi_t->nb_transport_bw(*gp0, ph, t);
            auto gp1 = spi::spi_pkt_mm::get().allocate();
            gp1->get_data().push_back(0x18);
            ph = tlm::nw::RESPONSE;
            spi_t->nb_transport_bw(*gp1, ph, t);
            break;
        }
        case 0xf0: // S25FL128S reset
        case 0x66: // n25qxxx reset enable
        case 0x99: // n25qxxx reset memory
            break;
        case 0x06: // Write Enable (WREN)
            wren = true;
            break;
        case 0x04: // Write Disable (WRDI)
            wren = false;
            break;
        case 0x01: { //  Write Registers (WRR 01h), SR1, CR1
            auto sr1 = cmd.get();
            auto cr1 = cmd.get();
            break;
        }
        case 0x05: { // Read Status Register 1RDSR1
            auto gp0 = spi::spi_pkt_mm::get().allocate();
            gp0->get_data().push_back(wren ? 0x02 : 0x0);
            tlm::tlm_phase ph = tlm::nw::RESPONSE;
            sc_core::sc_time t;
            spi_t->nb_transport_bw(*gp0, ph, t);
            break;
        }
        default:
            SCCWARN(SCMOD) << "command not understood: 0x" << std::hex << e;
        }
    }
}

} /* namespace generic */
} // namespace vpvper
