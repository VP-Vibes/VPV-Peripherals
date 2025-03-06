#include "dma.h"
#include <scc/report.h>
#include <scc/utilities.h>

using namespace std;
using namespace sc_core;

namespace vpvper {
namespace minres {

SC_HAS_PROCESS(dma);

dma::dma(sc_core::sc_module_name nm)
: sc_core::sc_module(nm)
, scc::tlm_target<>(clk_period)
, initiator_socket("initiator_socket")
, NAMEDD(regs, dma_regs)

{
    regs->control.set_write_cb([this](scc::sc_register<uint32_t>& reg, uint32_t data) -> bool {
        bool prev_ch0 = regs->r_control.ch0_enable_transfer;
        bool prev_ch1 = regs->r_control.ch1_enable_transfer;

        reg.put(data); // Write new value to register

        bool new_ch0 = regs->r_control.ch0_enable_transfer;
        bool new_ch1 = regs->r_control.ch1_enable_transfer;

        if(prev_ch0 == 0 && new_ch0 == 1) {
            regs->r_status.ch0_busy = 1;
            ch0_enable_event.notify(SC_ZERO_TIME);   // Trigger event immediately
            regs->r_control.ch0_enable_transfer = 0; // Reset the flag to 0
        }

        if(prev_ch1 == 0 && new_ch1 == 1) {
            regs->r_status.ch1_busy = 1;
            ch1_enable_event.notify(SC_ZERO_TIME);
            regs->r_control.ch1_enable_transfer = 0;
        }

        return true;
    });

    regs->registerResources(*this);

    SC_METHOD(reset_cb);
    sensitive << rst_i;
    dont_initialize();

    SC_THREAD(transfer_process_ch0);
    SC_THREAD(transfer_process_ch1);

    // Update IRQ when interrupt pending registers change
    regs->ip.set_write_cb([this](scc::sc_register<uint32_t>&, uint32_t, sc_time) -> bool {
        update_irq();
        return true;
    });
}

dma::~dma() = default;

void dma::reset_cb() {
    if(rst_i.read()) {
        regs->reset_start();
        irq_o.write(false);
    } else {
        regs->reset_stop();
    }
}

void dma::transfer_process_ch0() {
    while(true) {
        wait(ch0_enable_event);

        {
            uint32_t src_addr = regs->r_ch0_src_start_addr;
            uint32_t dst_addr = regs->r_ch0_dst_start_addr;
            uint32_t seg_length = regs->r_ch0_transfer.seg_length;
            uint32_t seg_count = regs->r_ch0_transfer.seg_count;
            uint32_t width = regs->r_ch0_transfer.width;

            // Use a buffer for segment transfer
            std::vector<uint8_t> buffer(seg_length * (1 << width));

            for(uint32_t seg = 0; seg < seg_count; seg++) {
                tlm::tlm_generic_payload trans;
                sc_time delay = SC_ZERO_TIME;

                // Read entire segment in one TLM transaction
                trans.set_command(tlm::TLM_READ_COMMAND);
                trans.set_address(src_addr);
                trans.set_data_ptr(buffer.data());
                trans.set_data_length(buffer.size());
                trans.set_streaming_width(buffer.size()); // Set full streaming width
                trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                initiator_socket->b_transport(trans, delay);

                if(trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
                    SCCFATAL("DMA") << "TLM transaction failed at READ ch0";
                }

                // Write entire segment in one TLM transaction
                trans.set_command(tlm::TLM_WRITE_COMMAND);
                trans.set_address(dst_addr);
                trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                initiator_socket->b_transport(trans, delay);

                if(trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
                    SCCFATAL("DMA") << "TLM transaction failed at WRITE ch0";
                }

                // Update addresses using stride
                src_addr += regs->r_ch0_src_addr_inc.src_stride;
                dst_addr += regs->r_ch0_dst_addr_inc.dst_stride;

                // Trigger segment done interrupt
                if(regs->r_ie.ch0_ie_seg_transfer_done) {
                    regs->r_ip.ch0_ip_seg_transfer_done = 1;
                    update_irq();
                }
            }

            regs->r_status.ch0_busy = 0;
            if(regs->r_ie.ch0_ie_transfer_done) {
                regs->r_ip.ch0_ip_transfer_done = 1;
                update_irq();
            }
        }
    }
}

void dma::transfer_process_ch1() {
    while(true) {
        wait(ch1_enable_event);

        {
            uint32_t src_addr = regs->r_ch1_src_start_addr;
            uint32_t dst_addr = regs->r_ch1_dst_start_addr;
            uint32_t seg_length = regs->r_ch1_transfer.seg_length;
            uint32_t seg_count = regs->r_ch1_transfer.seg_count;
            uint32_t width = regs->r_ch1_transfer.width;

            std::vector<uint8_t> buffer(seg_length * (1 << width));

            for(uint32_t seg = 0; seg < seg_count; seg++) {
                tlm::tlm_generic_payload trans;
                sc_time delay = SC_ZERO_TIME;

                trans.set_command(tlm::TLM_READ_COMMAND);
                trans.set_address(src_addr);
                trans.set_data_ptr(buffer.data());
                trans.set_data_length(buffer.size());
                trans.set_streaming_width(buffer.size()); // Set full streaming width
                trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                initiator_socket->b_transport(trans, delay);

                if(trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
                    SCCFATAL("DMA") << "TLM transaction failed at READ ch1";
                }

                trans.set_command(tlm::TLM_WRITE_COMMAND);
                trans.set_address(dst_addr);
                trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

                initiator_socket->b_transport(trans, delay);

                if(trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
                    SCCFATAL("DMA") << "TLM transaction failed at WRITE ch1";
                }

                src_addr += regs->r_ch1_src_addr_inc.src_stride;
                dst_addr += regs->r_ch1_dst_addr_inc.dst_stride;

                if(regs->r_ie.ch1_ie_seg_transfer_done) {
                    regs->r_ip.ch1_ip_seg_transfer_done = 1;
                    update_irq();
                }
            }

            regs->r_status.ch1_busy = 0;
            if(regs->r_ie.ch1_ie_transfer_done) {
                regs->r_ip.ch1_ip_transfer_done = 1;
                update_irq();
            }
        }
    }
}

void dma::update_irq() {
    bool irq = (regs->r_ip.ch0_ip_seg_transfer_done && regs->r_ie.ch0_ie_seg_transfer_done) ||
               (regs->r_ip.ch0_ip_transfer_done && regs->r_ie.ch0_ie_transfer_done) ||
               (regs->r_ip.ch1_ip_seg_transfer_done && regs->r_ie.ch1_ie_seg_transfer_done) ||
               (regs->r_ip.ch1_ip_transfer_done && regs->r_ie.ch1_ie_transfer_done);
    irq_o.write(irq);
}

} // namespace minres
} // namespace vpvper
