/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "external_uart_device.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>

#include <sstream>

namespace vpvper {
namespace pulp {

////////////////////////////////////////////////////////////////////////////////
/// base (=stdio)
ExternalUARTDevice::ExternalUARTDevice(sc_core::sc_module_name name, sc_core::sc_time scan_period)
  : sc_core::sc_module{name} 
  , scan_period_(scan_period)
{
  sock_t_.register_b_transport(this, &ExternalUARTDevice::b_transport);

  SC_THREAD(sense_input);
}

void ExternalUARTDevice::b_transport(tlm::tlm_generic_payload& gp, sc_core::sc_time& delay) {
  unsigned char* ptr = gp.get_data_ptr();

  if(gp.get_command() == tlm::TLM_WRITE_COMMAND) {
    if ( write (ptr) != 0 ) {
      gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    } else {
      gp.set_response_status(tlm::TLM_OK_RESPONSE);
    }
  } else {
    if ( read (ptr) != 0 )  {
      gp.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    } else {
      gp.set_response_status(tlm::TLM_OK_RESPONSE);
    }
  }
}

int ExternalUARTDevice::read(unsigned char* ptr) {
  return (-1); // not supported in simple UART device
}

int ExternalUARTDevice::write(unsigned char* ptr) {
  std::cout << ptr[0];
  return (0);
}

////////////////////////////////////////////////////////////////////////////////
/// named pipe based
ExternalUARTDeviceFileStream::ExternalUARTDeviceFileStream(
  sc_core::sc_module_name name, 
  sc_core::sc_time scan_period, 
  const std::string& out_fpath, 
  const std::string& in_fpath,
  bool is_named_pipe)
  : ExternalUARTDevice(name, scan_period)
  , out_fpath_(out_fpath)
  , in_fpath_(in_fpath)
  , is_named_pipe_(is_named_pipe)
{
  namespace io = boost::iostreams;
  using src = boost::iostreams::file_descriptor_source;
  using sink = boost::iostreams::file_descriptor_sink;
  int fd_in, fd_out;
  
  if (is_named_pipe_) {
    struct stat attribut;
    // open output named pipe, if pipe is already created this is not an error - use it
    if ((mkfifo (out_fpath_.c_str(),  S_IRUSR | S_IWUSR)) == -1) {
      if(errno != EEXIST){
        std::stringstream x;
        x << "failed to create output named pipeline \"" << out_fpath_ << "\"";
        SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
      }
    }
    if (stat (out_fpath_.c_str(), &attribut) == -1) {
      std::stringstream x;
      x << "failed to create output named pipeline \"" << out_fpath_ << "\"";
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    }
    fd_out = open (out_fpath_.c_str(), O_RDWR);
    if(fd_out < 0) {
      std::stringstream x;
      x << "failed to open output named pipeline \"" << out_fpath_ << "\": E" << errno;
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    }
    // open input named pipe, if pipe is already created this is not an error - use it
    if ((mkfifo (in_fpath_.c_str(),  S_IRUSR | S_IWUSR)) == -1) {
      if(errno != EEXIST){
        std::stringstream x;
        x << "failed to open input named pipeline \"" << in_fpath_ << "\"";
        SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
      }
    }
    if (stat (in_fpath_.c_str(), &attribut) == -1) {
      std::stringstream x;
      x << "failed to open input named pipeline \"" << in_fpath_ << "\"";
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    }
    fd_in = open (in_fpath_.c_str(), O_RDONLY | O_NONBLOCK);
    if(fd_in < 0) {
      std::stringstream x;
      x << "failed to open input named pipeline \"" << out_fpath_ << "\": E" << errno;
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    }
  } else {
    fd_out = open (out_fpath_.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if(fd_out < 0) {
      std::stringstream x;
      x << "failed to open output file \"" << out_fpath_ << "\": E" << errno;
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    }
    fd_in = open (in_fpath_.c_str(), O_RDONLY | O_NONBLOCK);
    if(fd_in < 0) {
      std::stringstream x;
      x << "failed to open input file \"" << in_fpath_ << "\": E" << errno;
      SC_REPORT_WARNING(ID_UARTDEV, x.str().c_str());
    }

  }

  // connect a stream handle to the "output file": everything is a file.
  h_stream_out_ = std::make_shared< io::stream<sink> > (
    sink(fd_out, io::file_descriptor_flags::close_handle)
  );
  // connect a stream handle to the "input file"
  h_stream_in_ = std::make_shared< io::stream<src> > (
    src(fd_in, io::file_descriptor_flags::close_handle)
  );
}

int ExternalUARTDeviceFileStream::read(unsigned char* ptr) {
  static unsigned char last = ' ';
  if(!inbuf_.empty()){
    *ptr = inbuf_[0];
    inbuf_.erase(inbuf_.begin());
    last = *ptr;
  }else{
    last = *ptr;
  }
  return 0;
}

int ExternalUARTDeviceFileStream::write(unsigned char* ptr) {
  if(h_stream_out_->is_open()) {
    h_stream_out_->put(ptr[0]);
    h_stream_out_->flush();
  }
  return 0;
}

void ExternalUARTDeviceFileStream::sense_input(void) {
  struct pollfd fds{ .fd = (*h_stream_in_)->handle(), .events = POLLIN };
  while(1) {
    sc_core::wait(scan_period_);
    int res = poll(&fds, 1, 0);
    if(res < 0||fds.revents&(POLLERR|POLLNVAL)) {
      std::stringstream x;
      x << "failed to poll named pipe \"" << in_fpath_ << "\"";
      SC_REPORT_FATAL(ID_UARTDEV, x.str().c_str());
    } else {
      char p;
      h_stream_in_->get(p);
      inbuf_.push_back(p);
    }
  }
}

ExternalUARTDeviceFileStream::~ExternalUARTDeviceFileStream(void) {
  if(h_stream_out_->is_open() ){
    h_stream_out_->close();
  }
  if(h_stream_in_->is_open() ){
    h_stream_in_->close();
  }
}

} // namespace pulp
} // namespace vpvper
