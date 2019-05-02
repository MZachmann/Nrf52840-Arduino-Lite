#include <stdint.h>
#include "DigitalPin.h"
#include "DigitalIn.h"
#include <nrf_gpio.h>

// Simple digital input facade. Can be constructed immediately or delayed

    DigitalIn::DigitalIn() : _Pin(NOPIN)
    {
    }

    // Create a DigitalIn connected to the specified pin
	DigitalIn::DigitalIn(uint8_t pin)
    {
		SetPin(pin);
    }

    // Create a DigitalIn connected to the specified pin
    DigitalIn::DigitalIn(uint8_t pin, uint8_t mode)
    {
		SetPin(pin, mode);
    }

    void DigitalIn::SetPin(uint8_t pin) 
    {
		SetPin(pin, NRF_GPIO_PIN_NOPULL);
    }

    void DigitalIn::SetPin(uint8_t pin, uint8_t mode) 
    {
		_Pin = pin;
		if(pin != NOPIN)
		{
			nrf_gpio_cfg_input(pin, (nrf_gpio_pin_pull_t)mode);
		}
    }

	uint8_t DigitalIn::GetPin(void)
	{
		return _Pin;
	}

	bool DigitalIn::IsInitialized(void)
	{
		return _Pin != NOPIN;
	}

    // Read the pin, return 0, 1
    int DigitalIn::Read()
    {
        if(_Pin == NOPIN)
        {
            // assert?
            return 0;
        }
        return nrf_gpio_pin_read(_Pin);
    }

    // Set the input pin mode nrf_gpio_pin_pull_t
    void DigitalIn::Mode(uint8_t pmode)
    {
        if(_Pin != NOPIN)
          nrf_gpio_cfg_input(_Pin, (nrf_gpio_pin_pull_t)pmode);
    }

    // A shorthand for read()
    DigitalIn::operator int()
    {
        // Underlying call is thread safe
        return (0 != Read()) ? 1 : 0;
    }
