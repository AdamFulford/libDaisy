#pragma once
#ifndef VENO_LOOPER_V5_BOARD_H
#define VENO_LOOPER_V5_BOARD_H /**< & */
#include <map>
#include "daisy_seed.h"

//v5.1 pin assignments
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
#define PIN_VOct_CV2_ADC A5
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

#define BAUD_RATE 921600 
#define BLOCK_SIZE 128 //256
#define NUM_BUTTON_LEDS 6

#define NUM_NEOPIXELS 60

#define RING1_START 55
#define RING1_END 34

#define RING2_START 25
#define RING2_END 4

#define LAYER1_START 59
#define LAYER2_START 0

#define BANK_START 33

#define NUM_RING_PIXELS 22
#define NUM_BANK_PIXELS 8

namespace daisy
{

class VenoLooper_v5
{
    public:

    enum PICO_Button_LEDs //enum for addressing button LEDs
    {
        REC1_LED,
        PLAY1_LED,
        REV1_LED,
        REC2_LED,
        PLAY2_LED,
        REV2_LED,
        LAST_BUTTON_LED
    };

    enum Layer1Leds
    {

    };

    enum Layer2Leds
    {

    };

    enum Pico_Buttons
    {
    REC1_BUTTON,
    PLAY1_BUTTON,
    REV1_BUTTON,   
    STOP1_BUTTON,
    CLR1_BUTTON,
    REC2_BUTTON,
    PLAY2_BUTTON,
    REV2_BUTTON,
    STOP2_BUTTON,
    CLR2_BUTTON,
    SAVE_BUTTON,
    ENC_CLICK_BUTTON,
    LINK_SW,
    QUANTISE_SW,

    LAST_PICO_BUTTON
    };

    enum Pico_Gates
    {
        REV1_GATE = 14,
        REV2_GATE = 15,
        LAST_PICO_GATE = 16
    };

    //order of inputs connected to MUXs
    enum MUX_IDs
    {
        //mux1
        LENGTH1_POT,   //000
        SPEED2_POT, //001
        START1_POT, // 010
        LENGTH2_POT,  //011
        XFADE_POT, //100
        PLAY_TOGGLE2_POT, //101
        DECAY_POT, //110
        PLAY_TOGGLE1_POT, //111

        //mux2
        ATTACK1_POT,
        START2_POT,
        SPEED1_POT,
        ATTACK2_POT,
        LAYER1_POT,
        RELEASE2_POT,
        LAYER2_POT,
        RELEASE1_POT,

        //mux3
        INPUT1_CV,
        LENGTH1_CV,
        LENGTH2_CV,
        MUX3_3,
        INPUT2_CV,
        MUX3_5,
        MUX3_6,
        CRUSH_POT,

        LAST_MUX
    };

    enum speedControls
    {
        SPEED1_POT_CTRL,
        SPEED2_POT_CTRL,

        SPEED1_CV_CTRL,
        SPEED2_CV_CTRL,

        LAST_SPEED    
    };


    enum CV_IDs
    {   
        VOct2_CV,
        VOct1_CV,
        LAYER1_CV,
        LAYER2_CV,
        START1_CV,
        START2_CV,
        LAST_CV
    };

    enum Daisy_Gates
    {
        PLAY1_GATE,
        PLAY2_GATE,
        REC1_GATE,
        REC2_GATE,
        CLOCK_GATE,

        LAST_GATE
    };

    enum GateOuts
    {
        EOC1,
        EOC2,

        LAST_GATE_OUT
    };

    enum CntrlFreq
    {Fast,Slow};

 
//mux input slew coeffs
float MuxSlew[LAST_MUX] =
{
    //mux1
    0.05f,    // LENGTH1_POT,   //000
    0.05f,    // SPEED2_POT, //001
    0.05f,    // START1_POT, // 010
    0.05f,    // LENGTH2_POT,  //011
    0.05f,    // XFADE_POT, //100
    0.05f,    // PLAY_TOGGLE2_POT, //101
    0.05f,    // DECAY_POT, //110
    0.05f,    // PLAY_TOGGLE1_POT, //111

    //mux2
    0.05f,    // ATTACK1_POT,
    0.05f,    // START2_POT,
    0.05f,    // SPEED1_POT,
    0.05f,    // ATTACK2_POT,
    0.000001f,    // LAYER1_POT,
    0.05f,    // RELEASE2_POT,
    0.000001f,    // LAYER2_POT,
    0.05f,    // RELEASE1_POT,

    //mux3
    0.000001f,    // INPUT1_CV,
    0.05f,    // LENGTH1_CV,
    0.05f,    // LENGTH2_CV,
    0.05f,    // MUX3_3,
    0.000001f,    // INPUT2_CV,
    0.05f,    // MUX3_5,
    0.05f,    // MUX3_6,
    0.05f    // CRUSH_POT,

    // LAST_MUX
};

//CV (Direct ADC) slew coeffs
float CVSlew[LAST_CV] =
{
    0.002f, //VOct2_CV
    0.002f, //VOct1_CV
    0.000001f, //LAYER1_CV
    0.000001f, //LAYER2_CV
    0.002f, //START1_CV
    0.002f //START2_CV
};

CntrlFreq MuxFreq[LAST_MUX]
{
    //mux1
    Slow,// LENGTH1_POT,   //000
    Slow,// SPEED2_POT, //001
    Slow,// START1_POT, // 010
    Slow,// LENGTH2_POT,  //011
    Slow,// XFADE_POT, //100
    Slow,// PLAY_TOGGLE2_POT, //101
    Slow,// DECAY_POT, //110
    Slow,// PLAY_TOGGLE1_POT, //111

    //mux2
    Slow,// ATTACK1_POT,
    Slow,// START2_POT,
    Slow,// SPEED1_POT,
    Slow,// ATTACK2_POT,
    Slow,// LAYER1_POT,
    Slow,// RELEASE2_POT,
    Slow,// LAYER2_POT,
    Slow,// RELEASE1_POT,

    // //mux3
    Fast,// INPUT1_CV,
    Slow,// LENGTH1_CV,
    Slow,// LENGTH2_CV,
    Slow,// MUX3_3,
    Fast,// INPUT2_CV,
    Slow,// MUX3_5,
    Slow,// MUX3_6,
    Slow,// CRUSH_POT,

    // LAST_MUX


};

CntrlFreq CVFreq[LAST_CV]
{
    Fast, //VOct2_CV
    Fast, //VOct1_CV
    Slow, //LAYER1_CV
    Slow, //LAYER2_CV
    Slow, //START1_CV
    Slow //START2_CV
};

struct CV_Calibration
{
    std::array<float,LAST_CV> CV_Offsets {};
    std::array<float,LAST_CV> CV_Scale {};


    std::array<float, LAST_MUX> MuxOffsets {};
    std::array<float, LAST_MUX> MuxScale {};

    std::array<float, 2> AudioOffsets{};

    //VOct2_CV 0
    //VOct1_CV 1
    //LAYER1_CV 2
    //LAYER2_CV 3
    //START1_CV 4
    //START2_CV 5

    // INPUT1_CV,
    // LENGTH1_CV,
    // LENGTH2_CV,
    // MUX3_3,
    // INPUT2_CV,
    // MUX3_5,
    // MUX3_6,
    // CRUSH_POT,
};

CV_Calibration calibration1

//CV offset
{{  0.020568f,    //VOct2_CV 0
    0.026275f,    //VOct1_CV 1
    0.010223f,    //LAYER1_CV 2
    0.016174f,    //LAYER2_CV 3
    0.026702f,    //START1_CV 4
    0.022521f,    //START2_CV 5
},

//CV scale
{   0.983526f,    //VOct2_CV 0
    0.975134f,    //VOct1_CV 1
    1.0f,    //LAYER1_CV 2
    1.0f,    //LAYER2_CV 3
    1.0f,    //START1_CV 4
    1.0f,    //START2_CV 5
},
//MUX offset
{
0.0f,    // LENGTH1_POT,   //000
0.0f,    // SPEED2_POT, //001
0.0f,    // START1_POT, // 010
0.0f,    // LENGTH2_POT,  //011
0.0f,    // XFADE_POT, //100
0.0f,    // PLAY_TOGGLE2_POT, //101
0.0f,    // DECAY_POT, //110
0.0f,    // PLAY_TOGGLE1_POT, //111

//mux2
0.0f,    // ATTACK1_POT,
0.0f,    // START2_POT,
0.0f,    // SPEED1_POT,
0.0f,    // ATTACK2_POT,
0.0f,    // LAYER1_POT,
0.0f,    // RELEASE2_POT,
0.0f,    // LAYER2_POT,
0.0f,    // RELEASE1_POT,

//mux3
0.021423f, // INPUT1_CV,
0.020058f, // LENGTH1_CV,
0.024497f, // LENGTH2_CV,
0.0f, // MUX3_3,
0.023590f, // INPUT2_CV,
0.0f, // MUX3_5,
0.0f, // MUX3_6,
0.0f // CRUSH_POT,
},

//MUX scale
{
    1.033058f,    // LENGTH1_POT,   //000
    1.033058f,    // SPEED2_POT, //001
    1.033058f,    // START1_POT, // 010
    1.033058f,    // LENGTH2_POT,  //011
    1.033058f,    // XFADE_POT, //100
    1.033058f,    // PLAY_TOGGLE2_POT, //101
    1.033058f,    // DECAY_POT, //110
    1.033058f,    // PLAY_TOGGLE1_POT, //111
    
    //mux2
    1.033058f,    // ATTACK1_POT,
    1.033058f,    // START2_POT,
    1.033058f,    // SPEED1_POT,
    1.033058f,    // ATTACK2_POT,
    1.033058f,    // LAYER1_POT,
    1.033058f,    // RELEASE2_POT,
    1.033058f,    // LAYER2_POT,
    1.033058f,    // RELEASE1_POT,
    
    //mux3
    1.0f, // INPUT1_CV,
    1.0f, // LENGTH1_CV,
    1.0f, // LENGTH2_CV,
    1.0f, // MUX3_3,
    1.0f, // INPUT2_CV,
    1.0f, // MUX3_5,
    1.0f, // MUX3_6,
    1.0f // CRUSH_POT,
},

//Audio Offsets
{
    0.00648115f, //audio output 1
    0.008065646f //audio output 2
}



};    

//test results 1
// MUX Inputs:
// MUX_1:  0.206539	MUX_2:  0.457402	MUX_3:  0.000000	MUX_4:  0.967815
// MUX_5:  0.435149	MUX_6:  0.000011	MUX_7:  0.498974	MUX_8:  0.000009
// MUX_9:  0.215240	MUX_10:  0.000000	MUX_11:  0.454380	MUX_12:  0.000026
// MUX_13:  0.000000	MUX_14:  0.294754	MUX_15:  0.000000	MUX_16:  0.348115
// MUX_17: -0.021423	MUX_18: -0.020058	MUX_19: -0.024497	MUX_20:  0.000000
// MUX_21: -0.023590	MUX_22:  0.000000	MUX_23:  0.000000	MUX_24:  0.967864
// ######################
// Direct CV Inputs:
// CV_1: -0.020568	CV_2: -0.026275	CV_3: -0.010223
// CV_4: -0.016174	CV_5: -0.026702	CV_6: -0.022521

CV_Calibration calibration2

//CV offset
{{  -0.028441f,    //VOct2_CV 0
    -0.026611f,    //VOct1_CV 1
    -0.020721f,    //LAYER1_CV 2
    -0.031860f,    //LAYER2_CV 3
    -0.020812f,    //START1_CV 4
    -0.023498f,    //START2_CV 5
},

//CV scale
{   1.0f,    //VOct2_CV 0
    1.0f,    //VOct1_CV 1
    1.0f,    //LAYER1_CV 2
    1.0f,    //LAYER2_CV 3
    1.0f,    //START1_CV 4
    1.0f,    //START2_CV 5
},
//MUX offset
{
0.0f,    // LENGTH1_POT,   //000
0.0f,    // SPEED2_POT, //001
0.0f,    // START1_POT, // 010
0.0f,    // LENGTH2_POT,  //011
0.0f,    // XFADE_POT, //100
0.0f,    // PLAY_TOGGLE2_POT, //101
0.0f,    // DECAY_POT, //110
0.0f,    // PLAY_TOGGLE1_POT, //111

//mux2
0.0f,    // ATTACK1_POT,
0.0f,    // START2_POT,
0.0f,    // SPEED1_POT,
0.0f,    // ATTACK2_POT,
0.0f,    // LAYER1_POT,
0.0f,    // RELEASE2_POT,
0.0f,    // LAYER2_POT,
0.0f,    // RELEASE1_POT,

//mux3
-0.026123f, // INPUT1_CV,
-0.018890f, // LENGTH1_CV,
-0.026654f, // LENGTH2_CV,
0.0f, // MUX3_3,
-0.032287f, // INPUT2_CV,
0.0f, // MUX3_5,
0.0f, // MUX3_6,
0.0f // CRUSH_POT,
},

//MUX scale
{
    1.033058f,    // LENGTH1_POT,   //000
    1.033058f,    // SPEED2_POT, //001
    1.033058f,    // START1_POT, // 010
    1.033058f,    // LENGTH2_POT,  //011
    1.033058f,    // XFADE_POT, //100
    1.033058f,    // PLAY_TOGGLE2_POT, //101
    1.033058f,    // DECAY_POT, //110
    1.033058f,    // PLAY_TOGGLE1_POT, //111
    
    //mux2
    1.033058f,    // ATTACK1_POT,
    1.033058f,    // START2_POT,
    1.033058f,    // SPEED1_POT,
    1.033058f,    // ATTACK2_POT,
    1.033058f,    // LAYER1_POT,
    1.033058f,    // RELEASE2_POT,
    1.033058f,    // LAYER2_POT,
    1.033058f,    // RELEASE1_POT,
    
    //mux3
    1.0f, // INPUT1_CV,
    1.0f, // LENGTH1_CV,
    1.0f, // LENGTH2_CV,
    1.0f, // MUX3_3,
    1.0f, // INPUT2_CV,
    1.0f, // MUX3_5,
    1.0f, // MUX3_6,
    1.0f // CRUSH_POT,
},

//Audio Offsets
{
    0.0484344f, //audio output 1
    0.0604635f //audio output 2
}

};    

//test results 2
// MUX Inputs:
// MUX_1:  0.121813	MUX_2:  0.968175	MUX_3:  0.968104	MUX_4:  0.968175
// MUX_5:  0.968143	MUX_6:  0.000114	MUX_7:  0.968093	MUX_8:  0.000102
// MUX_9:  0.967985	MUX_10:  0.968000	MUX_11:  0.967984	MUX_12:  0.967996
// MUX_13:  0.967910	MUX_14:  0.968005	MUX_15:  0.967926	MUX_16:  0.967954
// MUX_17:  0.026123	MUX_18:  0.018890	MUX_19:  0.026654	MUX_20:  0.000000
// MUX_21:  0.032287	MUX_22:  0.000000	MUX_23:  0.000000	MUX_24:  0.967965
// ######################
// Direct CV Inputs:
// CV_1:  0.028473	CV_2:  0.026733	CV_3:  0.021026
// CV_4:  0.032135	CV_5:  0.020935	CV_6:  0.023712



VenoLooper_v5() {}
~VenoLooper_v5() {}

void Init(bool boost = false);

    /** 
    Wait some ms before going on.
    \param del Delay time in ms.
    */
    void DelayMs(size_t del);

 /** Starts the callback
    \param cb Interleaved callback function
    */
    void StartAudio(AudioHandle::InterleavingAudioCallback cb);

    /** Starts the callback
    \param cb multichannel callback function
    */
    void StartAudio(AudioHandle::AudioCallback cb);

    /** Stops the audio if it is running. */
    void StopAudio();

    /**
       Switch callback functions
       \param cb New interleaved callback function.
    */
    void ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb);

    /**
       Switch callback functions
       \param cb New multichannel callback function.
    */
    void ChangeAudioCallback(AudioHandle::AudioCallback cb);

    /** Updates the Audio Sample Rate, and reinitializes.
     ** Audio must be stopped for this to work.
     */
    void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);

    /** Returns the audio sample rate in Hz as a floating point number.
     */
    float AudioSampleRate();

    /** Sets the number of samples processed per channel by the audio callback.
     */
    void SetAudioBlockSize(size_t blocksize);

    /** Returns the number of samples per channel in a block of audio. */
    size_t AudioBlockSize();

    /** Returns the rate in Hz that the Audio callback is called */
    float AudioCallbackRate();

    /** Starts Transfering data from the ADC */
    void StartAdc();

    /** Stops Transfering data from the ADC */
    void StopAdc();

    void InitPicoUart(uint8_t* rx_buff);

    /** Processes the ADC inputs, updating their values */
    //CntrlFreq fast should be called per sample, 
    //slow should be called every callback
    void ProcessAnalogControls(CntrlFreq freq);

    /** Process tactile switches and keyboard states */
    void ProcessGates();

    //void ProcessMCP23017();
    void DebouncePicoButtons();
    void UpdatePicoGates();
    void UpdateDaisyGates();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls(Fast);
        ProcessAnalogControls(Slow);
        //ProcessGates();
        //ProcessMCP23017();
    };

    /** Returns the knob's value
        \param idx The knob of interest.
    */
    float GetMuxValue(MUX_IDs idx);

    /** Returns the CV input's value
        \param idx The CV input of interest.
    */
    float GetCvValue(CV_IDs idx);

    //returns state of digital input pin on MCP23017
    //bool GetRawPinState(PICO_Inputs idx);

     /** Getter for knob objects
        \param idx The knob input of interest.
    */
    AnalogControl* GetKnob(size_t idx);

    /** Getter for CV objects.
        \param idx The CV input of interest.
    */
    AnalogControl* GetCv(size_t idx);

    //fills active buffer
    //swaps active and ready buffers
    //for transmission to Pico. Call in audiocallback
    void UpdateLEDs();
 
    //Blocking UART transmit for bool array 
    //for LED states. Call in main() (polling)
    void TransmitLED_States();

    //sets the RGB values for a single neopixel
    void SetNeoPixel(uint8_t id, uint8_t Red, uint8_t Green, uint8_t Blue);

    void SetLayerLED(uint8_t channel, uint8_t layer, uint8_t Red, uint8_t Green, uint8_t Blue);

    void SetRingLED(uint8_t channel, uint8_t position, uint8_t Red, uint8_t Green, uint8_t Blue);

    void SetBankLED(uint8_t slot, uint8_t Red, uint8_t Green, uint8_t Blue);

    void ClearAllNeoPixels();

    void ClearAllButtonLEDs();

    //sets a single button led state (on or off)
    void SetButtonLED(PICO_Button_LEDs id, bool state);

    //UartButton Debounce()

    //Gate output objects and functions

    /** Print formatted debug log message
     */
    template <typename... VA>
    static void Print(const char* format, VA... va)
    {
        Log::Print(format, va...);
    }

    /** Print formatted debug log message with automatic line termination
    */
    template <typename... VA>
    static void PrintLine(const char* format, VA... va)
    {
        Log::PrintLine(format, va...);
    }

    /** Start the logging session. Optionally wait for terminal connection before proceeding.
    */
    static void StartLog(bool wait_for_pc = false)
    {
        Log::StartLog(wait_for_pc);
    }

    bool CalibrateCVs();

    bool RestoreCalibration();

    DaisySeed                     seed;
    MidiUartHandler               midi;
    PicoUartTXRX                  PicoUart;
    AnalogControl                 MUX_Input[LAST_MUX];
    AnalogControl                 cv[LAST_CV];
    GateIn                        gates[LAST_GATE];
    GPIO                          gateOut[LAST_GATE_OUT];
    Encoder                       encoder;
               
    private:

    //data for LEDs: first 180 bytes are RGB values for all 60 neopixels
    //Last byte is packed bools for 6 Button LEDs
    uint8_t LED_Data_a[(NUM_NEOPIXELS * 3) + 1] {};
    uint8_t LED_Data_b[(NUM_NEOPIXELS * 3) + 1] {};

    // Pointers to active and ready buffers for LEDs
    uint8_t* active_buffer = LED_Data_a;
    uint8_t* ready_buffer = LED_Data_b;

    bool ButtonLED_State[NUM_BUTTON_LEDS] {};
    uint8_t NeoPixelState[NUM_NEOPIXELS * 3] {};

    /** Set all the HID callback rates any time a new callback rate is established */
    void SetHidUpdateRates();
    /** Local shorthand for debug log destination
    */
    using Log = Logger<LOGGER_INTERNAL>;
    //void InitMidi();

    // PersistentStorage<CV_Calibration> SavedCalibration(seed.qspi);
    
};

                     
} // namespace daisy

#endif