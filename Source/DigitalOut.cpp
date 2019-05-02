#include <stdint.h>
#include "DigitalPin.h"
#include "DigitalOut.h"
#include <nrf_gpio.h>

// Simple digital output facade


	// blank digitalout
	DigitalOut::DigitalOut() : _Pin(NOPIN)
	{
	}

    // Create a DigitalOut connected to the specified pin
	// and set the initial value
    DigitalOut::DigitalOut(uint8_t pin, int value)
    {
		SetPin(pin, value);
    }

	// define the pin# and set the initial value
    void DigitalOut::SetPin(uint8_t pin, int value)
    {
		_Pin = pin;
		if(pin != NOPIN)
		{
			nrf_gpio_cfg_output(pin);
			nrf_gpio_pin_write(pin, value);
		}
    }

	uint32_t DigitalOut::GetPin(void)
	{
		return _Pin;
	}

	bool DigitalOut::IsInitialized(void)
	{
		return _Pin != NOPIN;
	}

    // Set the output, specified as 0 or 1 (int)
    void DigitalOut::Write(int value)
    {
		if(_Pin != NOPIN)
			nrf_gpio_pin_write(_Pin, value);
    }

    // Return the output setting, represented as 0 or 1 (int)
    int DigitalOut::Read()
	{
		if(_Pin == NOPIN)
			return 0;

        return nrf_gpio_pin_out_read(_Pin);
    }

	// equivalent to write, assign an int
    DigitalOut & DigitalOut::operator= (bool value)
    {
        Write(value ? 1 : 0);
        return *this;
    }

	// equivalent to write, assign an int
    DigitalOut &DigitalOut::operator= (int value)
    {
        Write(value);
        return *this;
    }

	// another write but with rhs (x=srcobj)
    DigitalOut &DigitalOut::operator= (DigitalOut &rhs)
    {
        Write(rhs.Read());
        return *this;
    }

	// read shorthand  readresult = (int)obj
    DigitalOut::operator int()
    {
        return Read();
    }

