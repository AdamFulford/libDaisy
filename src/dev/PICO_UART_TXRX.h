#pragma once
#ifndef PICO_UART_H
#define PICO_UART_H

//class for Uart configuration, handling encoding, parsing, 
//tx and rx (dma listen) with RP2040, and debouncing received button inputs

#include <array>
#include "daisy.h"
#include "per/uart.h"
#include "util/cobs.h"

#define START_BYTE 0x7e
#define END_BYTE 0x81
#define NUM_BOOLS 6
#define PAYLOAD_LENGTH 2
#define BUFF_SIZE (5 + PAYLOAD_LENGTH) //start byte + payload + CRC + end byte

using namespace daisy;

class PicoUartTXRX
{
    public:
    PicoUartTXRX() {}
    ~PicoUartTXRX() {}

enum ParserState
{
    WAIT_FOR_START_BYTE,
    WAIT_FOR_OH_BYTE,
    WAIT_FOR_LENGTH,
    WAIT_FOR_PAYLOAD,
    WAIT_FOR_CRC,
    WAIT_FOR_END_BYTE
};

enum SwitchType
{
        TYPE_TOGGLE,    
        TYPE_MOMENTARY,
};


struct Config
{
    UartHandler::Config::Peripheral periph;
    dsy_gpio_pin                    rx;
    dsy_gpio_pin                    tx;
    uint32_t                        baudrate;

};

daisy::UartHandler::Result Init(Config config, uint8_t* rx_buffer);

//send data over uart in format array of uint8_t
daisy::UartHandler::Result transmit_packet(const uint8_t *data, size_t length);

//transmit std::array of bools
daisy::UartHandler::Result transmit_bools(const std::array<bool, NUM_BOOLS>& bools);

uint32_t GetNumBytesReceived()
{
    return NumBytesReceived_;
}

const uint8_t *GetData(size_t &length);

//debounces buttons. Do not call faster than 1kHz
void Debounce(uint8_t id);

bool RisingEdge(uint8_t id) const;

bool FallingEdge(uint8_t id) const;

//Set Momentary/toggle and polarity for a specific input id
void SetSwitchType(uint8_t id, SwitchType t, bool pol)
{
    flip_ [id] = pol;
    t_[id] = t;
}

//returns true if switch held down or toggle on
bool Pressed(uint8_t id) const;

bool GetPin(uint8_t id);

void GateUpdate(uint8_t id);

bool GateTrig(uint8_t id);

bool GateReleased(uint8_t id);

private:


//DMA memory for rx buffer
//uint8_t DMA_BUFFER_MEM_SECTION rx_buffer_[BUFF_SIZE];
//uint8_t tx_buffer_[BUFF_SIZE];

void ParseByte(uint8_t byte);

//callback for receiving data with interrupt
static void rx_callback(unsigned char *data, size_t size, void *context, daisy::UartHandler::Result result);

//helper function to pack bools into a byte
uint8_t packBools(const std::array<bool, NUM_BOOLS>& bools);

uint8_t calculate_crc8(const uint8_t *data, size_t length);



uint8_t* rx_buffer_;
uint8_t* tx_buffer_;
 
UartHandler uart;
uint8_t rx_payload_[PAYLOAD_LENGTH];

bool dataReceived_;
uint32_t NumBytesReceived_;

ParserState parserState_;
uint8_t temp_buffer_[BUFF_SIZE];
uint8_t LENGTH_BYTE;
uint8_t rx_index_;
uint8_t COBS_OH_BYTE;
bool PacketReceived_;

//debounce variables
uint32_t last_update_[16];
bool     updated_[16];
SwitchType     t_[16];
uint8_t  state_[16]; //debounced state
bool     flip_[16];
bool     toggleState_[16];
float    rising_edge_time_[16];

bool     gate_state_[16];
bool     gate_prev_state_[16];

uint16_t  pin_data;

};

#endif    // End of header