#include "hid/encoder.h"

using namespace daisy;

void Encoder::Init(Pin a, Pin b, Pin click, float update_rate)
{
    last_update_ = System::GetNow();
    updated_     = false;

    // Init GPIO for A, and B
    hw_a_.Init(a, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
    hw_b_.Init(b, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
    // Default Initialization for Switch
    sw_.Init(click);
    // Set initial states, etc.
    inc_ = 0;
    a_ = b_ = 0xff;
}

void Encoder::Init(dsy_gpio_pin a,
                   dsy_gpio_pin b)
{
    last_update_ = System::GetNow();
    updated_     = false;

    // Init GPIO for A, and B
    hw_a_.pin  = a;
    hw_a_.mode = DSY_GPIO_MODE_INPUT;
    hw_a_.pull = DSY_GPIO_PULLUP;
    hw_b_.pin  = b;
    hw_b_.mode = DSY_GPIO_MODE_INPUT;
    hw_b_.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&hw_a_);
    dsy_gpio_init(&hw_b_);
    // Default Initialization for Switch
    //sw_.Init(click);
    // Set initial states, etc.
    inc_ = 0;
    a_ = b_ = 0xff;
}

void Encoder::Debounce()
{
    // update no faster than 1kHz
    uint32_t now = System::GetNow();
    updated_     = false;

    if(now - last_update_ >= 1)
    {
        last_update_ = now;
        updated_     = true;

        // Shift Button states to debounce
        a_ = (a_ << 1) | hw_a_.Read();
        b_ = (b_ << 1) | hw_b_.Read();

        // infer increment direction
        inc_ = 0; // reset inc_ first
        if((a_ & 0x03) == 0x02 && (b_ & 0x03) == 0x00)
        {
            inc_ = 1;
        }
        else if((b_ & 0x03) == 0x02 && (a_ & 0x03) == 0x00)
        {
            inc_ = -1;
        }
    }

    // Debounce built-in switch
    sw_.Debounce();
}
