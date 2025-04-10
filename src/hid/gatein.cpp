#include "hid/gatein.h"

using namespace daisy;

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