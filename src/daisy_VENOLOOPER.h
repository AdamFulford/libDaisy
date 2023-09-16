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
        //PCA9685 address 0x00
        REV_BUTTON1_LED, //0
        PLAY_BUTTON1_LED, //1
        REC_BUTTON1_LED, //2
        Ring1White5_LED, //3
        Ring1Red4_LED, //4
        Ring1White4_LED, //5
        Ring1Red3_LED, //6
        Ring1White3_LED, //7
        Ch1Layer4_LED, //8
        Ch1Layer3_LED, //9
        Ch1Layer2_LED, //10
        Ch1Layer1_LED, //11
        Ring1Red2_LED, //12
        Ring1White2_LED, //13
        Ring1White1_LED, //14
        Ring1Red1_LED, //15
        
        //PCA9685 address 0x01
        EOC1_LED,
        EOC2_LED,
        Ch2Layer1_LED,
        Ch2Layer2_LED,
        Ch2Layer3_LED,
        Ch2Layer4_LED,
        Ring2White5_LED,
        Ring2Red5_LED,
        Ring2Red8_LED,
        Ring2White8_LED,
        Ring2Red7_LED,
        Ring2White7_LED,
        Ring2Red6_LED,
        Ring2White6_LED,
        Ring2White9_LED,
        Ring2Red9_LED,

        //PCA9685 address 0x02
        Ring2White2_LED,
        Ring2Red2_LED,
        Ring2Red4_LED,
        Ring2White4_LED,
        Ring2Red3_LED,
        Ring2White3_LED,
        REC_BUTTON2_LED,
        PLAY_BUTTON2_LED,
        REV_BUTTON2_LED,
        Ring2White1_LED,
        Ring2Red1_LED,
        SevenSeg_E_LED,
        SevenSeg_D_LED,
        SevenSeg_Dig2_LED,
        SevenSeg_DP_LED,
        SevenSeg_C_LED,

        //PCA9685 address 0x03
        Ring1Red8_LED,
        SevenSeg_Dig1_LED,
        NC1_LED,
        NC2_LED,
        SevenSeg_A_LED,
        SevenSeg_B_LED,
        SevenSeg_G_LED,
        SevenSeg_F_LED,
        Ring1Red9_LED,
        Ring1White9_LED,
        Ring1White8_LED,
        Ring1Red7_LED,
        Ring1White7_LED,
        Ring1Red6_LED,
        Ring1White6_LED,
        Ring1Red5_LED,

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
        BLANK,
        BLANK,
        BLANK,
        CV2,
        BLANK,
        BLANK,
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

    /** Process tactile switches and keyboard states */
    void ProcessGates();

    /** Process MCP23017 expander*/
    void ProcessMCP23017();

    void DebounceMCP23017();

    void DebounceEnc();

    /** Process Analog and Digital Controls */
    inline void ProcessAllControls()
    {
        ProcessAnalogControls();
        ProcessGates();
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

    void Set7Segment(bool digit, uint8_t value, float DP);

    DaisySeed                     seed;
    LedDriverPca9685<4, true>     led_driver;
    MidiUartHandler               midi;
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
constexpr VenoLooper::LEDs Ring1White[9] = {VenoLooper::Ring1White1_LED, VenoLooper::Ring1White2_LED, VenoLooper::Ring1White3_LED,
                        VenoLooper::Ring1White4_LED, VenoLooper::Ring1White5_LED, VenoLooper::Ring1White6_LED,
                        VenoLooper::Ring1White7_LED, VenoLooper::Ring1White8_LED, VenoLooper::Ring1White9_LED};

constexpr VenoLooper::LEDs Ring2White[9] = {VenoLooper::Ring2White1_LED, VenoLooper::Ring2White2_LED, VenoLooper::Ring2White3_LED,
                        VenoLooper::Ring2White4_LED, VenoLooper::Ring2White5_LED, VenoLooper::Ring2White6_LED,
                        VenoLooper::Ring2White7_LED, VenoLooper::Ring2White8_LED, VenoLooper::Ring2White9_LED};

constexpr VenoLooper::LEDs Ring1Red[9] = {VenoLooper::Ring1Red1_LED, VenoLooper::Ring1Red2_LED, VenoLooper::Ring1Red3_LED,
                        VenoLooper::Ring1Red4_LED, VenoLooper::Ring1Red5_LED, VenoLooper::Ring1Red6_LED,
                        VenoLooper::Ring1Red7_LED, VenoLooper::Ring1Red8_LED, VenoLooper::Ring1Red9_LED};

constexpr VenoLooper::LEDs Ring2Red[9] = {VenoLooper::Ring2Red1_LED, VenoLooper::Ring2Red2_LED, VenoLooper::Ring2Red3_LED,
                        VenoLooper::Ring2Red4_LED, VenoLooper::Ring2Red5_LED, VenoLooper::Ring2Red6_LED,
                        VenoLooper::Ring2Red7_LED, VenoLooper::Ring2Red8_LED, VenoLooper::Ring2Red9_LED};   


constexpr VenoLooper::LEDs Layer1Leds[4] = {VenoLooper::Ch1Layer1_LED, 
                                            VenoLooper::Ch1Layer2_LED, 
                                            VenoLooper::Ch1Layer3_LED, 
                                            VenoLooper::Ch1Layer4_LED};
constexpr VenoLooper::LEDs Layer2Leds[4] = {VenoLooper::Ch2Layer1_LED, 
                                            VenoLooper::Ch2Layer2_LED, 
                                            VenoLooper::Ch2Layer3_LED, 
                                            VenoLooper::Ch2Layer4_LED};

constexpr VenoLooper::LEDs SevenSeg[8] = {VenoLooper::SevenSeg_A_LED,
                                         VenoLooper::SevenSeg_B_LED,
                                         VenoLooper::SevenSeg_C_LED,
                                         VenoLooper::SevenSeg_D_LED,
                                         VenoLooper::SevenSeg_E_LED,
                                         VenoLooper::SevenSeg_F_LED,
                                         VenoLooper::SevenSeg_G_LED,
                                         VenoLooper::SevenSeg_DP_LED};

constexpr bool Numbers[10][7] = {{1,1,1,1,1,1,0}, //0
                                 {0,1,1,0,0,0,0}, //1
                                 {1,1,0,1,1,0,1}, //2
                                 {1,1,1,1,0,0,1}, //3
                                 {0,1,1,0,0,1,1}, //4
                                 {1,0,1,1,0,1,1}, //5
                                 {1,0,1,1,1,1,1}, //6
                                 {1,1,1,0,0,0,0}, //7
                                 {1,1,1,1,1,1,1}, //8
                                 {1,1,1,1,0,1,1}}; //9
} //namespace VenoLooper                 
} // namespace daisy

#endif