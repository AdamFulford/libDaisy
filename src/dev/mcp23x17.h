#pragma once

#include "per/gpio.h"
#include "per/i2c.h"

// This get defined in a public (ST) header file
#undef SetBit

namespace daisy
{
// Adapted from https://github.com/blemasle/arduino-mcp23017

/**
 * Registers addresses.
 * The library use addresses for IOCON.BANK = 0.
 * See "3.2.1 Byte mode and Sequential mode".
 */
enum class MCPRegister : uint8_t
{
    IODIR_A = 0x00, // Controls the direction of the data I/O for port A.
    IODIR_B = 0x01, // Controls the direction of the data I/O for port B.
    IPOL_A
    = 0x02, // Configures the polarity on the corresponding GPIO_ port bits for port A.
    IPOL_B
    = 0x03, // Configures the polarity on the corresponding GPIO_ port bits for port B.
    GPINTEN_A
    = 0x04, // Controls the interrupt-on-change for each pin of port A.
    GPINTEN_B
    = 0x05, // Controls the interrupt-on-change for each pin of port B.
    DEFVAL_A
    = 0x06, // Controls the default comparaison value for interrupt-on-change for port A.
    DEFVAL_B
    = 0x07, // Controls the default comparaison value for interrupt-on-change for port B.
    INTCON_A
    = 0x08, // Controls how the associated pin value is compared for the interrupt-on-change for port A.
    INTCON_B
    = 0x09, // Controls how the associated pin value is compared for the interrupt-on-change for port B.
    IOCON  = 0x0A, // Controls the device.
    GPPU_A = 0x0C, // Controls the pull-up resistors for the port A pins.
    GPPU_B = 0x0D, // Controls the pull-up resistors for the port B pins.
    INTF_A = 0x0E, // Reflects the interrupt condition on the port A pins.
    INTF_B = 0x0F, // Reflects the interrupt condition on the port B pins.
    INTCAP_A
    = 0x10, // Captures the port A value at the time the interrupt occured.
    INTCAP_B
    = 0x11, // Captures the port B value at the time the interrupt occured.
    GPIO_A = 0x12, // Reflects the value on the port A.
    GPIO_B = 0x13, // Reflects the value on the port B.
    OLAT_A = 0x14, // Provides access to the port A output latches.
    OLAT_B = 0x15, // Provides access to the port B output latches.
};

enum class MCPPort : uint8_t
{
    A = 0,
    B = 1
};

inline MCPRegister operator+(MCPRegister a, MCPPort b)
{
    return static_cast<MCPRegister>(static_cast<uint8_t>(a)
                                    + static_cast<uint8_t>(b));
}

enum class MCPMode : uint8_t
{
    INPUT,
    INPUT_PULLUP,
    OUTPUT,
};

/**
 * Barebones driver for MCP23017 I2C 16-Bit I/O Expander
 * For now it supports only polling approach.
 * 
 * Usage:
 *  Mcp23017 mcp;
 *  mcp.Init();
 *  mcp.PortMode(MCP23017Port::A, 0xFF); // Inputs
 *  mcp.PortMode(MCP23017Port::B, 0xFF);
 *  mcp.Read();
 *  mcp.GetPin(2);
 */
class Mcp23017Transport
{
  public:
    struct Config
    {
        I2CHandle::Config i2c_config;
        uint8_t           i2c_address;
        void              Defaults()
        {
            i2c_config.periph         = I2CHandle::Config::Peripheral::I2C_1;
            i2c_config.speed          = I2CHandle::Config::Speed::I2C_1MHZ;
            i2c_config.mode           = I2CHandle::Config::Mode::I2C_MASTER;
            i2c_config.pin_config.scl = Pin(PORTB, 8);
            i2c_config.pin_config.sda = Pin(PORTB, 9);
            i2c_address               = 0x27;
        }
    };

    void Init()
    {
        Config config;
        config.Defaults();
        Init(config);
    };

    void Init(const Config& config)
    {
        i2c_address_ = config.i2c_address << 1;
        i2c_.Init(config.i2c_config);
    };

    I2CHandle::Result WriteReg(MCPRegister reg, uint8_t val)
    {
        uint8_t data[1] = {val};
        return i2c_.WriteDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 1, timeout);
    }

    I2CHandle::Result WriteReg(MCPRegister reg, uint8_t portA, uint8_t portB)
    {
        uint8_t data[2] = {portA, portB};
        return i2c_.WriteDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 2, timeout);
    }

    uint8_t ReadReg(MCPRegister reg)
    {
        uint8_t data[1] = {0x00};
        i2c_.ReadDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 1, timeout);
        return data[0];
    }

    void ReadReg(MCPRegister reg, uint8_t& portA, uint8_t& portB)
    {
        uint8_t data[2];
        i2c_.ReadDataAtAddress(
            i2c_address_, static_cast<uint8_t>(reg), 1, data, 2, timeout);

        portA = data[0];
        portB = data[1];
    }

    daisy::I2CHandle i2c_;
    uint8_t          i2c_address_;
    uint8_t          timeout{10};
};

template <typename Transport>
class Mcp23X17
{
  public:
    struct Config
    {
        typename Transport::Config transport_config;
    };

    /** Specifies the expected behavior of the switch */
    enum Type
    {
        TYPE_TOGGLE,    /**< & */
        TYPE_MOMENTARY, /**< & */
    };

    void Init()
    {
        Config config;
        config.transport_config.Defaults();
        Init(config);

        //default all non-inverted and momentary
        for (size_t i=0; i>16; i++)
        {
            flip_[i] = false;
            t_[i] = TYPE_MOMENTARY;
        }

    };

    //polarity for each pin: 1 = inverted, 0 = non-inverted
    void Init(const Config& config)
    {
        transport.Init(config.transport_config);

        //BANK =     0 : sequential register addresses
        //MIRROR =     0 : use configureInterrupt
        //SEQOP =     1 : sequential operation disabled, address pointer does not increment
        //DISSLW =     0 : slew rate enabled
        //HAEN =     0 : hardware address pin is always enabled on 23017
        //ODR =     0 : open drain output
        //INTPOL =     0 : interrupt active low
        transport.WriteReg(MCPRegister::IOCON, 0b00100000);

        //enable all pull up resistors (will be effective for input pins only)
        transport.WriteReg(MCPRegister::GPPU_A, 0xFF, 0xFF);
    };

    //Set Momentary/toggle and polarity for a specific input id
    void SetSwitchType(uint8_t id, Type t, bool pol)
    {
        flip_ [id] = pol;
        t_[id] = t;
    }

    /**
     * Controls the pins direction on a whole port at once.
     * 
     * directions: 0 - output, 1 - input
     * pullups: 0 - disabled, 1 - enabled
     * inverted: 0 - false/normal, 1 - true/inverted
     * 
     * See "3.5.1 I/O Direction register".
     */
    void PortMode(MCPPort port,
                  uint8_t directions,
                  uint8_t pullups  = 0xFF,
                  uint8_t inverted = 0x00)
    {
        transport.WriteReg(MCPRegister::IODIR_A + port, directions);
        transport.WriteReg(MCPRegister::GPPU_A + port, pullups);
        transport.WriteReg(MCPRegister::IPOL_A + port, inverted);
    }

    /**
     * Controls a single pin direction. 
     * Pin 0-7 for port A, 8-15 fo port B.
     * 
     * 1 = Pin is configured as an input.
     * 0 = Pin is configured as an output.
     *
     * See "3.5.1 I/O Direction register".
     */
    void PinMode(uint8_t pin, MCPMode mode, bool inverted)
    {
        MCPRegister iodirreg  = MCPRegister::IODIR_A;
        MCPRegister pullupreg = MCPRegister::GPPU_A;
        MCPRegister polreg    = MCPRegister::IPOL_A;
        uint8_t     iodir, pol, pull;
        if(pin > 7)
        {
            iodirreg  = MCPRegister::IODIR_B;
            pullupreg = MCPRegister::GPPU_B;
            polreg    = MCPRegister::IPOL_B;
            pin -= 8;
        }
        iodir = transport.ReadReg(iodirreg);
        if(mode == MCPMode::INPUT || mode == MCPMode::INPUT_PULLUP)
            SetBit(iodir, pin);
        else
            ClearBit(iodir, pin);
        pull = transport.ReadReg(pullupreg);
        if(mode == MCPMode::INPUT_PULLUP)
            SetBit(pull, pin);
        else
            ClearBit(pull, pin);
        pol = transport.ReadReg(polreg);
        if(inverted)
            SetBit(pol, pin);
        else
            ClearBit(pol, pin);
        transport.WriteReg(iodirreg, iodir);
        transport.WriteReg(pullupreg, pull);
        transport.WriteReg(polreg, pol);
    }

    /**
     * Writes a single pin state.
     * Pin 0-7 for port A, 8-15 for port B.
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    void WritePin(uint8_t pin, uint8_t state)
    {
        MCPRegister gpioreg = MCPRegister::GPIO_A;
        uint8_t     gpio;
        if(pin > 7)
        {
            gpioreg = MCPRegister::GPIO_B;
            pin -= 8;
        }

        gpio = transport.ReadReg(gpioreg);
        if(state > 0)
        {
            gpio = SetBit(gpio, pin);
        }
        else
        {
            gpio = ClearBit(gpio, pin);
        }

        transport.WriteReg(gpioreg, gpio);
    }

    /**
     * Reads a single pin state.
     * Pin 0-7 for port A, 8-15 for port B.
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    uint8_t ReadPin(uint8_t pin)
    {
        MCPRegister gpioreg = MCPRegister::GPIO_A;
        uint8_t     gpio;
        if(pin > 7)
        {
            gpioreg = MCPRegister::GPIO_B;
            pin -= 8;
        }

        gpio = transport.ReadReg(gpioreg);
        if(ReadBit(gpio, pin))
        {
            return 1;
        }

        return 0;
    }

    /**
     * Writes pins state to a whole port.
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    void WritePort(MCPPort port, uint8_t value)
    {
        transport.WriteReg(MCPRegister::GPIO_A + port, value);
    }

    /**
     * Reads pins state for a whole port.
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    uint8_t ReadPort(MCPPort port)
    {
        return transport.ReadReg(MCPRegister::GPIO_A + port);
    }

    /**
     * Writes pins state to both ports.
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    void Write(uint16_t value)
    {
        transport.WriteReg(
            MCPRegister::GPIO_A, LowByte(value), HighByte(value));
    }

    /**
     * Reads pins state for both ports. 
     * 
     * 1 = Logic-high
     * 0 = Logic-low
     * 
     * See "3.5.10 Port register".
     */
    uint16_t Read()
    {
        uint8_t a = ReadPort(MCPPort::A);
        uint8_t b = ReadPort(MCPPort::B);

        pin_data = a | b << 8;
        return pin_data;
    }

    /**
     * @brief Fetches pin state from the result of recent Read() call. Useful to preserve unneeded reads
     * 
     * @param id pin ID
     * @return uint8_t pin state: 0x00 or 0xFF
     */
    uint8_t GetPin(uint8_t id) { return ReadBit(pin_data, id); }

    //id = id of pin to debounce
    void Debounce(uint8_t id)
    {
    // update no faster than 1kHz
    uint32_t now = System::GetNow();
    updated_[id]     = false;

    if(now - last_update_[id] >= 1)
    {
        last_update_[id] = now;
        updated_[id]     = true;

        // shift over, and introduce new state.
        
        uint8_t pinRead{};
        if (GetPin(id) == 0xff)
            pinRead = 0x01;
        else
            pinRead = 0x00;    
        
        state_[id]
            = (state_[id] << 1)
              | (flip_[id] ? !pinRead : pinRead);
        // Set time at which button was pressed
        if(state_[id] == 0x7f)
            {
                rising_edge_time_[id] = System::GetNow();
                if(t_[id] == TYPE_TOGGLE)
                {
                    toggleState_[id] = !toggleState_[id];
                }
            }
    }
    };

    /** \return true if a button was just pressed. */
    inline bool RisingEdge(uint8_t id) const { return updated_[id] ? state_[id] == 0x7f : false; } //01111111

    /** \return true if the button was just released */
    inline bool FallingEdge(uint8_t id) const
    {
        return updated_[id] ? state_[id] == 0x80 : false; //10000000
    }

    /** \return true if the button is held down (or if the toggle is on) */
    inline bool Pressed(uint8_t id) const 
    { 
        bool retval;
        if(t_[id] == TYPE_TOGGLE)
        {
            retval = toggleState_[id];
        }
        else
        {
            retval = (state_[id] == 0xff);
        }
        return retval; 
    }

    /** \return the time in milliseconds that the button has been held (or toggle has been on) */
    inline float TimeHeldMs(uint8_t id) const
    {
        return Pressed(id) ? System::GetNow() - rising_edge_time_[id] : 0;
    }

  private:
    uint8_t GetBit(uint8_t data, uint8_t id)
    {
        uint8_t mask     = 1 << id;
        uint8_t masked_n = data & mask;
        return masked_n >> id;
    }

    uint8_t SetBit(uint8_t data, uint8_t pos)
    {
        data = (data | (1 << pos));
        return data;
    }

    uint8_t ClearBit(uint8_t data, uint8_t pos)
    {
        data = (data & (~(1 << pos)));
        return data;
    }

    uint8_t ReadBit(uint16_t data, uint8_t pos)
    {
        return data & (1 << pos) ? 0xff : 0x00;
    }

    uint8_t LowByte(uint16_t val) { return val & 0xFF; }
    uint8_t HighByte(uint16_t val) { return (val >> 8) & 0xff; }

    uint16_t  pin_data;
    Transport transport;


    //debounce variables
    uint32_t last_update_[16];
    bool     updated_[16];
    Type     t_[16];
    uint8_t  state_[16];
    bool     flip_[16];
    bool     toggleState_[16];
    float    rising_edge_time_[16];

};

using Mcp23017 = Mcp23X17<Mcp23017Transport>;
} // namespace daisy
