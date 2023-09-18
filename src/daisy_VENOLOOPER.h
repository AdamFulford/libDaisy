#pragma once
#ifndef VENO_LOOPER_BOARD_H
#define VENO_LOOPER_BOARD_H /**< & */
#include "daisy_seed.h"

namespace daisy
{

class VenoLooper
{
    public:

    enum LEDs
    {
        //RING1
        RING1_1_RED,
        RING1_1_GREEN,
        RING1_1_BLUE,

        RING1_2_RED,
        RING1_2_GREEN,
        RING1_2_BLUE,

        RING1_3_RED,
        RING1_3_GREEN,
        RING1_3_BLUE,

        RING1_4_RED,
        RING1_4_GREEN,
        RING1_4_BLUE,

        RING1_5_RED,
        RING1_5_GREEN,
        RING1_5_BLUE,

        RING1_6_RED,
        RING1_6_GREEN,
        RING1_6_BLUE,

        RING1_7_RED,
        RING1_7_GREEN,
        RING1_7_BLUE,

        RING1_8_RED,
        RING1_8_GREEN,
        RING1_8_BLUE,

        RING1_9_RED,
        RING1_9_GREEN,
        RING1_9_BLUE,

        RING1_10_RED,
        RING1_10_GREEN,
        RING1_10_BLUE,

        RING1_11_RED,
        RING1_11_GREEN,
        RING1_11_BLUE,

        //RING2
        RING2_1_RED,
        RING2_1_GREEN,
        RING2_1_BLUE,

        RING2_2_RED,
        RING2_2_GREEN,
        RING2_2_BLUE,

        RING2_3_RED,
        RING2_3_GREEN,
        RING2_3_BLUE,

        RING2_4_RED,
        RING2_4_GREEN,
        RING2_4_BLUE,

        RING2_5_RED,
        RING2_5_GREEN,
        RING2_5_BLUE,

        RING2_6_RED,
        RING2_6_GREEN,
        RING2_6_BLUE,

        RING2_7_RED,
        RING2_7_GREEN,
        RING2_7_BLUE,

        RING2_8_RED,
        RING2_8_GREEN,
        RING2_8_BLUE,

        RING2_9_RED,
        RING2_9_GREEN,
        RING2_9_BLUE,

        RING2_10_RED,
        RING2_10_GREEN,
        RING2_10_BLUE,

        RING2_11_RED,
        RING2_11_GREEN,
        RING2_11_BLUE,

        //BANK
        BANK_1_RED,
        BANK_1_GREEN,
        BANK_1_BLUE,

        BANK_2_RED,
        BANK_2_GREEN,
        BANK_2_BLUE,

        BANK_3_RED,
        BANK_3_GREEN,
        BANK_3_BLUE,

        BANK_4_RED,
        BANK_4_GREEN,
        BANK_4_BLUE,

        BANK_5_RED,
        BANK_5_GREEN,
        BANK_5_BLUE,

        BANK_6_RED,
        BANK_6_GREEN,
        BANK_6_BLUE,

        BANK_7_RED,
        BANK_7_GREEN,
        BANK_7_BLUE,

        BANK_8_RED,
        BANK_8_GREEN,
        BANK_8_BLUE,
        
        //LAYER1
        LAYER1_1_RED,
        LAYER1_1_GREEN,
        LAYER1_1_BLUE,

        LAYER1_2_RED,
        LAYER1_2_GREEN,
        LAYER1_2_BLUE,

        LAYER1_3_RED,
        LAYER1_3_GREEN,
        LAYER1_3_BLUE,

        LAYER1_4_RED,
        LAYER1_4_GREEN,
        LAYER1_4_BLUE,

        //LAYER2
        LAYER2_1_RED,
        LAYER2_1_GREEN,
        LAYER2_1_BLUE,

        LAYER2_2_RED,
        LAYER2_2_GREEN,
        LAYER2_2_BLUE,

        LAYER2_3_RED,
        LAYER2_3_GREEN,
        LAYER2_3_BLUE,

        LAYER2_4_RED,
        LAYER2_4_GREEN,
        LAYER2_4_BLUE,

        //BUTTON LEDs
        REC1_BUTTON_LED,
        PLAY1_BUTTON_LED,
        REV1_BUTTON_LED,

        REC2_BUTTON_LED,
        PLAY2_BUTTON_LED,
        REV2_BUTTON_LED,
        
        LAST_LED
    };


    enum MuxChannels
    {
        //V2.0 Hardware
        //mux1
        LENGTH1_POT, //000
        SPEED2_POT, //001
        START1_POT, // 010
        LENGTH2_POT,  //011
        XFADE_POT, //100
        PLAY_TOGGLE2,//101
        OVERDUB_DECAY_POT, //110
        PLAY_TOGGLE1, //111

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
        CV1,
        BLANK2,
        BLANK3,
        BLANK4,
        CV2,
        BLANK6,
        BLANK7,
        IN_GAIN_POT,

        LAST_MUX_CHANNEL
    };

    enum speedControls
    {
        SPEED1_POT_CTRL,
        SPEED2_POT_CTRL,

        SPEED1_CV_CTRL,
        SPEED2_CV_CTRL,

        LAST_SPEED    
    };

    enum MCP23017_Channels
    {
        //V2.0 hardware
        //MCP23017 PortA
        SAVE_BUTTON,
        STOP2_BUTTON,
        LINK_TOGGLE,
        REC2_BUTTON,
        PLAY2_BUTTON,
        REV2_BUTTON,
        CLR2_BUTTON,
        QUANTISE_TOGGLE,

        //MCP23017 PortB
        ENC_CLICK,
        REV1_GATE,
        REV2_GATE,
        REV1_BUTTON,
        STOP1_BUTTON,
        PLAY1_BUTTON,
        CLR1_BUTTON,
        REC1_BUTTON,

        LAST_MCP23017_Channel
    };

    Mcp23017::Type DefaultSwType[16] = 
    {
        //MCP23017 PortA
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        //MCP23017 PortB
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY,
        Mcp23017::Type::TYPE_MOMENTARY

        //perform all latching in program code
    };

    enum CVs
    {
        LENGTH2_CV,
        VOct1_CV,
        VOct2_CV,
        LAYER1_CV,
        LAYER2_CV,
        START1_CV,
        START2_CV,
        LENGTH1_CV,

        LAST_CV
    };

    enum Gates
    {
        PLAY1_GATE,
        REC1_GATE,   
        PLAY2_GATE,
        REC2_GATE,
        CLOCK_GATE,

        LAST_GATE
    };



VenoLooper() {}
~VenoLooper() {}

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

    /** Processes the ADC inputs, updating their values */
    void ProcessAnalogControls();

    /** Process MCP23017 expander*/
    void ProcessMCP23017();

    void DebounceMCP23017();

    void DebounceEnc();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessMCP23017();
    };

    /** Returns the knob's value
        \param idx The knob of interest.
    */
    float GetKnobValue(MuxChannels idx);

    /** Returns the CV input's value
        \param idx The CV input of interest.
    */
    float GetCvValue(CVs idx);

    //returns state of digital input pin on MCP23017
    bool GetRawPinState(MCP23017_Channels idx);

     /** Getter for knob objects
        \param idx The knob input of interest.
    */
    AnalogControl* GetKnob(size_t idx);

    /** Getter for CV objects.
        \param idx The CV input of interest.
    */
    AnalogControl* GetCv(size_t idx);

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
    //MidiUartHandler               midi;
    Mcp23017                      mcp;
    AnalogControl                 pots[LAST_MUX_CHANNEL];
    AnalogControl                 cv[LAST_CV];
    GateIn                        gate_in[LAST_GATE];
    GPIO                          EOC1;
    GPIO                          EOC2;
    Encoder                       encoder;
    

    private:

    /** Set all the HID callback rates any time a new callback rate is established */
    void SetHidUpdateRates();
    /** Local shorthand for debug log destination
    */
    using Log = Logger<LOGGER_INTERNAL>;
    //void InitMidi();
    
};
namespace Veno_Looper //constants for addressing LEDs on VenoLooper hardware
{
constexpr VenoLooper::LEDs Ring1Red[11] = {VenoLooper::RING1_1_RED, VenoLooper::RING1_2_RED, VenoLooper::RING1_3_RED,
                                           VenoLooper::RING1_4_RED, VenoLooper::RING1_5_RED, VenoLooper::RING1_6_RED,
                                           VenoLooper::RING1_7_RED, VenoLooper::RING1_8_RED, VenoLooper::RING1_9_RED,
                                           VenoLooper::RING1_10_RED, VenoLooper::RING1_11_RED};

constexpr VenoLooper::LEDs Ring1Green[11] = {VenoLooper::RING1_1_GREEN, VenoLooper::RING1_2_GREEN, VenoLooper::RING1_3_GREEN,
                                             VenoLooper::RING1_4_GREEN, VenoLooper::RING1_5_GREEN, VenoLooper::RING1_6_GREEN,
                                             VenoLooper::RING1_7_GREEN, VenoLooper::RING1_8_GREEN, VenoLooper::RING1_9_GREEN,
                                             VenoLooper::RING1_10_GREEN, VenoLooper::RING1_11_GREEN};

constexpr VenoLooper::LEDs Ring1Blue[11] = {VenoLooper::RING1_1_BLUE, VenoLooper::RING1_2_BLUE, VenoLooper::RING1_3_BLUE,
                                             VenoLooper::RING1_4_BLUE, VenoLooper::RING1_5_BLUE, VenoLooper::RING1_6_BLUE,
                                             VenoLooper::RING1_7_BLUE, VenoLooper::RING1_8_BLUE, VenoLooper::RING1_9_BLUE,
                                             VenoLooper::RING1_10_BLUE, VenoLooper::RING1_11_BLUE};

constexpr VenoLooper::LEDs Ring2Red[11] = {VenoLooper::RING2_1_RED, VenoLooper::RING2_2_RED, VenoLooper::RING2_3_RED,
                                           VenoLooper::RING2_4_RED, VenoLooper::RING2_5_RED, VenoLooper::RING2_6_RED,
                                           VenoLooper::RING2_7_RED, VenoLooper::RING2_8_RED, VenoLooper::RING2_9_RED,
                                           VenoLooper::RING2_10_RED, VenoLooper::RING2_11_RED};

constexpr VenoLooper::LEDs Ring2Green[11] = {VenoLooper::RING2_1_GREEN, VenoLooper::RING2_2_GREEN, VenoLooper::RING2_3_GREEN,
                                             VenoLooper::RING2_4_GREEN, VenoLooper::RING2_5_GREEN, VenoLooper::RING2_6_GREEN,
                                             VenoLooper::RING2_7_GREEN, VenoLooper::RING2_8_GREEN, VenoLooper::RING2_9_GREEN,
                                             VenoLooper::RING2_10_GREEN, VenoLooper::RING2_11_GREEN};

constexpr VenoLooper::LEDs Ring2Blue[11] = {VenoLooper::RING2_1_BLUE, VenoLooper::RING2_2_BLUE, VenoLooper::RING2_3_BLUE,
                                             VenoLooper::RING2_4_BLUE, VenoLooper::RING2_5_BLUE, VenoLooper::RING2_6_BLUE,
                                             VenoLooper::RING2_7_BLUE, VenoLooper::RING2_8_BLUE, VenoLooper::RING2_9_BLUE,
                                             VenoLooper::RING2_10_BLUE, VenoLooper::RING2_11_BLUE};

constexpr VenoLooper::LEDs Layer1RED[4] = {VenoLooper::LAYER1_1_RED, 
                                            VenoLooper::LAYER1_2_RED, 
                                            VenoLooper::LAYER1_3_RED, 
                                            VenoLooper::LAYER1_4_RED};

constexpr VenoLooper::LEDs Layer1GREEN[4] = {VenoLooper::LAYER1_1_GREEN, 
                                            VenoLooper::LAYER1_2_GREEN, 
                                            VenoLooper::LAYER1_3_GREEN, 
                                            VenoLooper::LAYER1_4_GREEN};

constexpr VenoLooper::LEDs Layer1BLUE[4] = {VenoLooper::LAYER1_1_BLUE, 
                                            VenoLooper::LAYER1_2_BLUE, 
                                            VenoLooper::LAYER1_3_BLUE, 
                                            VenoLooper::LAYER1_4_BLUE}; 

constexpr VenoLooper::LEDs Layer2RED[4] = {VenoLooper::LAYER2_1_RED, 
                                            VenoLooper::LAYER2_2_RED, 
                                            VenoLooper::LAYER2_3_RED, 
                                            VenoLooper::LAYER2_4_RED};

constexpr VenoLooper::LEDs Layer2GREEN[4] = {VenoLooper::LAYER2_1_GREEN, 
                                            VenoLooper::LAYER2_2_GREEN, 
                                            VenoLooper::LAYER2_3_GREEN, 
                                            VenoLooper::LAYER2_4_GREEN};

constexpr VenoLooper::LEDs Layer2BLUE[4] = {VenoLooper::LAYER2_1_BLUE, 
                                            VenoLooper::LAYER2_2_BLUE, 
                                            VenoLooper::LAYER2_3_BLUE, 
                                            VenoLooper::LAYER2_4_BLUE}; 

constexpr VenoLooper::LEDs BankRed[10] = {VenoLooper::RING1_1_RED, VenoLooper::RING1_2_RED, VenoLooper::RING1_3_RED,
                                           VenoLooper::RING1_4_RED, VenoLooper::RING1_5_RED, VenoLooper::RING1_6_RED,
                                           VenoLooper::RING1_7_RED, VenoLooper::RING1_8_RED, VenoLooper::RING1_9_RED,
                                           VenoLooper::RING1_10_RED};

constexpr VenoLooper::LEDs BankGreen[10] = {VenoLooper::RING1_1_GREEN, VenoLooper::RING1_2_GREEN, VenoLooper::RING1_3_GREEN,
                                             VenoLooper::RING1_4_GREEN, VenoLooper::RING1_5_GREEN, VenoLooper::RING1_6_GREEN,
                                             VenoLooper::RING1_7_GREEN, VenoLooper::RING1_8_GREEN, VenoLooper::RING1_9_GREEN,
                                             VenoLooper::RING1_10_GREEN};

constexpr VenoLooper::LEDs BankBlue[10] = {VenoLooper::RING1_1_BLUE, VenoLooper::RING1_2_BLUE, VenoLooper::RING1_3_BLUE,
                                             VenoLooper::RING1_4_BLUE, VenoLooper::RING1_5_BLUE, VenoLooper::RING1_6_BLUE,
                                             VenoLooper::RING1_7_BLUE, VenoLooper::RING1_8_BLUE, VenoLooper::RING1_9_BLUE,
                                             VenoLooper::RING1_10_BLUE};

//V2.0 Hardware
constexpr uint8_t LED_Coords[120][2] = //{CSXX,SWXX}
{ 
{1,1},// RING1_1_RED,
{1,2},// RING1_1_GREEN,
{1,3},// RING1_1_BLUE,

{1,4},// RING1_2_RED,
{1,5},// RING1_2_GREEN,
{1,6},// RING1_2_BLUE,

{1,7},// RING1_3_RED,
{1,8},// RING1_3_GREEN,
{1,9},// RING1_3_BLUE,

{1,10},// RING1_4_RED,
{1,11},// RING1_4_GREEN,
{1,12},// RING1_4_BLUE,

{2,1},// RING1_5_RED,
{2,2},// RING1_5_GREEN,
{2,3},// RING1_5_BLUE,

{2,4},// RING1_6_RED,
{2,5},// RING1_6_GREEN,
{2,6},// RING1_6_BLUE,

{2,7},// RING1_7_RED,
{2,8},// RING1_7_GREEN,
{2,9},// RING1_7_BLUE,

{2,10},// RING1_8_RED,
{2,11},// RING1_8_GREEN,
{2,12},// RING1_8_BLUE,

{3,1},// RING1_9_RED,
{3,2},// RING1_9_GREEN,
{3,3},// RING1_9_BLUE,

{3,4},// RING1_10_RED,
{3,5},// RING1_10_GREEN,
{3,6},// RING1_10_BLUE,

{3,7},// RING1_11_RED,
{3,8},// RING1_11_GREEN,
{3,9},// RING1_11_BLUE,

{3,10},// RING2_1_RED,
{3,11},// RING2_1_GREEN,
{3,12},// RING2_1_BLUE,

{4,1},// RING2_2_RED,
{4,2},// RING2_2_GREEN,
{4,3},// RING2_2_BLUE,

{4,4},// RING2_3_RED,
{4,5},// RING2_3_GREEN,
{4,6},// RING2_3_BLUE,

{4,7},// RING2_4_RED,
{4,8},// RING2_4_GREEN,
{4,9},// RING2_4_BLUE,

{4,10},// RING2_5_RED,
{4,11},// RING2_5_GREEN,
{4,12},// RING2_5_BLUE,

{5,1},// RING2_6_RED,
{5,2},// RING2_6_GREEN,
{5,3},// RING2_6_BLUE,

{5,4},// RING2_7_RED,
{5,5},// RING2_7_GREEN,
{5,6},// RING2_7_BLUE,

{5,7},// RING2_8_RED,
{5,8},// RING2_8_GREEN,
{5,9},// RING2_8_BLUE,

{5,10},// RING2_9_RED,
{5,11},// RING2_9_GREEN,
{5,12},// RING2_9_BLUE,

{6,1},// RING2_10_RED,
{6,2},// RING2_10_GREEN,
{6,3},// RING2_10_BLUE,

{6,4},// RING2_11_RED,
{6,5},// RING2_11_GREEN,
{6,6},// RING2_11_BLUE,

{7,4},// BANK_1_RED,
{7,5},// BANK_1_GREEN,
{7,6},// BANK_1_BLUE,

{7,1},// BANK_2_RED,
{7,2},// BANK_2_GREEN,
{7,3},// BANK_2_BLUE,

{6,10},// BANK_3_RED,
{6,11},// BANK_3_GREEN,
{6,12},// BANK_3_BLUE,

{6,7},// BANK_4_RED,
{6,8},// BANK_4_GREEN,
{6,9},// BANK_4_BLUE,

{8,4},// BANK_5_RED,
{8,5},// BANK_5_GREEN,
{8,6},// BANK_5_BLUE,

{8,1},// BANK_6_RED,
{8,2},// BANK_6_GREEN,
{8,3},// BANK_6_BLUE,

{7,10},// BANK_7_RED,
{7,11},// BANK_7_GREEN,
{7,12},// BANK_7_BLUE,

{7,7},// BANK_8_RED,
{7,8},// BANK_8_GREEN,
{7,9},// BANK_8_BLUE,

{8,7},// LAYER1_1_RED,
{8,8},// LAYER1_1_GREEN,
{8,9},// LAYER1_1_BLUE,

{8,10},// LAYER1_2_RED,
{8,11},// LAYER1_2_GREEN,
{8,12},// LAYER1_2_BLUE,

{9,1},// LAYER1_3_RED,
{9,2},// LAYER1_3_GREEN,
{9,3},// LAYER1_3_BLUE,

{9,4},// LAYER1_4_RED,
{9,5},// LAYER1_4_GREEN,
{9,6},// LAYER1_4_BLUE,

// //LAYER2
{9,7},// LAYER2_1_RED,
{9,8},// LAYER2_1_GREEN,
{9,9},// LAYER2_1_BLUE,

{9,10},// LAYER2_2_RED,
{9,11},// LAYER2_2_GREEN,
{9,12},// LAYER2_2_BLUE,

{10,1},// LAYER2_3_RED,
{10,2},// LAYER2_3_GREEN,
{10,3},// LAYER2_3_BLUE,

{10,4},// LAYER2_4_RED,
{10,5},// LAYER2_4_GREEN,
{10,6},// LAYER2_4_BLUE,

// //BUTTON LEDs
{10,7},// REC1_BUTTON_LED,
{10,8},// PLAY1_BUTTON_LED,
{10,9},// REV1_BUTTON_LED,

{10,10},// REC2_BUTTON_LED,
{10,11},// PLAY2_BUTTON_LED,
{10,12},// REV2_BUTTON_LED,


};

} //namespace VenoLooper                 
} // namespace daisy

#endif