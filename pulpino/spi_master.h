/*
 * Copyright (c) 2019 -2021 Chair of Electronic Design Automation, Technical University of Munich
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __VPVPER_PULPINO_SPIMASTER_H__
#define __VPVPER_PULPINO_SPIMASTER_H__

#include "scc/tlm_target_bfs.h"
#include "scc/tlm_target_bfs_register_base.h"

#include <queue>

#include "tlm_core/tlm_2/tlm_generic_payload/tlm_generic_payload.h"
#include "tlm_utils/passthrough_target_socket.h"

namespace vpvper {
namespace pulpino {

class SPIMasterPayload;

/////////////////////////////////////////////////////////////////////////////
/// \class SPIMasterProtocolTypes
struct SPIMasterProtocolTypes {
    using tlm_payload_type = SPIMasterPayload;
    using tlm_phase_type = tlm::tlm_phase;
};

/////////////////////////////////////////////////////////////////////////////
/// \class SPIMasterPayload
struct SPIMasterPayload {
public:
    enum Command {
        SPI_IDLE,
        START_COMMAND,
        STOP_COMMAND,
        START_WRITE,
        START_READ,
        S_WRITE_COMMAND, // Standard Mode Write
        S_READ_COMMAND,  // Standard Mode Read
        Q_WRITE_COMMAND, // Quad Mode Write
        Q_READ_COMMAND,  // Quad Mode Read
        SPI_COMMAND,
        SPI_ADDRESS,
        DUMMY,
    };

    constexpr SPIMasterPayload()
    : command_{SPI_IDLE}
    , data_{0}
    , cs_{0} {}

    Command command_;
    uint32_t data_;
    uint8_t cs_;
};

/////////////////////////////////////////////////////////////////////////////
/// \class SPIMasterRegs
class SPIMasterRegs : public scc::tlm_target_bfs_register_base<SPIMasterRegs> {
public:
    std::array<scc::bitfield_register<uint32_t>, 9> registers{{
        {"STATUS", 0x00}, // Status Register
        {"CLKDIV", 0x04}, // Clock Divider
        {"SPICMD", 0x08}, // SPI Command
        {"SPIADR", 0x0C}, // SPI Adress
        {"SPILEN", 0x10}, // SPI Transfer Length
        {"SPIDUM", 0x14}, // SPI Dummy Cycles
        {"TXFIFO", 0x18}, // SPI Transmit FIFO
        {"RXFIFO", 0x20}, // SPI Receive FIFO
        {"INTCFG", 0x24}  // Inerrupt Configuration
    }};                   ///< SPIMaster registers
    std::array<scc::bitfield<uint32_t>, 19> bitfields{{
        // Bitfields not declared in Documentation -> used in the Treiber files of Pulpino
        {getRegister("STATUS"), "TX_STATUS", 24, 8, "spi.UNKNOWN_TXFIFO"}, // how much of the FIFO is in use right now
        {getRegister("STATUS"), "RX_STATUS", 16, 8, "spi.UNKNOWN_RXFIFO"},

        {getRegister("STATUS"), "CS", 8, 4, "spi.CS"},   // Chip Select (CS Signal used for the next transfer)
        {getRegister("STATUS"), "SR", 4, 1, "spi.SRST"}, // Software Reset (clear FIFO and abort active transfers)
        {getRegister("STATUS"), "QWR", 3, 1, "spi.QWR"}, // Quad Write command (write using Quad SPI Mode)
        {getRegister("STATUS"), "QRD", 2, 1, "spi.QRD"}, // Quad Read Command (read using Quad SPI Mode)
        {getRegister("STATUS"), "WR", 1, 1, "spi.WR"},   // Write Command (write standard SPI Mode)
        {getRegister("STATUS"), "RD", 0, 1, "spi.RD"},   // Read Command (read standard SPI Mode)

        {getRegister("CLKDIV"), "CLOCK_DIVIDER", 0, 8, "spi.CLKDIV"},
        {getRegister("SPICMD"), "SPI_CMD", 0, 32, "spi.SPICMD"}, // Sent first before performing read or write
        {getRegister("SPIADR"), "SPI_ADR", 0, 32, "spi.SPIADR"}, // Sent after SPI_CMD

        {getRegister("SPILEN"), "DATALEN", 16, 16, "spi.DATALEN"}, // SPI Data Length
        {getRegister("SPILEN"), "ADDRLEN", 8, 6, "spi.ADDRLEN"},   // SPI Adress Length
        {getRegister("SPILEN"), "CMDLEN", 0, 6, "spi.CMDLEN"},     // SPI Command Length

        {getRegister("SPIDUM"), "DUMMYWR", 16, 16, "spi.DUMMYWR"}, // Write Dummy Cycles
        {getRegister("SPIDUM"), "DUMMYRD", 0, 16, "spi.DUMMYRD"},  // Read Dummy Cycles

        {getRegister("TXFIFO"), "TX_FIFO", 0, 32, "spi.TX"}, // Transmit Data (Write Data into the FIFO)
        {getRegister("RXFIFO"), "RX_FIFO", 0, 32, "spi.RX"}, // Receive Data (Read Data from the FIFO)
        {getRegister("INTCFG"), "EN_INT", 31, 1, "spi.EN"}   // Enable Interrupts
    }};                                                      ///< SPIMaster register bitfields
    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    SPIMasterRegs(sc_core::sc_module_name name)
    : scc::tlm_target_bfs_register_base<SPIMasterRegs>{name} {};
};

/////////////////////////////////////////////////////////////////////////////
/// \class SPIMaster
/// \brief tlm_target_bfs based SPI-Master peripheral. Losely follows Pulp platform APB SPI Master:
/// https://github.com/pulp-platform/apb_spi_master
template <typename owner_t> class SPIMaster : public scc::tlm_target_bfs<SPIMasterRegs, owner_t> {
    typedef scc::tlm_target_bfs<SPIMasterRegs, owner_t> bfs_t;
    constexpr static const char* ID_SPIMASTER{"spi master"};
    SC_HAS_PROCESS(SPIMaster);
    /////////////////////////////////////////////////////////////////////////////
    /// \brief FSM states
    enum class States {
        IDLE,
        COMMAND,
        ADDRESS,
        DUMMY_TX,
        DUMMY_RX,
        DATA_TX,
        DATA_RX,
        WAIT_TX,
        WAIT_RX,
        FINISHED,
    };
    /////////////////////////////////////////////////////////////////////////////
    /// \brief message bit codes
    typedef enum Bitcodes {
        _STANDARD_ = 0x80000000,
        _QUAD_ = 0xF0000000,
        _CS0_ = 0x00000001,
        _CS1_ = 0x00000002,
        _CS2_ = 0x00000004,
        _CS3_ = 0x00000008
    } bitcodes_t;

public:
    std::unique_ptr<tlm_utils::simple_initiator_socket<SPIMaster, 32, SPIMasterProtocolTypes>> sock_i_{nullptr};
    ///< initiator socket (to ExternalSPIDevice) for transmit
    std::unique_ptr<tlm_utils::passthrough_target_socket<SPIMaster, 32, SPIMasterProtocolTypes>> sock_t_ext_{nullptr};
    ///< target socket (from ExternalSPIDevice) for receive

    /////////////////////////////////////////////////////////////////////////////
    /// \brief Constructor
    SPIMaster(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner = nullptr,
              uint64_t cpu_period_ps = 1000000 /*f~1MHz*/);

private:
    bool transferinProgress = false;
    bool quad = false;
    bool tx_mode = false;
    bool rx_mode = false;

    std::queue<uint32_t> fifo;
    ///< Queue
    uint8_t maxSizefifo_ = 2;
    ///< Set by SystemVerilog analyse
    uint8_t chipselect = 0;
    ///< ChipSelect
    uint16_t data_counter_ = 0;
    ///< Helper variable for the transaction
    uint32_t bit_send = 0;
    ///< Helper variable for the transaction
    uint32_t rx_help = 0;
    ///< Helper variable for temporary queuing
    uint32_t tx_status_help = 0;
    ///< Helper variable for temporary queuing

    sc_core::sc_time zero_time{sc_core::SC_ZERO_TIME};

    void reset(void);
    void sendInterrupt(void);
    void control_thread(void);
    void sendBit(uint32_t data_, uint8_t bit_offset, sc_core::sc_time& delay);
    void quad_sendBit(uint32_t data_, uint8_t bit_offset, sc_core::sc_time& delay);
    void state_change(void);

protected:
    uint64_t cpu_period_ps_;

    scc::bitfield<uint32_t>& tx_status_{bfs_t::regs->getBitfield("STATUS", "TX_STATUS", "spi.UNKNOWN_TXFIFO")};
    scc::bitfield<uint32_t>& rx_status_{bfs_t::regs->getBitfield("STATUS", "RX_STATUS", "spi.UNKNOWN_RXFIFO")};
    scc::bitfield<uint32_t>& cs_{bfs_t::regs->getBitfield("STATUS", "CS", "spi.CS")};
    scc::bitfield<uint32_t>& srst_{bfs_t::regs->getBitfield("STATUS", "SR", "spi.SRST")};
    scc::bitfield<uint32_t>& qwr_{bfs_t::regs->getBitfield("STATUS", "QWR", "spi.QWR")};
    scc::bitfield<uint32_t>& qrd_{bfs_t::regs->getBitfield("STATUS", "QRD", "spi.QRD")};
    scc::bitfield<uint32_t>& wr_{bfs_t::regs->getBitfield("STATUS", "WR", "spi.WR")};
    scc::bitfield<uint32_t>& rd_{bfs_t::regs->getBitfield("STATUS", "RD", "spi.RD")};

    scc::bitfield<uint32_t>& clkdiv_{bfs_t::regs->getBitfield("CLKDIV", "CLOCK_DIVIDER", "spi.CLKDIV")};
    scc::bitfield<uint32_t>& spicmd_{bfs_t::regs->getBitfield("SPICMD", "SPI_CMD", "spi.SPICMD")};
    scc::bitfield<uint32_t>& spiadr_{bfs_t::regs->getBitfield("SPIADR", "SPI_ADR", "spi.SPIADR")};

    scc::bitfield<uint32_t>& datalen_{bfs_t::regs->getBitfield("SPILEN", "DATALEN", "spi.DATALEN")};
    scc::bitfield<uint32_t>& addrlen_{bfs_t::regs->getBitfield("SPILEN", "ADDRLEN", "spi.ADDRLEN")};
    scc::bitfield<uint32_t>& cmdlen_{bfs_t::regs->getBitfield("SPILEN", "CMDLEN", "spi.CMDLEN")};

    scc::bitfield<uint32_t>& dummywr_{bfs_t::regs->getBitfield("SPIDUM", "DUMMYWR", "spi.DUMMYWR")};
    scc::bitfield<uint32_t>& dummyrd_{bfs_t::regs->getBitfield("SPIDUM", "DUMMYRD", "spi.DUMMYRD")};

    scc::bitfield<uint32_t>& tx_{bfs_t::regs->getBitfield("TXFIFO", "TX_FIFO", "spi.TX")};
    scc::bitfield<uint32_t>& rx_{bfs_t::regs->getBitfield("RXFIFO", "RX_FIFO", "spi.RX")};
    scc::bitfield<uint32_t>& en_{bfs_t::regs->getBitfield("INTCFG", "EN_INT", "spi.EN")};

    States state = States::IDLE;
};

/////////////////////////////////////////////////////////////////////////////
// SPIMaster<> methods definition:

template <class owner_t>
inline SPIMaster<owner_t>::SPIMaster(sc_core::sc_module_name name, scc::tlm_target_bfs_params&& params, owner_t* owner,
                                     uint64_t cpu_period_ps)
: bfs_t(name, std::move(params), owner)
, cpu_period_ps_(cpu_period_ps) {
    reset();

    srst_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(valueToWrite) {
            SC_REPORT_INFO(ID_SPIMASTER, "Software Reset");
            if(transferinProgress == true) {
                SC_REPORT_INFO(ID_SPIMASTER, "The Spi Periphreal was currently in progress");
                transferinProgress = false;
            }
            this->reset();
        }
    });

    cs_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if((valueToWrite & _CS0_) || (valueToWrite & _CS1_) || (valueToWrite & _CS2_) || (valueToWrite & _CS3_)) {
            chipselect = valueToWrite;
        } else {
            SC_REPORT_ERROR(ID_SPIMASTER, "No valid ChipSelect, Choose from 0b1, 0b10, 0b100 or 0b1000");
        }

        valueToWrite = 0;
    });

    qwr_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(transferinProgress && valueToWrite) {
            SC_REPORT_WARNING(ID_SPIMASTER, "Can not write on Bitfield qwr while in Progress, "
                                            "valueToWrite set to 0");
        }
        if(valueToWrite && !transferinProgress) {
            transferinProgress = true;
            tx_mode = true;
            quad = true;
        }
        valueToWrite = 0;
    });

    qrd_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(transferinProgress && valueToWrite) {
            SC_REPORT_WARNING(ID_SPIMASTER, "Can not write on Bitfield qrd while in Progress, "
                                            "valueToWrite set to 0");
        }
        if(valueToWrite && !transferinProgress) {
            transferinProgress = true;
            rx_mode = true;
            quad = true;
        }
        valueToWrite = 0;
    });

    wr_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(transferinProgress && valueToWrite) {
            SC_REPORT_WARNING(ID_SPIMASTER, "Can not write on Bitfield wr while in Progress, "
                                            "valueToWrite set to 0");
        }
        if(valueToWrite && !transferinProgress) {
            transferinProgress = true;
            tx_mode = true;
            quad = false;
        }
        valueToWrite = 0;
    });

    rd_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(transferinProgress && valueToWrite) {
            SC_REPORT_WARNING(ID_SPIMASTER, "Can not write on Bitfield rd while in Progress, "
                                            "valueToWrite set to 0");
        }
        if(valueToWrite && !transferinProgress) {
            transferinProgress = true;
            rx_mode = true;
            quad = false;
        }
        valueToWrite = 0;
    });

    clkdiv_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        if(transferinProgress) {
            valueToWrite = 0;
            SC_REPORT_WARNING(ID_SPIMASTER, "While SPI is in Progress the Clockdivider shall "
                                            "not be modified, valueToWrite set to 0");
        }
    });

    tx_.setWriteCallback([this](auto&&, uint32_t& valueToWrite) {
        // check if SPI is not in read Mode
        if(rx_mode) {
            valueToWrite = 0;
            SC_REPORT_ERROR(ID_SPIMASTER, "Can not Write new Data, because SPI is in read Mode. "
                                          "ValueToWrite set to 0");
        } else {
            // check for queue
            if(fifo.size() < maxSizefifo_) {
                fifo.push(valueToWrite);
                tx_status_ = tx_status_ + 4; // 4 Byte Data is being stored
                if(!transferinProgress) {
                    tx_status_help = tx_status_;
                }
            } else {
                valueToWrite = 0;
                SC_REPORT_WARNING(ID_SPIMASTER, "The Fifo is full, no new Data can be stored");
            }
        }
    });

    rx_.setReadCallback([this](auto&) {
        if(!fifo.empty()) {
            rx_ = fifo.front(); // writing new value to rx_ from queue
            fifo.pop();         // freeing space in queue

            rx_status_ = rx_status_ - 32; // One data less

            return static_cast<uint32_t>(rx_);
        } else {
            SC_REPORT_WARNING(ID_SPIMASTER, "Can not Read new Data from RX! FIFO is empty");
            return static_cast<uint32_t>(0);
        }
    });

    sock_i_ = std::make_unique<tlm_utils::simple_initiator_socket<SPIMaster, 32, SPIMasterProtocolTypes>>("initiator_socket");
    sock_t_ext_ = std::make_unique<tlm_utils::passthrough_target_socket<SPIMaster, 32, SPIMasterProtocolTypes>>("target_socket");

    SC_THREAD(control_thread);
}

template <class owner_t> void SPIMaster<owner_t>::reset() {
    // Bitfelder gleich Null setzen
    for(auto& bf : bfs_t::regs->bitfields) {
        bf = 0;
    }

    transferinProgress = false;
    quad = false;
    tx_mode = false;
    rx_mode = false;
    data_counter_ = 0;
    bit_send = 0;
    tx_status_help = 0;

    // clear Fifo
    while(!fifo.empty()) {
        fifo.pop();
    }

    state = States::IDLE;
}

template <class owner_t> void SPIMaster<owner_t>::state_change() {
    switch(state) {
    case States::IDLE:
        if(transferinProgress) {
            if(cmdlen_ != 0) {
                state = States::COMMAND;
            } else if(addrlen_ != 0) {
                state = States::ADDRESS;
            } else if(tx_mode) {
                if(dummywr_ != 0) {
                    state = States::DUMMY_TX;
                } else {
                    state = States::DATA_TX;
                }
            } else if(rx_mode) {
                if(dummyrd_ != 0) {
                    state = States::DUMMY_RX;
                } else {
                    state = States::DATA_RX;
                }
            }
        } else {
            state = States::IDLE;
        }
        break;

    case States::COMMAND:
        if(addrlen_ != 0) {
            state = States::ADDRESS;
        } else if(tx_mode) {
            if(dummywr_ != 0) {
                state = States::DUMMY_TX;
            } else {
                state = States::DATA_TX;
            }
        } else if(rx_mode) {
            if(dummyrd_ != 0) {
                state = States::DUMMY_RX;
            } else {
                state = States::DATA_RX;
            }
        } else {
            state = States::FINISHED;
        }
        break;

    case States::ADDRESS:
        if(tx_mode) {
            if(dummywr_ != 0) {
                state = States::DUMMY_TX;
            } else {
                state = States::DATA_TX;
            }
        } else if(rx_mode) {
            if(dummyrd_ != 0) {
                state = States::DUMMY_RX;
            } else {
                state = States::DATA_RX;
            }
        } else {
            state = States::FINISHED;
        }
        break;

    case States::DUMMY_TX:
        state = States::DATA_TX;
        break;

    case States::DUMMY_RX:
        state = States::DATA_RX;
        break;

    case States::DATA_TX:
        if(data_counter_ < datalen_) {
            if(!fifo.empty()) {
                state = States::DATA_TX;
            } else {
                if(data_counter_ < datalen_) {
                    state = States::WAIT_TX;
                }
            }
        } else {
            state = States::FINISHED;
        }
        break;

    case States::WAIT_TX:
        if(fifo.empty()) {
            state = States::WAIT_TX; // no new DATA to be transmitted
        } else {
            state = States::DATA_TX;
        }
        break;

    case States::DATA_RX:
        if(data_counter_ < datalen_) {
            if(fifo.size() < maxSizefifo_) {
                state = States::DATA_RX;
            } else {
                if(data_counter_ < datalen_) {
                    state = States::WAIT_RX;
                }
            }
        } else {
            state = States::FINISHED;
        }
        break;

    case States::WAIT_RX:
        if(fifo.size() >= maxSizefifo_) {
            state = States::WAIT_RX;
        } else if(fifo.size() << maxSizefifo_) {
            state = States::DATA_RX;
        }
        break;

    case States::FINISHED:
        if(!tx_status_ || !rx_status_) {
            transferinProgress = false;
            rx_mode = false;
            tx_mode = false;
            state = States::IDLE;
        }
        break;
    }
}

template <class owner_t> void SPIMaster<owner_t>::sendBit(uint32_t data_, uint8_t bit_offset, sc_core::sc_time& delay) {
    SPIMasterPayload payload{};
    payload.command_ = SPIMasterPayload::S_WRITE_COMMAND;
    bit_send = _STANDARD_ & (data_ << bit_offset);
    payload.data_ = bit_send;
    (*sock_i_)->b_transport(payload, delay);
}

template <class owner_t> void SPIMaster<owner_t>::quad_sendBit(uint32_t data_, uint8_t bit_offset, sc_core::sc_time& delay) {
    SPIMasterPayload payload{};
    payload.command_ = SPIMasterPayload::Q_WRITE_COMMAND;
    bit_send = _QUAD_ & (data_ << bit_offset);
    payload.data_ = bit_send;
    (*sock_i_)->b_transport(payload, delay);
}

template <class owner_t> void SPIMaster<owner_t>::control_thread() {
    auto base_clk_period = sc_core::sc_time(static_cast<double>(cpu_period_ps_), sc_core::SC_PS);

    sc_core::sc_time clk_period{sc_core::SC_ZERO_TIME};
    sc_core::sc_time zero_time{sc_core::SC_ZERO_TIME};

    while(true) {
        if(clkdiv_) {
            clk_period = base_clk_period * clkdiv_;
        } else {
            clk_period = base_clk_period;
        }

        wait(clk_period);

        switch(state) {
        case States::IDLE: {
            if(transferinProgress) {
                if(tx_mode) {
                    tx_status_ = tx_status_help;
                } // helper for tx_status -> gets put to 0 in callbacks
                SPIMasterPayload payload{};
                payload.command_ = SPIMasterPayload::START_COMMAND;
                payload.cs_ = chipselect;
                (*sock_i_)->b_transport(payload, zero_time);
            }
            break;
        }

        case States::COMMAND: {
            SPIMasterPayload payload{};
            payload.command_ = SPIMasterPayload::SPI_COMMAND;
            (*sock_i_)->b_transport(payload, zero_time);
            if(quad) { // Sending Command Quad Mode
                for(int i_q = 0; i_q < cmdlen_; i_q += 4) {
                    quad_sendBit(spicmd_, i_q, clk_period);
                    wait(clk_period);
                }
            } else { // Sending Command Standard Mode
                for(int i = 0; i < cmdlen_; i++) {
                    sendBit(spicmd_, i, clk_period);
                    wait(clk_period);
                }
            }
            break;
        }

        case States::ADDRESS: {
            SPIMasterPayload payload{};
            payload.command_ = SPIMasterPayload::SPI_ADDRESS;
            (*sock_i_)->b_transport(payload, zero_time);
            if(quad) { // Sending Address Quad Mode
                for(int j_q = (addrlen_ - 1); j_q >= 0; j_q -= 4) {
                    quad_sendBit(spiadr_, (31 - j_q), clk_period);
                    wait(clk_period);
                }
            } else { // Sending Address Standard Mode
                for(int j = (addrlen_ - 1); j >= 0; j--) {
                    sendBit(spiadr_, (31 - j), clk_period);
                    wait(clk_period);
                }
            }
            break;
        }

        case States::DUMMY_TX: {
            uint32_t zero = 0;
            SPIMasterPayload payload{};
            payload.command_ = SPIMasterPayload::DUMMY;
            (*sock_i_)->b_transport(payload, zero_time);
            if(quad) { // Quad Mode
                for(int dqt = 0; dqt < dummywr_; dqt++) {
                    quad_sendBit(0, 0, clk_period);
                    wait(clk_period);
                }
            } else { // Standard Mode
                for(int dt = 0; dt < dummywr_; dt++) {
                    sendBit(0, 0, clk_period);
                    wait(clk_period);
                }
            }
            break;
        }

        case States::DUMMY_RX: {
            uint32_t zero = 0;
            SPIMasterPayload payload{};
            payload.command_ = SPIMasterPayload::DUMMY;
            (*sock_i_)->b_transport(payload, zero_time);
            if(quad) {
                for(int dqr = 0; dqr < dummyrd_; dqr++) {
                    quad_sendBit(0, 0, clk_period);
                    wait(clk_period);
                }
            } else { // rd
                for(int dr = 0; dr < dummyrd_; dr++) {
                    sendBit(0, 0, clk_period);
                    wait(clk_period);
                }
            }
            break;
        }

        case States::DATA_TX: {
            SPIMasterPayload payload{};
            if(quad) { // qwr
                payload.command_ = SPIMasterPayload::START_WRITE;
                (*sock_i_)->b_transport(payload, zero_time);
                if(!fifo.empty()) {
                    for(int l = 0; l < 31; l += 4) { // send 4 Byte Data
                        quad_sendBit(fifo.front(), l, clk_period);
                        data_counter_ += 4;
                        wait(clk_period);
                    }
                    fifo.pop();                  // Deletes the data that has been send from the queue
                    tx_status_ = tx_status_ - 4; // one place in the fifo has been freed
                }
            } else { // wr
                payload.command_ = SPIMasterPayload::START_WRITE;
                (*sock_i_)->b_transport(payload, zero_time);
                if(!fifo.empty()) {               // proof for reg_done
                    for(int k = 0; k < 32; k++) { // sendet ein Datenwort mit der Länge 32 Bit
                        sendBit(fifo.front(), k, clk_period);
                        data_counter_++;
                        wait(clk_period);
                    }
                    fifo.pop();                  // Deletes the data that has been send from the queue
                    tx_status_ = tx_status_ - 4; // 0x7FF is the maximal amount of databits that can be send
                }
            }
            break;
        }

        case States::DATA_RX: {
            SPIMasterPayload payload{};
            if(quad) { // qrd
                payload.command_ = SPIMasterPayload::START_READ;
                (*sock_i_)->b_transport(payload, zero_time);
                for(int p = 7; p >= 0; p--) {
                    payload.command_ = SPIMasterPayload::Q_READ_COMMAND;
                    (*sock_i_)->b_transport(payload, clk_period);
                    rx_help = rx_help | (payload.data_ << (p * 4));
                    data_counter_ += 4;
                    wait(clk_period);
                }
                fifo.push(rx_help);
                rx_help = 0;
                rx_status_ = rx_status_ + 32;
            } else { // rd
                payload.command_ = SPIMasterPayload::START_READ;
                (*sock_i_)->b_transport(payload, zero_time);
                for(int o = 31; o >= 0; o--) {
                    payload.command_ = SPIMasterPayload::S_READ_COMMAND;
                    (*sock_i_)->b_transport(payload, clk_period);
                    rx_help = rx_help | (payload.data_ << o);
                    data_counter_++;
                    wait(clk_period);
                }
                fifo.push(rx_help);
                rx_help = 0;
                rx_status_ = rx_status_ + 32;
            }
            break;
        }

        case States::FINISHED: {
            SPIMasterPayload payload{};
            payload.command_ = SPIMasterPayload::STOP_COMMAND;
            (*sock_i_)->b_transport(payload, clk_period);
            quad = false;
            data_counter_ = 0;
            rd_ = static_cast<uint32_t>(1); // set finished bit
            break;
        }
        }

        state_change();
    } // end while
} // end function

template <class owner_t> void SPIMaster<owner_t>::sendInterrupt(void) {
    if(en_) {
        (*bfs_t::irq_out_)[0] = 1;
        (*bfs_t::irq_out_)[1] = 1;
    }
}

} // namespace pulpino
} // namespace vpvper

#endif // __VPVPER_PULPINO_SPIMASTER_H__
