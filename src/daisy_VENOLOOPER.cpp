#include "daisy_VENOLOOPER.h"
#ifndef SAMPLE_RATE
#define SAMPLE_RATE DSY_AUDIO_SAMPLE_RATE /**< & */
#endif

//v1.0 pin assignments
#define PIN_MUX1_SEL1 D9
#define PIN_MUX1_SEL2 D8
#define PIN_MUX1_SEL3 D10
#define PIN_MIDI_RX D14
#define PIN_REC2_GATE D7
#define PIN_I2S1_SCL D11
#define PIN_I2S1_SDA D12
#define PIN_PLAY2_GATE D30
#define PIN_REV2_GATE D29
#define PIN_MUX1_ADC A1
#define PIN_LENGTH_CV2_ADC A4
#define PIN_VOct_CV1_ADC A5
#define PIN_Voct_CV2_ADC A3
#define PIN_LAYER_CV1_ADC A2
#define PIN_LAYER_CV2_ADC A6
#define PIN_START_CV1_ADC A0
#define PIN_START_CV2_ADC A8
#define PIN_LENGTH_CV1_ADC A7
#define PIN_MUX2_ADC A12
#define PIN_MUX2_SEL1 D32
#define PIN_MUX2_SEL2 D0
#define PIN_MUX2_SEL3 D27
#define PIN_PLAY1_GATE D24
#define PIN_REV1_GATE D25
#define PIN_REC1_GATE D26
#define PIN_CLOCK D28
#define PIN_SDMMC_SK D6
#define PIN_SDMMC_CMD D5
#define PIN_SDMMC_D0 D4
#define PIN_SDMMC_D1 D3
#define PIN_SDMMC_D2 D2
#define PIN_SDMMC_D3 D1
#define PIN_MIDI_UART_RX D14
#define PIN_MIDI_UART_TX D13


using namespace daisy;

// static constexpr I2CHandle::Config i2c_config
//     = {I2CHandle::Config::Peripheral::I2C_1,
//        {{DSY_GPIOB, 8}, {DSY_GPIOB, 9}},
//        I2CHandle::Config::Speed::I2C_1MHZ};

static LedDriverPca9685<4, true>::DmaBuffer DMA_BUFFER_MEM_SECTION
    led_dma_buffer_a,
    led_dma_buffer_b;

void VenoLooper::Init(bool boost)
{
    seed.Init(boost);
    seed.SetAudioBlockSize(256);
    seed.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);

    Pin CVpins[] = 
    {seed::PIN_LENGTH_CV2_ADC,
    seed::PIN_VOct_CV1_ADC,
    seed::PIN_Voct_CV2_ADC,
    seed::PIN_LAYER_CV1_ADC,
    seed::PIN_LAYER_CV2_ADC,
    seed::PIN_START_CV1_ADC,
    seed::PIN_START_CV2_ADC,
    seed::PIN_LENGTH_CV1_ADC};

    //ADCs
    AdcChannelConfig adc_cfg[LAST_CV + 2];

    //For some reason muxes only work on channels below 8?
    //mux1
    adc_cfg[0].InitMux(seed::PIN_MUX1_ADC, 
                             8,
                             seed::PIN_MUX1_SEL1,
                             seed::PIN_MUX1_SEL2,
                             seed::PIN_MUX1_SEL3,
                             AdcChannelConfig::ConversionSpeed::SPEED_387CYCLES_5);

    //mux2
    adc_cfg[1].InitMux(seed::PIN_MUX2_ADC, 
                             8,
                             seed::PIN_MUX2_SEL1,
                             seed::PIN_MUX2_SEL2,
                             seed::PIN_MUX2_SEL3,
                             AdcChannelConfig::ConversionSpeed::SPEED_387CYCLES_5);  

    for(size_t i = 0; i < (LAST_CV); i++)
    {
        adc_cfg[i+2].InitSingle(CVpins[i]);
    }    

    seed.adc.Init(adc_cfg,10);

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
    //LEDs
    // 4x PCA9685 addresses 0x00, 0x01,  0x02 and 0x03
    uint8_t   addr[4] = {0x00, 0x01, 0x02, 0x03};
    I2CHandle i2c_led;
    //i2c_led.Init(i2c_config);
    led_driver.Init(i2c_led, addr, led_dma_buffer_a, led_dma_buffer_b);

    //MCP23017 GPIO expander
    Mcp23017::Config mcp_config;
    mcp_config.transport_config.i2c_address = 0x24;
   // mcp_config.transport_config.i2c_config = i2c_config;

    mcp.Init(mcp_config);
    //Port direction, including pullups, inverted:
    mcp.PortMode(MCPPort::A,0xFF,0xFF,0xFF);
    mcp.PortMode(MCPPort::B,0xFF,0xFF, 0xFF);
    
    //set switch types to default:
    for(size_t i=0; i < LAST_DIN; i++)
    {
        mcp.SetSwitchType(i,DefaultSwType[i],0);
    }
    
    //Gates
    gates[0].Init(seed::PIN_PLAY1_GATE);
    gates[1].Init(seed::PIN_REV1_GATE);
    gates[2].Init(seed::PIN_REC1_GATE);

    gates[3].Init(seed::PIN_PLAY2_GATE);
    gates[4].Init(seed::PIN_REV2_GATE);
    gates[5].Init(seed::PIN_REC2_GATE);

    gates[6].Init(seed::PIN_CLOCK);

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

void VenoLooper::DelayMs(size_t del)
{
    seed.DelayMs(del);
}

void VenoLooper::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    seed.StartAudio(cb);
}

void VenoLooper::StartAudio(AudioHandle::AudioCallback cb)
{
    seed.StartAudio(cb);
}

void VenoLooper::StopAudio()
{
    seed.StopAudio();
}

void VenoLooper::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void VenoLooper::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    seed.ChangeAudioCallback(cb);
}

void VenoLooper::SetHidUpdateRates()
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

void VenoLooper::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    seed.SetAudioSampleRate(samplerate);
    SetHidUpdateRates();
}

float VenoLooper::AudioSampleRate()
{
    return seed.AudioSampleRate();
}

void VenoLooper::SetAudioBlockSize(size_t size)
{
    seed.SetAudioBlockSize(size);
    SetHidUpdateRates();
}

size_t VenoLooper::AudioBlockSize()
{
    return seed.AudioBlockSize();
}

float VenoLooper::AudioCallbackRate()
{
    return seed.AudioCallbackRate();
}

void VenoLooper::StartAdc()
{
    seed.adc.Start();
}

void VenoLooper::StopAdc()
{
    seed.adc.Stop();
}

void VenoLooper::ProcessAnalogControls(VenoLooper::CntrlFreq freq)
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

void VenoLooper::ProcessGates()
{
    for (size_t i = 0; i < LAST_GATE; i++)
    {
        gates[i].Update();
    }
        
}

void VenoLooper::ProcessMCP23017()
{
    mcp.Read();
}

void VenoLooper::DebounceMCP23017()
{
    for(size_t i=0; i < LAST_DIN; i++)
    {
        mcp.Debounce(i);
    }
}

float VenoLooper::GetKnobValue(Pots idx)
{
    return pots[idx < LAST_POT ? idx : 0].Value();
}

float VenoLooper::GetCvValue(CVs idx)
{
    return cv[idx < LAST_CV ? idx : 0].Value();
}

bool VenoLooper::GetRawPinState(DigitalInputs idx)
{
    return (mcp.GetPin(idx) == 0xFF) ? true : false;
}

AnalogControl* VenoLooper::GetKnob(size_t idx)
{
    return &pots[idx < LAST_POT ? idx : 0];
}

AnalogControl* VenoLooper::GetCv(size_t idx)
{
    return &cv[idx < LAST_CV ? idx : 0];
}
