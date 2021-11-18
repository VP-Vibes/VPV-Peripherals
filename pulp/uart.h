/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULP_UART_H__
#define __VPVPER_PULP_UART_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

#include <queue>

namespace vpvper {
namespace pulp {

/////////////////////////////////////////////////////////////////////////////
/// \class UARTRegs
class UARTRegs : public scc::tlm_target_bfs_register_base<UARTRegs> {
public:
  std::array<scc::bitfield_register<uint32_t>, 8> registers{{
    {"RBR_THR_DLL", 0x0000}, 
      ///< Receiver Buffer Register (RBR if DLAB=0 && read), Transmitter Holding Register (THR if DLAB=0 && write), Divisor Latch (LS) (if DLAB=1)
    {"IER_DLM",  0x0004},
      ///< Interrupt Enable Register (IER if DLAB=0), Divisor Latch (MS) (if DLAB=1)
    {"IIR_FCR",  0x0008},
      ///< Interrupt Identification Register (IIR if read), FIFO Control Register (FCR if write)
    {"LCR",  0x000C},
      ///< Line Control Register (LCR)
    {"MCR",  0x0010},
      ///< Modem Control Register (MCR)
    {"LSR",  0x0014},
      ///< Line Status Register (LSR)
    {"MSR",  0x0018},
      ///< Modem Status Register (MSR)
    {"SCR",  0x001C}
      ///< Scratch Register (SCR)
  }}; ///< UART registers
  std::array<scc::bitfield<uint32_t>, 12> bitfields{{
    {getRegister("RBR_THR_DLL"),"RBR_THR_DLL", 0, 8,"uart.RBR_THR_DLL"},
    {getRegister("IER_DLM"),"IER_DLM", 0, 8,"uart.IER_DLM"},
    {getRegister("IIR_FCR"),"IIR_FCR", 0, 8,"uart.IIR_FCR"},
    {getRegister("LCR"),"LCR.data", 0, 2,"uart.LCR.data"},
    {getRegister("LCR"),"LCR.stop", 2, 1,"uart.LCR.stop"},
    {getRegister("LCR"),"LCR.parity", 3, 3,"uart.LCR.parity"},
    {getRegister("LCR"),"LCR.break", 6, 1,"uart.LCR.break"},
    {getRegister("LCR"),"LCR.dlab", 7, 1,"uart.LCR.dlab"},
    {getRegister("MCR"),"MCR", 0, 8,"uart.MCR"},
    {getRegister("LSR"),"LSR", 0, 8,"uart.LSR"},
    {getRegister("MSR"),"MSR", 0, 8,"uart.MSR"},
    {getRegister("SCR"),"SCR", 0, 8,"uart.SCR"},
  }}; ///< UART register bitfields
  /////////////////////////////////////////////////////////////////////////////
  /// \brief Constructor
  UARTRegs(sc_core::sc_module_name name) : tlm_target_bfs_register_base<UARTRegs>{name} {};
};

/////////////////////////////////////////////////////////////////////////////
/// \class UART
/// \brief tlm_target_bfs based UART peripheral. Losely follows 16650 standard
template <typename owner_t>
class UART : public scc::tlm_target_bfs<UARTRegs, owner_t> {
  typedef scc::tlm_target_bfs<UARTRegs, owner_t> bfs_t;
  constexpr static const char* ID_UART{"uart"};
  SC_HAS_PROCESS(UART);
  /////////////////////////////////////////////////////////////////////////////
  /// \brief interrupt events for send interrupt process
  typedef enum INTERRUPT_EVENTS{
    S_RDATA_AVAILABLE = 1,
    S_TBR_EMPTY = 2,
    S_RLSR_CHANGE = 4,
    R_RDATA_AVAILABLE = 0x10,
    R_TBR_EMPTY = 0x20,
    R_RLSR_CHANGE = 0x40,
  } interrupt_events_t;

public:
  std::shared_ptr<tlm_utils::simple_initiator_socket<UART>> sock_i_{nullptr};
    ///< initiator socket (to ExternalUARTDevice) for transmit 
  std::shared_ptr<tlm_utils::simple_target_socket<UART>> sock_t_ext_{nullptr};
    ///< target socket (from ExternalUARTDevice) for receive

  /////////////////////////////////////////////////////////////////////////////
  /// \brief Constructor
  UART(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner = nullptr);
  /////////////////////////////////////////////////////////////////////////////
  /// \brief Destructor
  virtual ~UART(void) {}
  /////////////////////////////////////////////////////////////////////////////
  /// \brief tlm transport to receive new data from external device
  void b_transport_ext(tlm::tlm_generic_payload&, sc_core::sc_time&);

protected:
  // shadow registers: multi-use registers/bitfields (same address, function depends on internal states and R/W)
  scc::bitfield<uint32_t>& rbr_thr_dll_{bfs_t::regs->getBitfield("RBR_THR_DLL", "RBR_THR_DLL", "uart.RBR_THR_DLL")};
  scc::bitfield<uint32_t>& ier_dlm_{bfs_t::regs->getBitfield("IER_DLM", "IER_DLM", "uart.IER_DLM")};
  scc::bitfield<uint32_t>& iir_fcr_{bfs_t::regs->getBitfield("IIR_FCR", "IIR_FCR", "uart.IIR_FCR")};
  
  // normal registers/bitfields
  scc::bitfield<uint32_t>& dlab_{bfs_t::regs->getBitfield("LCR", "LCR.dlab", "uart.LCR.dlab")};
  scc::bitfield<uint32_t>& mcr_{bfs_t::regs->getBitfield("MCR", "MCR", "uart.MCR")};
  scc::bitfield<uint32_t>& lsr_{bfs_t::regs->getBitfield("LSR", "LSR", "uart.LSR")};
  scc::bitfield<uint32_t>& msr_{bfs_t::regs->getBitfield("MSR", "MSR", "uart.MSR")};
  scc::bitfield<uint32_t>& scr_{bfs_t::regs->getBitfield("SCR", "SCR", "uart.SCR")};
  
  // actual register states for multi-use (addressed) registers
  uint8_t rbr_{0}; uint8_t thr_{0}; uint8_t dll_{0};
  uint8_t ier_{0}; uint8_t dlm_{0};
  uint8_t iir_{0}; uint8_t fcr_{0};
  
private:  
  sc_core::sc_event interrupt_event_;
  std::queue<interrupt_events_t> interrupt_event_q_;

  bool send_interrupt_{false};

  void reset(void);
  void sendData(uint8_t);
  void getData(uint8_t&);
  void send_interrupt_process(void);
};

/////////////////////////////////////////////////////////////////////////////
// UART<> methods definition:

template <class owner_t>
inline UART<owner_t>::UART(
  sc_core::sc_module_name name,
  scc::tlm_target_bfs_params&& params,
  owner_t* owner)
  : bfs_t(name, std::move(params), owner)
{
  reset();
  
  sock_i_ = std::make_shared<tlm_utils::simple_initiator_socket<UART>>("initiator_socket");
  sock_t_ext_ = std::make_shared<tlm_utils::simple_target_socket<UART>>("target_socket_external");
  sock_t_ext_->register_b_transport(this, &UART::b_transport_ext);

  rbr_thr_dll_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
    if(dlab_ == 1) {
      dll_ = valueToWrite;
    } else {
      thr_ = valueToWrite;
      sendData(thr_);
      interrupt_event_q_.push(R_TBR_EMPTY);
      interrupt_event_.notify();
    }
  });
  
  rbr_thr_dll_.setReadCallback([this](auto&&) {
    if(dlab_ == 1) {
      return(dll_);
    } else {
      uint8_t ptr;
      getData(ptr);
      interrupt_event_q_.push(R_RDATA_AVAILABLE);
      interrupt_event_.notify();
      return(ptr);
    }
  });
  
  ier_dlm_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
    if(dlab_ == 1) {
      dlm_ = valueToWrite;
    } else {
      ier_ = valueToWrite;
    }
  });
  
  ier_dlm_.setReadCallback([this](auto&&) {
    if(dlab_ == 1) {
      return(dlm_);
    } else {
      return(ier_);
    }
  });
  
  iir_fcr_.setReadCallback([this](auto&&) {
    uint8_t iir = iir_;
    interrupt_event_q_.push(R_TBR_EMPTY);
    interrupt_event_.notify();
    return(iir);
  });
  
  lsr_.setReadCallback([this](auto&&) {
    return(lsr_.get()|0x60); // vp output has no "limit"
  });
  
  SC_THREAD(send_interrupt_process);
}

template <class owner_t>
void UART<owner_t>::b_transport_ext(tlm::tlm_generic_payload& gp,
                           sc_core::sc_time& delay) {
  gp.set_response_status(tlm::TLM_OK_RESPONSE);
  auto data = *reinterpret_cast<uint32_t*>(gp.get_data_ptr());
  // regs_.RBR = data;
  uint8_t in_LSR = lsr_; //regs_.LSR;

  //regs_.LSR = (regs_.LSR & ~(0x01)) | (data & 0x1);
  lsr_ = (lsr_ & ~(0x01)) | (data & 0x1);
  if(lsr_ != in_LSR){
    if(ier_ & 0x4) {
      interrupt_event_q_.push(S_RLSR_CHANGE);
      interrupt_event_.notify();
    }
  }
  if( (ier_ & 0x1) && (lsr_ & 0x1)){
    interrupt_event_q_.push(S_RDATA_AVAILABLE);
    interrupt_event_.notify();
  }
}

template <class owner_t>
void UART<owner_t>::reset() {
  for (auto& bf : bfs_t::regs->bitfields) {
    bf = 0;
  }
}

template <class owner_t>
void UART<owner_t>::sendData(uint8_t data) {
  tlm::tlm_generic_payload gp{};
  gp.set_command(tlm::TLM_WRITE_COMMAND);
  gp.set_data_ptr(static_cast<unsigned char*>(&data));
  gp.set_data_length(1);
  gp.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
  sc_core::sc_time delay{sc_core::SC_ZERO_TIME};

  (*sock_i_)->b_transport(gp, delay);

  if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
    SC_REPORT_FATAL(ID_UART, "data send failed in UART::sendData()");
  }

  if(ier_ & 0x2) {
    interrupt_event_q_.push(S_TBR_EMPTY);
    interrupt_event_.notify();
  }
}

template <class owner_t>
void UART<owner_t>::getData(uint8_t& data) {

  tlm::tlm_generic_payload gp{};
  sc_core::sc_time delay{sc_core::SC_ZERO_TIME};
  gp.set_command(tlm::TLM_READ_COMMAND);
  gp.set_data_ptr(static_cast<unsigned char*>(&data));
  gp.set_data_length(1);
  gp.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

  (*sock_i_)->b_transport(gp, delay);

  if (gp.get_response_status() != tlm::TLM_OK_RESPONSE) {
    SC_REPORT_FATAL(ID_UART, "data receive failed in UART::getData()");
  }

  data = *reinterpret_cast<uint8_t*>(gp.get_data_ptr());
  rbr_ = data;

}

template <class owner_t>
void UART<owner_t>::send_interrupt_process(void) {
  (*bfs_t::irq_out_)[0] = 0;
  while (1) {
    wait(interrupt_event_);
    int x = 0;
    while(!interrupt_event_q_.empty()) {
      x |=  int(interrupt_event_q_.front()); // get all events
      interrupt_event_q_.pop();
    }
    if(x & 0xF ) { // set interrupt
      if(x & S_RLSR_CHANGE){
        iir_ = 0x3 << (1);
      } else if(x & S_RDATA_AVAILABLE){
        iir_ |= 0x1 << (2);
      } else if(x & S_TBR_EMPTY){
        iir_ |= 0x1 << (1);
      }
    }
    else { // reset interrupt
      if(x & R_RLSR_CHANGE)
        iir_ &= ~(0x3 << (1));
      if(x & R_RDATA_AVAILABLE)
        iir_ &= ~(0x1 << (2));
      if(x & R_TBR_EMPTY)
        iir_ &= ~(0x1 << (1));
    }

    if(iir_ & (~0x1)) {
      iir_ &= (~0x1); // IP
      (*bfs_t::irq_out_)[0] = 1;
    }else {
      (*bfs_t::irq_out_)[0] = 0;
      iir_ |= 1; // no IP
    }

  }
}

} // namespace pulp
} // namespace vpvper

#endif // __VPVPER_PULP_UART_H__
