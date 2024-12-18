#pragma once
#ifndef VENO_LOOPER_V5_BOARD_H
#define VENO_LOOPER_V5_BOARD_H /**< & */
#include "daisy_seed.h"

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



    enum MUX_Inputs
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

        LAST_POT
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


    enum CVs
    {   
        LENGTH2_CV,
        VOct2_CV,
        VOct1_CV,
        LAYER1_CV,
        LAYER2_CV,
        START1_CV,
        START2_CV,
        LENGTH1_CV,
        
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

    //pot param slew

float PotSlew[LAST_POT] =
{
    0.05f, //XFADE_POT,   //000
    0.05f, //PLAY_TOGGLE1_POT
    0.2f, //SPEED1_POT
    0.05f, //ATTACK1_POT
    0.05f, //START1_POT
    0.05f, //OVERDUB_DECAY_POT
    0.000001f, //LAYER1_POT
    0.05f, //LENGTH1_POT
    0.05f, //LENGTH2_POT
    0.000001f, //LAYER2_POT
    0.05f, //START2_POT
    0.05f, //RELEASE2_POT
    0.2f, //SPEED2_POT
    0.05f, //PLAY_TOGGLE2_POT
    0.05f, //RELEASE1_POT
    0.05f //ATTACK2_POT
};

float CVSlew[LAST_CV] =
{
    0.002f, //LENGTH2_CV
    0.002f, //VOct2_CV
    0.002f, //VOct1_CV
    0.000001f, //LAYER1_CV
    0.000001f, //LAYER2_CV
    0.002f, //START1_CV
    0.002f, //START2_CV
    0.002f //LENGTH1_CV
};

CntrlFreq PotFreq[LAST_POT]
{
    Slow, //XFADE_POT,   //000
    Slow, //PLAY_TOGGLE1_POT
    Fast, //SPEED1_POT
    Slow, //ATTACK1_POT
    Slow, //START1_POT
    Slow, //OVERDUB_DECAY_POT
    Slow, //LAYER1_POT
    Slow, //LENGTH1_POT
    Slow, //LENGTH2_POT
    Slow, //LAYER2_POT
    Slow, //START2_POT
    Slow, //RELEASE2_POT
    Fast, //SPEED2_POT
    Slow, //PLAY_TOGGLE2_POT
    Slow, //RELEASE1_POT
    Slow //ATTACK2_POT
};

CntrlFreq CVFreq[LAST_CV]
{
    Slow, //LENGTH2_CV
    Fast, //VOct2_CV
    Fast, //VOct1_CV
    Slow, //LAYER1_CV
    Slow, //LAYER2_CV
    Slow, //START1_CV
    Slow, //START2_CV
    Slow //LENGTH1_CV
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

    /** Processes the ADC inputs, updating their values */
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
    float GetKnobValue(MUX_Inputs idx);

    /** Returns the CV input's value
        \param idx The CV input of interest.
    */
    float GetCvValue(CVs idx);

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
    AnalogControl                 pots[LAST_POT];
    AnalogControl                 cv[LAST_CV];
    GateIn                        gates[LAST_GATE];
    //Switch                        gates[LAST_GATE];                    

    private:

    /** Set all the HID callback rates any time a new callback rate is established */
    void SetHidUpdateRates();
    /** Local shorthand for debug log destination
    */
    using Log = Logger<LOGGER_INTERNAL>;
    //void InitMidi();
    
};

                     
} // namespace daisy

#endif