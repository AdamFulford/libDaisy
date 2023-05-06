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

using namespace daisy;

static constexpr I2CHandle::Config led_i2c_config
    = {I2CHandle::Config::Peripheral::I2C_1,
       {{DSY_GPIOB, 8}, {DSY_GPIOB, 9}},
       I2CHandle::Config::Speed::I2C_400KHZ};

static LedDriverPca9685<4, true>::DmaBuffer DMA_BUFFER_MEM_SECTION
    led_dma_buffer_a,
    led_dma_buffer_b;

void VenoLooper::Init(bool boost)
{
    seed.Init(boost);
    seed.SetAudioBlockSize(48);

    dsy_gpio_pin CVpins[] = 
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


    //mux1
    adc_cfg[0].InitMux(seed::PIN_MUX1_ADC, 
                             8,
                             seed::PIN_MUX1_SEL1,
                             seed::PIN_MUX1_SEL2,
                             seed::PIN_MUX1_SEL3);

    //mux2
    adc_cfg[1].InitMux(seed::PIN_MUX2_ADC, 
                             8,
                             seed::PIN_MUX2_SEL1,
                             seed::PIN_MUX2_SEL2,
                             seed::PIN_MUX2_SEL3);  

    for(size_t i = 0; i < (LAST_CV); i++)
    {
        adc_cfg[i+2].InitSingle(CVpins[i]);
    }    

    seed.adc.Init(adc_cfg,10);

     //init CV handling
    for(size_t i = 0; i < LAST_CV; i++)
    {
        cv[i].InitBipolarCv(seed.adc.GetPtr(i+2), AudioCallbackRate());
    }

    //init pot handling
     for(size_t i = 0; i < LAST_POT; i++)
     {
        if(i < 8)
            pots[i].Init(seed.adc.GetMuxPtr(0,i),AudioCallbackRate());
        else
            pots[i].Init(seed.adc.GetMuxPtr(1,i-8),AudioCallbackRate());
     }

    //LEDs
    // 4x PCA9685 addresses 0x00, 0x01,  0x02 and 0x03
    uint8_t   addr[4] = {0x00, 0x01, 0x02, 0x03};
    I2CHandle i2c;
    i2c.Init(led_i2c_config);
    led_driver.Init(i2c, addr, led_dma_buffer_a, led_dma_buffer_b);

    // Gate In
    dsy_gpio_pin gate_in_pin;
    gate_in_pin = seed::PIN_CLOCK;
    gate_in.Init(&gate_in_pin);

    //MCP23017 GPIO expander
    //Gates
    //Buttons
    //Encoder
    //SD Card
    //MIDI
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

void VenoLooper::ProcessAnalogControls()
{
    for(size_t i = 0; i < LAST_POT; i++)
        pots[i].Process();
    for(size_t i = 0; i < LAST_CV; i++)
        cv[i].Process();
}

void VenoLooper::ProcessDigitalControls()
{
        // Gate Input
    gate_in_trig_ = gate_in.Trig();
}

float VenoLooper::GetKnobValue(size_t idx) const
{
    return pots[idx < LAST_POT ? idx : 0].Value();
}

float VenoLooper::GetCvValue(size_t idx) const
{
    return cv[idx < LAST_CV ? idx : 0].Value();
}

AnalogControl* VenoLooper::GetKnob(size_t idx)
{
    return &pots[idx < LAST_POT ? idx : 0];
}

AnalogControl* VenoLooper::GetCv(size_t idx)
{
    return &cv[idx < LAST_CV ? idx : 0];
}