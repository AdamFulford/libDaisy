#include "daisy_VENOLOOPER_v5.h"
#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE /**< & */
#endif

//v5.1 pin assignments
//fix with seed assignments
#define PIN_EOC2 D9
#define PIN_EOC1 D8

#define PIN_ENC_DOWN D10
#define PIN_MIDI_UART_RX D14
#define PIN_MIDI_UART_TX D13
#define PIN_PLAY2_GATE D7
#define PIN_UART_RP2040_TO_STM D11
#define PIN_UART_STM_TO_RP2040 D12
#define PIN_REC2_GATE D30

#define PIN_MUX1_ADC A1
#define PIN_ENC_UP D19
#define PIN_Voct_CV2_ADC A5
#define PIN_VOct_CV1_ADC A3
#define PIN_LAYER_CV1_ADC A2
#define PIN_LAYER_CV2_ADC A6
#define PIN_START_CV1_ADC A0
#define PIN_START_CV2_ADC A8
#define PIN_MUX3_ADC A7
#define PIN_MUX2_ADC A12

#define PIN_MUX_SEL3 D32
#define PIN_MUX_SEL1 D0
#define PIN_MUX_SEL2 D27

#define PIN_REC1_GATE D25
#define PIN_CLOCK D26
#define PIN_PLAY1_GATE D28

#define PIN_SDMMC_SK D6
#define PIN_SDMMC_CMD D5
#define PIN_SDMMC_D0 D4
#define PIN_SDMMC_D1 D3
#define PIN_SDMMC_D2 D2
#define PIN_SDMMC_D3 D1

#define BAUD_RATE 31250

using namespace daisy;

static constexpr I2CHandle::Config i2c_config
    = {I2CHandle::Config::Peripheral::I2C_1,
       {{DSY_GPIOB, 8}, {DSY_GPIOB, 9}},
       I2CHandle::Config::Speed::I2C_1MHZ};

static LedDriverPca9685<4, true>::DmaBuffer DMA_BUFFER_MEM_SECTION
    led_dma_buffer_a,
    led_dma_buffer_b;

void VenoLooper_v5::Init(bool boost)
{
    seed.Init(boost);
    seed.SetAudioBlockSize(256);
    seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    dsy_gpio_pin CVpins[] = 
    {seed::PIN_VOct_CV1_ADC,
    seed::PIN_Voct_CV2_ADC,
    seed::PIN_LAYER_CV1_ADC,
    seed::PIN_LAYER_CV2_ADC,
    seed::PIN_START_CV1_ADC,
    seed::PIN_START_CV2_ADC};

    //ADCs
    AdcChannelConfig adc_cfg[LAST_CV + 2];

    //For some reason muxes only work on channels below 8?
    //mux1
    adc_cfg[0].InitMux(seed::PIN_MUX1_ADC, 
                             8,
                             seed::PIN_MUX_SEL1,
                             seed::PIN_MUX_SEL2,
                             seed::PIN_MUX_SEL3,
                             AdcChannelConfig::ConversionSpeed::SPEED_387CYCLES_5);

    //mux2
    adc_cfg[1].InitMux(seed::PIN_MUX2_ADC, 
                             8,
                             seed::PIN_MUX_SEL1,
                             seed::PIN_MUX_SEL2,
                             seed::PIN_MUX_SEL3,
                             AdcChannelConfig::ConversionSpeed::SPEED_387CYCLES_5);  

        //mux3
    adc_cfg[2].InitMux(seed::PIN_MUX3_ADC, 
                             8,
                             seed::PIN_MUX_SEL1,
                             seed::PIN_MUX_SEL2,
                             seed::PIN_MUX_SEL3,
                             AdcChannelConfig::ConversionSpeed::SPEED_387CYCLES_5);  

    for(size_t i = 0; i < (LAST_CV); i++)
    {
        adc_cfg[i+3].InitSingle(CVpins[i]);
    }    

    seed.adc.Init(adc_cfg,11);

    float CVUpdateFreq{};
     //init CV handling
    for(size_t i = 0; i < LAST_CV; i++)
    {
        CVUpdateFreq = CVFreq[i] == Slow ? AudioCallbackRate() : AudioCallbackRate() * AudioBlockSize();
        cv[i].InitBipolarCv(seed.adc.GetPtr(i+2), CVUpdateFreq,CVSlew[i]);
    }

    uint8_t potIndex{};
    float potUpdateFreq{};

    //init pot handling
     for(size_t i = 0; i < LAST_POT; i++)
     {
        potIndex = i < 8 ? i : i-8;
        potUpdateFreq = PotFreq[i] == Slow ? AudioCallbackRate() : AudioCallbackRate() * AudioBlockSize();
        pots[i].Init(seed.adc.GetMuxPtr(potIndex == i ? 0 : 1,potIndex),potUpdateFreq,false,false,PotSlew[i]);
     }


    //Gates
    // gates[0].Init(seed::PIN_PLAY1_GATE);
    // gates[1].Init(seed::PIN_REV1_GATE);
    // gates[2].Init(seed::PIN_REC1_GATE);

    // gates[3].Init(seed::PIN_PLAY2_GATE);
    // gates[4].Init(seed::PIN_REV2_GATE);
    // gates[5].Init(seed::PIN_REC2_GATE);

    // gates[6].Init(seed::PIN_CLOCK);

    //PICO Uart connection
    PicoUartTXRX::Config config;

    config.periph = UartHandler::Config::Peripheral::UART_4;
    config.baudrate = BAUD_RATE;
    config.tx = Pin(PORTB,9);
    config.rx = Pin(PORTB,8);

   // PicoUart.Init(config);

    //Encoder
    //SD Card
    //MIDI
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.rx = seed::PIN_MIDI_UART_RX;
    midi_config.transport_config.tx = seed::PIN_MIDI_UART_TX;
    midi.Init(midi_config);
    midi.StartReceive();
}

void VenoLooper_v5::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void VenoLooper_v5::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void VenoLooper_v5::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void VenoLooper_v5::StopAudio()
{
    seed.StopAudio();
}

void VenoLooper_v5::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void VenoLooper_v5::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void VenoLooper_v5::SetHidUpdateRates()
{
    //set the hids to the new update rate
    for(size_t i = 0; i < LAST_POT; i++)
    {
        pots[i].SetSampleRate(AudioCallbackRate());
    }
    for(size_t i = 0; i < LAST_CV; i++)
    {
        cv[i].SetSampleRate(AudioCallbackRate());
    }
}

void VenoLooper_v5::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float VenoLooper_v5::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

void VenoLooper_v5::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t VenoLooper_v5::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

float VenoLooper_v5::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void VenoLooper_v5::StartAdc()
{
    seed.adc.Start();
}

void VenoLooper_v5::StopAdc()
{
    seed.adc.Stop();
}

void VenoLooper_v5::ProcessAnalogControls(VenoLooper_v5::CntrlFreq freq)
{
    for(size_t i = 0; i < LAST_POT; i++)
    {
        if(PotFreq[i]==freq)
            pots[i].Process();
    }


    for(size_t i = 0; i < LAST_CV; i++)
    {
        if(CVFreq[i]==freq)
            cv[i].Process();
    }

}

void VenoLooper_v5::ProcessGates()
{
    for (size_t i = 0; i < LAST_GATE; i++)
    {
        gates[i].Update();
    }
        
}

// void VenoLooper_v5::ProcessMCP23017()
// {
//     mcp.Read();
// }

// void VenoLooper_v5::DebounceMCP23017()
// {
//     for(size_t i=0; i < LAST_PICO_INPUT; i++)
//     {
//         mcp.Debounce(i);
//     }
// }

float VenoLooper_v5::GetKnobValue(MUX_Inputs idx)
{
    return pots[idx < LAST_POT ? idx : 0].Value();
}

float VenoLooper_v5::GetCvValue(CVs idx)
{
    return cv[idx < LAST_CV ? idx : 0].Value();
}

// bool VenoLooper_v5::GetRawPinState(MUX_Inputs idx)
// {
//     return (mcp.GetPin(idx) == 0xFF) ? true : false;
// }

AnalogControl* VenoLooper_v5::GetKnob(size_t idx)
{
    return &pots[idx < LAST_POT ? idx : 0];
}

AnalogControl* VenoLooper_v5::GetCv(size_t idx)
{
    return &cv[idx < LAST_CV ? idx : 0];
}
