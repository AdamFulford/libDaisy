#include "daisy_VENOLOOPER_v5.h"

#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE /**< & */
#endif

#include <array>
#include <vector>
#include <algorithm>

using namespace daisy;


void VenoLooper_v5::Init(bool boost)
{
    seed.Init(boost);
    seed.SetAudioBlockSize(BLOCK_SIZE);
    seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    Pin CVpins[] = 
    {seed::PIN_VOct_CV2_ADC,
    seed::PIN_VOct_CV1_ADC,
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
    float UpdateFreq{};

    //init pot handling
     for(size_t i = 0; i < LAST_MUX; i++)
     {
        if(i<8) //0 -> 7
        {
            muxIndex = i;
            muxChannel = 0;
        }
        else if(i<16) //8 -> 15
        {
            muxIndex = i - 8;
            muxChannel = 1;
        }
        else if(i<24) //16 -> 23
        {
            muxIndex = i - 16;
            muxChannel = 2;
        }

        else //error
        {
            continue; //don't set anything up
        }
        //muxIndex = i < 8 ? i : i-8;
        UpdateFreq = MuxFreq[i] == Slow ? 
        AudioCallbackRate() : 
        AudioCallbackRate() * AudioBlockSize();

        if(i == INPUT1_CV ||
           i == LENGTH1_CV ||
           i == LENGTH2_CV ||
           i == INPUT2_CV)
        {
            MUX_Input[i].InitBipolarCv(seed.adc.GetMuxPtr(muxChannel,muxIndex),
            UpdateFreq,
            MuxSlew[i]);
        }
        else
        {
            MUX_Input[i].Init(seed.adc.GetMuxPtr(muxChannel,muxIndex),
            UpdateFreq,
            false,
            false,
            MuxSlew[i]);
        }

     }

    //MIDI
    MidiUartHandler::Config midi_config;
    midi_config.transport_config.periph = UartHandler::Config::Peripheral::USART_1;
    midi_config.transport_config.rx = seed::PIN_MIDI_UART_RX;
    midi_config.transport_config.tx = seed::PIN_MIDI_UART_TX;
    midi.Init(midi_config);
    midi.StartReceive();

    //gate pins
    std::array<Pin, LAST_GATE> Gatepins = 
    {seed::PIN_PLAY1_GATE,
     seed::PIN_PLAY2_GATE,
     seed::PIN_REC1_GATE,
     seed::PIN_REC2_GATE,
     seed::PIN_CLOCK};

    // //pointers to gate pins
    // Pin* GatePinPtrs[LAST_GATE] {};

    // for (size_t i=0; i<LAST_GATE; ++i)
    // {
    //     GatePinPtrs[i] = &Gatepins[i];
    // }

    //gates init
    for(size_t i=0; i<LAST_GATE; ++i)
    {
        gates[i].Init(Gatepins[i], true);
    }

    PicoUart.SetSwitchType(Pico_Gates::REV1_GATE,
                           PicoUartTXRX::SwitchType::TYPE_MOMENTARY, 
                           false);

    PicoUart.SetSwitchType(Pico_Gates::REV2_GATE,
                           PicoUartTXRX::SwitchType::TYPE_MOMENTARY, 
                           false);

    //buttons
    for(size_t i=0; i<LAST_PICO_BUTTON; ++i)
    {
        PicoUart.SetSwitchType(i,PicoUartTXRX::SwitchType::TYPE_MOMENTARY,false);
    }

    //Init for Pico UART inputs called from main() separately to this function,
    //to connect to rx_buff

    //Gate (EOC) outputs
    gateOut[0].Init(seed::PIN_EOC1,GPIO::Mode::OUTPUT);
    gateOut[1].Init(seed::PIN_EOC2,GPIO::Mode::OUTPUT);

    //Encoder
    encoder.Init(seed::PIN_ENC_UP, seed::PIN_ENC_DOWN);

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
    //PicoUart.StartListening();
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
    UpdateDaisyGates();
    UpdatePicoGates();
    
}

//functions for pico UART inputs

void VenoLooper_v5::DebouncePicoButtons()
{
    for(size_t i=0; i<LAST_PICO_BUTTON; ++i)
    {
         PicoUart.Debounce(i);
    }
}

void VenoLooper_v5::UpdatePicoGates()
{
    for(size_t i=LAST_PICO_BUTTON; i<LAST_PICO_BUTTON + LAST_PICO_GATE; ++i)
    {
         PicoUart.GateUpdate(i);
    }
}

void VenoLooper_v5::UpdateDaisyGates()
{
    for(size_t i=0; i<LAST_GATE; ++i)
    {
        gates[i].Update();
    }
}

float VenoLooper_v5::GetMuxValue(MUX_IDs idx)
{
    float retVal{(MUX_Input[idx < LAST_MUX ? idx : 0].Value()
        * calibration_.MuxScale[idx]) 
        + calibration_.MuxOffsets[idx]};

        //return std::min(std::max(retVal,0.0f),1.0f);

        return std::min(retVal,1.0f);
}

float VenoLooper_v5::GetCvValue(CV_IDs idx)
{
    float retVal{(cv[idx < LAST_CV ? idx : 0].Value()
        * calibration_.CV_Scale[idx]) 
        + calibration_.CV_Offsets[idx]};

    // return std::min(std::max(retVal,0.0f),1.0f);
    return std::min(retVal,1.0f);
}

AnalogControl* VenoLooper_v5::GetKnob(size_t idx)
{
    return &MUX_Input[idx < LAST_MUX ? idx : 0];
}

AnalogControl* VenoLooper_v5::GetCv(size_t idx)
{
    return &cv[idx < LAST_CV ? idx : 0];
}

void VenoLooper_v5::UpdateLEDs()
{
    //copy neopixel data into active buffer
    memcpy(active_buffer,NeoPixelState,sizeof(NeoPixelState));

    //pack LED states into last byte of active buffer
    uint8_t packed = 0;

    int NumBits{NUM_BOOLS <= 8 ? NUM_BOOLS : 8}; 
    //don't allow num bits to be larger than 8
    for (int i = 0; i < NumBits; i++) {
        packed |= (ButtonLED_State[i] & 0x01) << i; // Shift each boolean into the correct bit position
    }
    //set final byte in active buffer
    active_buffer[NUM_NEOPIXELS * 3] = packed;

    //swap over buffers
    auto* temp = active_buffer;
    active_buffer = ready_buffer;
    ready_buffer = temp;
}

void VenoLooper_v5::TransmitLED_States()
{
    //transmit bools
    PicoUart.transmit_packet(ready_buffer, (NUM_NEOPIXELS * 3) + 1);
}

void VenoLooper_v5::SetNeoPixel(uint8_t id, uint8_t Red, uint8_t Green, uint8_t Blue)
{
    if(id < NUM_NEOPIXELS)
    {
        NeoPixelState[id * 3] = Red;
        NeoPixelState[(id * 3) + 1] = Green;
        NeoPixelState[(id*3) + 2] = Blue;
    }
        //set relevant bytes in active buffer (first 180 relate to neopixels)
}

void VenoLooper_v5::SetLayerLED(uint8_t channel, uint8_t layer, uint8_t Red, uint8_t Green, uint8_t Blue)
{
    if(channel == 0)
    {
        SetNeoPixel(LAYER1_START - layer,
                    Red,
                    Green,
                    Blue);
    }
    if(channel == 1)
    {
        SetNeoPixel(LAYER2_START + layer,
                    Red,
                    Green,
                    Blue);
    }
    
}

 void VenoLooper_v5::SetRingLED(uint8_t channel, uint8_t position, uint8_t Red, uint8_t Green, uint8_t Blue)
 {
    if (position > NUM_RING_PIXELS)
    {
        //don't set anything
    }
    else
    {
        if(channel == 0)
        {
            SetNeoPixel(RING1_START - position,Red,Green,Blue);
        }

        if(channel == 1)
        {
            SetNeoPixel(RING2_START - position,Red,Green,Blue);
        }
    }
 }

 void VenoLooper_v5::SetBankLED(uint8_t slot, uint8_t Red, uint8_t Green, uint8_t Blue)
 {
    if(slot > NUM_BANK_PIXELS)
    {
        //out of range, don't set
    }
    else
    {
        SetNeoPixel(BANK_START - slot,Red,Green,Blue);
    }
 }

void VenoLooper_v5::ClearAllNeoPixels()
{
    memset(NeoPixelState,0,sizeof(NeoPixelState));
}

void VenoLooper_v5::ClearAllButtonLEDs()
{
    memset(ButtonLED_State,false,NUM_BUTTON_LEDS);
}

void VenoLooper_v5::SetButtonLED(PICO_Button_LEDs id, bool state)
{
    if(id < LAST_BUTTON_LED) //check id is within bounds
    {
        ButtonLED_State[id] = state;
    }
}

void VenoLooper_v5::SetCalibration(CV_Calibration& calibration)
{
    calibration_ = calibration;
}

float VenoLooper_v5::GetAudioOffset(size_t channel)
{
    if(channel >= 2) {return 0.0f;}

    return(calibration_.AudioOffsets[channel]);
}

bool VenoLooper_v5::ValidateQSPI(bool quick)
    {
        uint32_t start;
        uint32_t size;
        if(quick)
        {
            start = 0x400000;
            size  = 0x4000;
        }
        else
        {
            start = 0;
            size  = 0x800000;
        }
        // Erase the section to be tested
        seed.qspi.Erase(start, start + size);
        // Create some test data
        std::vector<uint8_t> test;
        test.resize(size);
        uint8_t *testmem = test.data();
        for(size_t i = 0; i < size; i++)
            testmem[i] = (uint8_t)(i & 0xff);
        // Write the test data to the device
        seed.qspi.Write(start, size, testmem);
        // Read it all back and count any/all errors
        // I supppose any byte where ((data & 0xff) == data)
        // would be able to false-pass..
        size_t fail_cnt = 0;
        for(size_t i = 0; i < size; i++)
            if(testmem[i] != (uint8_t)(i & 0xff))
                fail_cnt++;
        return fail_cnt == 0;
    }