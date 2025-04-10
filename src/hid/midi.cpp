#include "midi.h"

namespace daisy
{
static constexpr size_t kDefaultMidiRxBufferSize = 256;//used to be 8192

static uint8_t DMA_BUFFER_MEM_SECTION
    default_midi_rx_buffer[kDefaultMidiRxBufferSize];

MidiUartTransport::Config::Config()
{
    periph         = UartHandler::Config::Peripheral::USART_1;
    rx             = Pin(PORTB, 7);
    tx             = Pin(PORTB, 6);
    rx_buffer      = default_midi_rx_buffer;
    rx_buffer_size = kDefaultMidiRxBufferSize;
}
} // namespace daisy
