#pragma once
#ifndef VENO_LOOPER_V5_BOARD_H
#define VENO_LOOPER_V5_BOARD_H /**< & */
#include "daisy_seed.h"

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

#define BAUD_RATE 115200
#define BLOCK_SIZE 256

namespace daisy
{

class VenoLooper_v5
{
    public:

    enum NeoPixel_LEDs //enum for addressing NeoPixels
    {
        LAST_NP_LED
    };

    enum Button_LEDs //enum for addressing button LEDs
    {
        LAST_BUTTON_LED
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

    enum PICO_Inputs
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

        LINK_SWITCH,
        QUANTISE_SWITCH,
        SAVE_BUTTON,
        ENC_CLICK,

        REV1_GATE,
        REV2_GATE,

        LAST_PICO_INPUT    
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
        REC1_GATE,
        PLAY2_GATE,
        REC2_GATE,
        CLOCK_GATE,

        LAST_GATE
    };

    enum CntrlFreq
    {Fast,Slow};

 
//mux input slew coeffs
float MuxSlew[LAST_MUX] =
{
    //mux1
    0.05f,    // LENGTH1_POT,   //000
    0.2f,    // SPEED2_POT, //001
    0.05f,    // START1_POT, // 010
    0.05f,    // LENGTH2_POT,  //011
    0.05f,    // XFADE_POT, //100
    0.05f,    // PLAY_TOGGLE2_POT, //101
    0.05f,    // DECAY_POT, //110
    0.05f,    // PLAY_TOGGLE1_POT, //111

    //mux2
    0.05f,    // ATTACK1_POT,
    0.05f,    // START2_POT,
    0.2f,    // SPEED1_POT,
    0.05f,    // ATTACK2_POT,
    0.000001f,    // LAYER1_POT,
    0.05f,    // RELEASE2_POT,
    0.000001f,    // LAYER2_POT,
    0.05f,    // RELEASE1_POT,

    //mux3
    0.05f,    // INPUT1_CV,
    0.05f,    // LENGTH1_CV,
    0.05f,    // LENGTH2_CV,
    0.05f,    // MUX3_3,
    0.05f,    // INPUT2_CV,
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
    Fast,// SPEED2_POT, //001
    Slow,// START1_POT, // 010
    Slow,// LENGTH2_POT,  //011
    Slow,// XFADE_POT, //100
    Slow,// PLAY_TOGGLE2_POT, //101
    Slow,// DECAY_POT, //110
    Slow,// PLAY_TOGGLE1_POT, //111

    //mux2
    Slow,// ATTACK1_POT,
    Slow,// START2_POT,
    Fast,// SPEED1_POT,
    Slow,// ATTACK2_POT,
    Slow,// LAYER1_POT,
    Slow,// RELEASE2_POT,
    Slow,// LAYER2_POT,
    Slow,// RELEASE1_POT,

    // //mux3
    Slow,// INPUT1_CV,
    Slow,// LENGTH1_CV,
    Slow,// LENGTH2_CV,
    Slow,// MUX3_3,
    Slow,// INPUT2_CV,
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

    /** Process MCP23017 expander*/
    //void ProcessMCP23017();

    //void DebounceMCP23017();

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
    bool GetRawPinState(PICO_Inputs idx);

     /** Getter for knob objects
        \param idx The knob input of interest.
    */
    AnalogControl* GetKnob(size_t idx);

    /** Getter for CV objects.
        \param idx The CV input of interest.
    */
    AnalogControl* GetCv(size_t idx);

    // ToDo: 
    // helper functions for LED setting (Layer, Ring, Bank, Buttons)
    // memory allocation for active and ready LED buffers
    // UpdateLEDs() (swaps the active and ready buffers)

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

    DaisySeed                     seed;
    //LedDriverPca9685<4, true>     led_driver;
    MidiUartHandler               midi;
    PicoUartTXRX                  PicoUart;
    //Mcp23017                      mcp;
    AnalogControl                 MUX_Input[LAST_MUX];
    AnalogControl                 cv[LAST_CV];
    GateIn                        gates[LAST_GATE];
    //Switch                        gates[LAST_GATE];                    

    private:

    std::array<bool, 6> ButtonLEDStates_a {};
    std::array<bool, 6> ButtonLEDStates_b {};

    // Pointers to active and ready buffers
    std::array<bool, 6>* active_buffer = &ButtonLEDStates_a;
    std::array<bool, 6>* ready_buffer = &ButtonLEDStates_b;



    /** Set all the HID callback rates any time a new callback rate is established */
    void SetHidUpdateRates();
    /** Local shorthand for debug log destination
    */
    using Log = Logger<LOGGER_INTERNAL>;
    //void InitMidi();
    
};

                     
} // namespace daisy

#endif