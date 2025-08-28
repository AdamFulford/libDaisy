#pragma once
#ifndef VENO_ORBIT_V1_0_BOARD_H
#define VENO_ORBIT_V1_0_BOARD_H /**< & */
#include <map>
#include "daisy_seed.h"

//Veno-Orbit v1.0 pin assignments
#define PIN_EOC1 D30
#define PIN_EOC2 D29

#define PIN_ENC_DOWN D8
#define PIN_MIDI_UART_RX D14
#define PIN_MIDI_UART_TX D13
#define PIN_PLAY2_GATE D27
#define PIN_UART_RP2040_TO_STM D11
#define PIN_UART_STM_TO_RP2040 D12
#define PIN_REC2_GATE D0

#define PIN_MUX1_ADC A12
#define PIN_ENC_UP D10
#define PIN_VOct_CV2_ADC A6
#define PIN_VOct_CV1_ADC A10
#define PIN_LAYER_CV1_ADC A11
#define PIN_LAYER_CV2_ADC A8
#define PIN_START_CV1_ADC A9
#define PIN_START_CV2_ADC A0
#define PIN_MUX3_ADC A7
#define PIN_MUX2_ADC A1

#define PIN_MUX_SEL3 D20
#define PIN_MUX_SEL2 D18
#define PIN_MUX_SEL1 D17

#define PIN_REC1_GATE D7
#define PIN_CLOCK D9
#define PIN_PLAY1_GATE D26

#define PIN_SDMMC_SK D6
#define PIN_SDMMC_CMD D5
#define PIN_SDMMC_D0 D4
#define PIN_SDMMC_D1 D3
#define PIN_SDMMC_D2 D2
#define PIN_SDMMC_D3 D1

#define PIN_CV1 A4
#define PIN_CV2 A13

#define BAUD_RATE 921600 
#define BLOCK_SIZE 128
#define NUM_BUTTON_LEDS 6

#define NUM_NEOPIXELS 60

#define RING1_START 55
#define RING1_END 34

#define RING2_START 25
#define RING2_END 4

#define LAYER1_START 56
#define LAYER2_START 3

#define BANK_START 33

#define NUM_RING_PIXELS 22
#define NUM_BANK_PIXELS 8

#define LED_BRIGHTNESS 0.5
#define BANK_LAYER_BRIGHTNESS 0.25

using namespace daisy;

namespace daisy
{

class VenoOrbit_v1_0
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
    UNDO_BUTTON,
    SAVE_BUTTON,
    ENC_CLICK_BUTTON,
    LINK_SW,

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
        LAYER1_POT, //001
        START1_POT, // 010
        SPEED1_POT,  //011
        SPEED2_POT, //100
        LAYER2_POT, //101
        LENGTH2_POT, //110
        START2_POT, //111

        //mux2
        ATTACK1_POT,
        ATTACK2_POT,
        PLAY_TOGGLE2,
        PLAY_TOGGLE1,
        RELEASE2_POT,
        RELEASE1_POT,
        MUX2_6,
        MUX2_7,

        //mux3
        MUX3_0,
        LENGTH2_CV,
        INPUT_TYPE_TOGGLE,
        LENGTH1_CV,
        IN_LEVEL_POT,
        XFADE_POT,
        DECAY_POT,
        CLOCK_DETECT,

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
        MOD1_CV,
        MOD2_CV,
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

    
    enum Outputs
    {
        AUDIO_L_OUT,
        AUDIO_R_OUT,
        MOD_L_OUT,
        MOD_R_OUT,
        MOD_L_0V_OUT,
        MOD_R_0V_OUT,
        MOD_L_3V_OUT,
        MOD_R_3V_OUT,

        LAST_OUT
    };

//mux input slew coeffs
float MuxSlew[LAST_MUX] =
{
    //mux1
    0.05f,    // LENGTH1_POT,   //000
    0.05f,    // LAYER1_POT, //001
    0.05f,    // START1_POT, // 010
    0.05f,    // SPEED1_POT,  //011
    0.05f,    // SPEED2_POT, //100
    0.05f,    // LAYER2_POT, //101
    0.05f,    // LENGTH2_POT, //110
    0.05f,    // START2_POT, //111

    //mux2
    0.05f,    // ATTACK1_POT,
    0.05f,    // ATTACK2_POT,
    0.05f,    // PLAY_TOGGLE2,
    0.05f,    // PLAY_TOGGLE1,
    0.05f,    // RELEASE2_POT,
    0.05f,    // RELEASE1_POT,
    0.05f,    // MUX2_6,
    0.05f,    // MUX2_7,

    //mux3
    0.05f,    // MUX3_0,
    0.05f,    // LENGTH2_CV,
    0.05f,    // INPUT_TYPE_TOGGLE,
    0.05f,    // LENGTH1_CV,
    0.000001f,   // IN_LEVEL_POT,
    0.05f,    // XFADE_POT,
    0.05f,    // DECAY_POT,
    0.05f    // CLOCK_DETECT,

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
    0.002f, //START2_CV
    0.000001f, //MOD1_CV
    0.000001f, //MOD2_CV
};

CntrlFreq MuxFreq[LAST_MUX]
{
    //mux1
    Slow,// LENGTH1_POT,   //000
    Slow,// LAYER1_POT, //001
    Slow,// START1_POT, // 010
    Slow,// SPEED1_POT,  //011
    Slow,// SPEED2_POT, //100
    Slow,// LAYER2_POT, //101
    Slow,// LENGTH2_POT, //110
    Slow,// START2_POT, //111

    //mux2
    Slow,// ATTACK1_POT,
    Slow,// ATTACK2_POT,
    Slow,// PLAY_TOGGLE2,
    Slow,// PLAY_TOGGLE1,
    Slow,// RELEASE2_POT,
    Slow,// RELEASE1_POT,
    Slow,// MUX2_6,
    Slow,// MUX2_7,

    // //mux3
    Slow,// MUX3_0,
    Slow,// LENGTH2_CV,
    Slow,// INPUT_TYPE_TOGGLE,
    Slow,// LENGTH1_CV,
    Fast,// IN_LEVEL_POT,
    Slow,// XFADE_POT,
    Slow,// DECAY_POT,
    Slow,// CLOCK_DETECT,

    // LAST_MUX


};

CntrlFreq CVFreq[LAST_CV]
{
    Fast, //VOct2_CV
    Fast, //VOct1_CV
    Slow, //LAYER1_CV
    Slow, //LAYER2_CV
    Slow, //START1_CV
    Slow, //START2_CV
    Fast, //MOD1_CV
    Fast //MOD2_CV
};

struct CV_Calibration
{
    public: 
    
    std::array<float,LAST_CV> CV_Offsets {};
    std::array<float,LAST_CV> CV_Scale {};

    std::array<float, LAST_MUX> MuxOffsets {};
    std::array<float, LAST_MUX> MuxScale {};

    //0 = L Audio, 1 = R Audio, 2 = L MOD, 3 = R MOD 
    std::array<float, LAST_OUT> OutputOffsets{}; 
    
    ///0 = L Audio, 1 = R Audio, 2 = L MOD, 3 = R MOD 
    std::array<float, LAST_OUT> OutputScale{}; 

    friend bool operator!= (const CV_Calibration& c1, const CV_Calibration& c2);
};


VenoOrbit_v1_0() {}
~VenoOrbit_v1_0() {}

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
    void UpdateClock();

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

    float GetOutputOffset(size_t channel);
    float GetOutputScale(size_t channel);

    CV_Calibration GetCalibration(){return calibration_;}

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

    // bool SaveCalibration() {return true;};
    // bool RestoreCalibration();

    //takes external calibration and saves it for use when calling 
    //GetMuxValue and GetCvValue etc.
    void SetCalibration(CV_Calibration& calibration);

    bool ValidateQSPI(bool quick = true);

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

    CV_Calibration calibration_{};

    //void InitMidi();
 
};

//operator for VenoLooper_v5::CV_Calibration
inline bool operator!= (const VenoOrbit_v1_0::CV_Calibration& c1, const VenoOrbit_v1_0::CV_Calibration& c2)
{
    return (c1.CV_Offsets != c2.CV_Offsets ||
        c1.MuxOffsets != c2.MuxOffsets ||
        c1.CV_Scale != c2.CV_Scale ||
        c1.MuxScale != c2.MuxScale ||
        c1.OutputOffsets != c2.OutputOffsets);
}
                  
} // namespace daisy 

#endif