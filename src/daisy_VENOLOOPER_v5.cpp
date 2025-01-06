#include "daisy_VENOLOOPER_v5.h"
#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE /**< & */
#endif

using namespace daisy;


void VenoLooper_v5::Init(bool boost)
{
    seed.Init(boost);
    seed.SetAudioBlockSize(BLOCK_SIZE);
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

    seed.adc.Init(adc_cfg, (LAST_MUX / 8 ) + LAST_CV);

    float CVUpdateFreq{};
     //init CV handling
    for(size_t i = 0; i < LAST_CV; i++)
    {
        CVUpdateFreq = CVFreq[i] == Slow ? 
                                    AudioCallbackRate() : 
                                    AudioCallbackRate() * AudioBlockSize();

        cv[i].InitBipolarCv(seed.adc.GetPtr(i+3), CVUpdateFreq,CVSlew[i]);
    }

    uint8_t muxIndex{};
    uint8_t muxChannel{};
    float potUpdateFreq{};

    //init pot handling
     for(size_t i = 0; i < LAST_MUX; i++)
     {
        if(i<8) //0 -> 7
        {
            muxIndex = i;
            muxChannel = 0;
        }
        else if(i<16) //8 -> 16
        {
            muxIndex = i - 8;
            muxChannel = 1;
        }
        else //16 -> 24
        {
            muxIndex = i - 16;
            muxChannel = 2;
        }
        //muxIndex = i < 8 ? i : i-8;
        potUpdateFreq = MuxFreq[i] == Slow ? 
        AudioCallbackRate() : 
        AudioCallbackRate() * AudioBlockSize();

        MUX_Input[i].Init(seed.adc.GetMuxPtr(muxChannel,muxIndex),
        potUpdateFreq,
        false,
        false,
        MuxSlew[i]);
     }

    //MIDI
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.rx = seed::PIN_MIDI_UART_RX;
    midi_config.transport_config.tx = seed::PIN_MIDI_UART_TX;
    midi.Init(midi_config);
    midi.StartReceive();

    //Encoder
    //SD Card
}

void VenoLooper_v5::InitPicoUart(uint8_t* rx_buff)
{
    PicoUartTXRX::Config config;
    config.periph = UartHandler::Config::Peripheral::UART_4;
    config.baudrate = BAUD_RATE;
    config.tx = Pin(seed::PIN_UART_STM_TO_RP2040);
    config.rx = Pin(seed::PIN_UART_RP2040_TO_STM);

    PicoUart.Init(config, rx_buff);

    PicoUart.StartListening();
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
    for(size_t i = 0; i < LAST_MUX; i++)
    {
        MUX_Input[i].SetSampleRate(AudioCallbackRate());
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
    for(size_t i = 0; i < LAST_MUX; i++)
    {
        if(MuxFreq[i]==freq)
            MUX_Input[i].Process();
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

    //add RP2040 gates (rev L and rev R)
        
}

//functions for pico UART inputs

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

float VenoLooper_v5::GetMuxValue(MUX_IDs idx)
{
    return MUX_Input[idx < LAST_MUX ? idx : 0].Value();
}

float VenoLooper_v5::GetCvValue(CV_IDs idx)
{
    return cv[idx < LAST_CV ? idx : 0].Value();
}

// bool VenoLooper_v5::GetRawPinState(MUX_Inputs idx)
// {
//     return (mcp.GetPin(idx) == 0xFF) ? true : false;
// }

AnalogControl* VenoLooper_v5::GetKnob(size_t idx)
{
    return &MUX_Input[idx < LAST_MUX ? idx : 0];
}

AnalogControl* VenoLooper_v5::GetCv(size_t idx)
{
    return &cv[idx < LAST_CV ? idx : 0];
}
