#include "PICO_Uart_TXRX.h"

void PicoUartTXRX::Init(Config config, uint8_t* rx_buffer)
{
    //tx_buffer_ = tx_buffer;
    rx_buffer_ = rx_buffer;
    //initialise uart periphal
    UartHandler::Config UartConfig{};

    UartConfig.periph = config.periph;
    UartConfig.pin_config.tx = config.tx;
    UartConfig.pin_config.rx = config.rx;
    UartConfig.baudrate = config.baudrate;

    UartConfig.mode = UartHandler::Config::Mode::TX_RX;

    uart.Init(UartConfig);

    memset(rx_buffer_,0x00,BUFF_SIZE);
    //memset(tx_buffer_,0x00,BUFF_SIZE);

    NumBytesReceived_ = 0;

    parserState_ = ParserState::WAIT_FOR_START_BYTE;
    memset(temp_buffer_,0x00,sizeof(temp_buffer_));
    LENGTH_BYTE = 0x00;
    rx_index_ = 0;
    COBS_OH_BYTE = 255;
    PacketReceived_ = false;

    memset(last_update_, 0, sizeof(last_update_));
    memset(updated_, false, sizeof(updated_));
    //memset(t_,SwitchType::TYPE_MOMENTARY,sizeof(t_));
    memset(state_, 0x00, sizeof(state_));
    //memset(flip_, false, sizeof(flip_));
    memset(toggleState_, false , sizeof(toggleState_));
    memset(rising_edge_time_, 0 , sizeof(rising_edge_time_));

    memset(gate_state_,false,sizeof(gate_state_));
    memset(gate_prev_state_,false,sizeof(gate_prev_state_));

    //System::Delay(1000);

}


daisy::UartHandler::Result PicoUartTXRX::StartListening()
{

    //start receiving
    return uart.DmaListenStart(rx_buffer_,BUFF_SIZE,PicoUartTXRX::rx_callback, this);

}

daisy::UartHandler::Result PicoUartTXRX::transmit_packet(const uint8_t *data, size_t length)
{
    // Ensure the packet length fits within the buffer (including COBS, CRC, start/end bytes)
    if (length + 5 > BUFF_SIZE) {
        // Handle error: Data too large for buffer
        return daisy::UartHandler::Result::ERR;
    }

    // Create a single temporary buffer
    uint8_t temp_buff[BUFF_SIZE] {};

    // Set Start Byte and calculate COBS overhead
    temp_buff[0] = START_BYTE;
    temp_buff[1] = cobs::cobs_calcOverhead(data, length, START_BYTE);
    temp_buff[2] = length;

    // Copy payload directly into the temp buffer starting from index 2
    memcpy(&temp_buff[3], data, length);

    // COBS stuff the payload in-place within the temp buffer
    //lenght of length + 1? Check output
    cobs::cobs_stuffPacket(&temp_buff[3], length , START_BYTE);

    // Calculate CRC and set it after the payload
    uint8_t crc = calculate_crc8(&temp_buff[3], length);
    temp_buff[length + 3] = crc;

    // Set End Byte
    temp_buff[length + 4] = END_BYTE;

    // Copy the entire packet to the DMA buffer
    //memcpy(tx_buffer_, temp_buff, length + 5);  // Copy only the valid part of the packet

    return uart.BlockingTransmit(temp_buff,sizeof(temp_buff));
}

daisy::UartHandler::Result
    PicoUartTXRX::transmit_bools(const std::array<bool, NUM_BOOLS>& bools)
{
    uint8_t data[PAYLOAD_LENGTH]{};

    data[0] = packBools(bools);

    return transmit_packet(data, PAYLOAD_LENGTH);
}

//callback at TC, HT and IDLE
void PicoUartTXRX::rx_callback(unsigned char *data, 
                                size_t size, 
                                void *context, 
                                daisy::UartHandler::Result result)
{
    
    //allows access to the non-static class members by 
    //through the context, which points at the PicoUartTXRX class instance that
    //invoked the rx_callback
    auto *instance = static_cast<PicoUartTXRX *>(context);

    for (size_t i = 0; i < size; ++i)
    {
        instance->ParseByte(data[i]);
    }

}

uint8_t PicoUartTXRX::packBools(const std::array<bool, NUM_BOOLS>& bools)
{
    uint8_t packed = 0;

    int NumBits{NUM_BOOLS <= 8 ? NUM_BOOLS : 8}; 
    //don't allow num bits to be larger than 8
    for (int i = 0; i < NumBits; i++) {
        packed |= (bools[i] & 0x01) << i; // Shift each boolean into the correct bit position
    }
    return packed;
}

uint8_t PicoUartTXRX::calculate_crc8(const uint8_t *data, size_t length)
{
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x07 : (crc << 1);
        }
    }
    return crc;
}

void PicoUartTXRX::ParseByte(uint8_t byte) 
{

        ++NumBytesReceived_;

        static uint8_t temp_buffer[MAX_PACKET_SIZE];
        //byte by byte passing routine:
        switch(parserState_)
        {
            case WAIT_FOR_START_BYTE:
            {
                if(byte == START_BYTE)
                {
                    parserState_ = WAIT_FOR_OH_BYTE;
                }
            break;
            }

            case ParserState::WAIT_FOR_OH_BYTE:
            {
                COBS_OH_BYTE = byte;
                parserState_ = ParserState::WAIT_FOR_LENGTH;

            break;
            }

            case ParserState::WAIT_FOR_LENGTH:
            {
                if(byte > 0 && byte <= MAX_PACKET_SIZE)
                {
                    LENGTH_BYTE = byte;
                    parserState_ = ParserState::WAIT_FOR_PAYLOAD;
                    rx_index_ = 0; //reset index

                }
                else
                {
                    //payload error, reset
                    parserState_ = ParserState::WAIT_FOR_START_BYTE;
                    rx_index_ = 0;
                }
            break;
            }

            case ParserState::WAIT_FOR_PAYLOAD:
            {
                if (rx_index_ < LENGTH_BYTE)
                {
                    temp_buffer[rx_index_] = byte;
                    rx_index_++;

                    if (rx_index_ == LENGTH_BYTE)
                        parserState_    = ParserState::WAIT_FOR_CRC;
                }
            break;
            }

            case ParserState::WAIT_FOR_CRC:
            {
            uint8_t calcCRC = calculate_crc8(temp_buffer,LENGTH_BYTE);

			if (calcCRC == byte)
            {
				parserState_ = ParserState::WAIT_FOR_END_BYTE;
            }
			else
			{
				//bytesRead = 0;
                memset(temp_buffer,0x00,sizeof(temp_buffer));
				parserState_ = ParserState::WAIT_FOR_START_BYTE;
                rx_index_ = 0;
			}

            break;
            }
            
            case ParserState::WAIT_FOR_END_BYTE:
            {
                parserState_ = ParserState::WAIT_FOR_START_BYTE;

                if (byte == END_BYTE)
                {
                    //unpack:
                    if(cobs::cobs_unpackPacket(temp_buffer,COBS_OH_BYTE,START_BYTE,LENGTH_BYTE))
                    { //success
                        //copy data to rx_payload
                        memcpy(rx_payload_,temp_buffer,LENGTH_BYTE);
                        //set flag
                        PacketReceived_ = true;

                    }
                    else //error, don't copy data
                    {
                        
                    }
                }
                
                //clear temp buffer and reset index
                memset(temp_buffer,0x00,sizeof(temp_buffer));
                rx_index_ = 0;
            
            break;
            }

            default:
            {
            //invalid state: reset
            //clear temp buffer and reset index
            memset(temp_buffer,0x00,sizeof(temp_buffer));
            parserState_ = ParserState::WAIT_FOR_START_BYTE;
            rx_index_ = 0;

            break;
            }  
        }
    } 

    const uint8_t* PicoUartTXRX::GetData(size_t &length) 
    {
    
        length = PAYLOAD_LENGTH; // Set the size of valid data
        return rx_payload_;           // Return pointer to the buffer
    
    }

bool PicoUartTXRX::GetPin(uint8_t id)
{
    if(id > 15)
    {
        return false;
    }
    else
    {
    // Combine the two payload bytes into a 16-bit integer
    uint16_t data = (rx_payload_[1] << 8) | rx_payload_[0];
    
    // Return true if the bit at position 'pos' is set, false otherwise
    return (data & (1 << id)) != 0;
    }
}

void PicoUartTXRX::Debounce(uint8_t id) 
{
// update no faster than 1kHz
    uint32_t now = System::GetNow();
    updated_[id]     = false;

    if(now - last_update_[id] >= 1)
    {
        last_update_[id] = now;
        updated_[id]     = true;

        // shift over, and introduce new state.
        
        uint8_t pinRead{};
        if (GetPin(id))
            pinRead = 0x01;
        else
            pinRead = 0x00;    
        
        state_[id]
            = (state_[id] << 1)
              | (flip_[id] ? !pinRead : pinRead);
        // Set time at which button was pressed
        if(state_[id] == 0x7f)
            {
                rising_edge_time_[id] = System::GetNow();
                if(t_[id] == TYPE_TOGGLE)
                {
                    toggleState_[id] = !toggleState_[id];
                }
            }
    }
}

bool PicoUartTXRX::RisingEdge(uint8_t id) const
{
    return updated_[id] ? state_[id] == 0x7f : false; 
}

bool PicoUartTXRX::FallingEdge(uint8_t id) const
{
    return updated_[id] ? state_[id] == 0x80 : false; //10000000
}

bool PicoUartTXRX::Pressed(uint8_t id) const
{ 
    bool retval;
    if(t_[id] == TYPE_TOGGLE)
    {
        retval = toggleState_[id];
    }
    else
    {
        retval = (state_[id] == 0xff);
    }
    return retval; 
}

bool PicoUartTXRX::GateState(uint8_t id)
{
    return gate_state_[id];
}

void PicoUartTXRX::GateUpdate(uint8_t id)
{
    gate_prev_state_[id] = gate_state_[id];
    gate_state_[id] = flip_[id] ? !GetPin(id) : GetPin(id);
}

bool PicoUartTXRX::GateTrig(uint8_t id)
{
    return gate_state_[id] && !gate_prev_state_[id];
}

bool PicoUartTXRX::GateReleased(uint8_t id)
{
    return !gate_state_[id] && gate_prev_state_[id];  
}

