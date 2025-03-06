/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_EXTERNAL_UART_DEVICE_H__
#define __VPVPER_PULPINO_EXTERNAL_UART_DEVICE_H__

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#include <memory>

#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

namespace vpvper {
namespace pulpino {

////////////////////////////////////////////////////////////////////////////////
///\class ExternalUARTDevice
///\brief external UART device connected to Uart peripheral. Simple std output, no input
class ExternalUARTDevice : public sc_core::sc_module {
    SC_HAS_PROCESS(ExternalUARTDevice);

protected:
    constexpr static const char* ID_UARTDEV{"external uart device"};

    sc_core::sc_time scan_period_;
    ///< scan period for sense_input thread
    std::vector<char> inbuf_;
    ///< virtual input fifo is filled through periodic sense_input, no characters are lost

public:
    tlm_utils::simple_target_socket<ExternalUARTDevice> sock_t_{"target_socket"};
    ///< receiver socket (from UART peripheral writes)
    tlm_utils::simple_initiator_socket<ExternalUARTDevice> sock_i_{"initiator_socket"};
    ///< transmitter socket (to UART peripheral writes)

    ExternalUARTDevice(sc_core::sc_module_name, sc_core::sc_time scan_period);
    virtual void b_transport(tlm::tlm_generic_payload&, sc_core::sc_time&);
    virtual ~ExternalUARTDevice(void) {}

protected:
    virtual void sense_input(void){};
    virtual int read(unsigned char* buf);
    virtual int write(unsigned char* buf);
};

////////////////////////////////////////////////////////////////////////////////
///\class ExternalUARTDeviceFileStream
///\brief (named) pipe based output and input. The input pipe is periodically read and buffered
class ExternalUARTDeviceFileStream final : public ExternalUARTDevice {
    typedef boost::iostreams::back_insert_device<std::vector<char>> vec_sink_t;

    const std::string out_fpath_;
    ///< path to the output file
    const std::string in_fpath_;
    ///< path to the input file
    const bool is_named_pipe_;
    ///< will use named pipelines instead of normal files
    vec_sink_t vec_in_sink_{ExternalUARTDevice::inbuf_};
    boost::iostreams::stream<vec_sink_t> vec_in_stream_{vec_in_sink_};

protected:
    std::shared_ptr<boost::iostreams::stream<boost::iostreams::file_descriptor_sink>> h_stream_out_{nullptr};
    ///< stream handle for output
    std::shared_ptr<boost::iostreams::stream_buffer<boost::iostreams::file_descriptor_source>> h_stream_in_{nullptr};
    ///< stream handle for input
public:
    ExternalUARTDeviceFileStream(sc_core::sc_module_name, sc_core::sc_time scan_period, const std::string& out_fpath,
                                 const std::string& in_fpath, bool is_named_pipe = false);
    virtual ~ExternalUARTDeviceFileStream(void);

protected:
    virtual void sense_input(void) override;
    virtual int read(unsigned char* buf) override;
    virtual int write(unsigned char* buf) override;
};

} // namespace pulpino
} // namespace vpvper

#endif // __VPVPER_PULPINO_EXTERNAL_UART_DEVICE_H__
