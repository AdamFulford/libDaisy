#include "hid/gatein.h"

using namespace daisy;

void GateIn::Init(dsy_gpio_pin *pin_cfg, bool invert)
{
    /** Converting old type to new type */
    Pin p(static_cast<GPIOPort>(pin_cfg->port), pin_cfg->pin);
    pin_.Init(p, GPIO::Mode::INPUT);
    prev_state_ = false;
    state_      = false;
    invert_     = invert;
}


void GateIn::Init(Pin pin_cfg, bool invert)
{
    pin_.Init(pin_cfg, GPIO::Mode::INPUT);
    prev_state_ = false;
    state_      = false;
    invert_     = invert;
}

void GateIn::Update()
{
    prev_state_ = state_;
    state_      = invert_ ? !pin_.Read() : pin_.Read();
}

bool GateIn::Trig()
{
    return state_ && !prev_state_;
}

bool GateIn::Released()
{
    return !state_ && prev_state_;
}