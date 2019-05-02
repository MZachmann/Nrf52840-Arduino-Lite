#include "AppInclude.h"
#include "WInterrupts.h"
#include <nrfx_gpiote.h>
#include "DigitalPin.h"

// this simply maps pin #s to callback method pointers
// the in_pin_handler deals with all interrupts, which then get filtered and
// potentially called back

// the maximum number of pin-interrupts this supports
#define MAX_USERS 4

static bool _IsInitialized = false;
static voidFuncPtr _Callbacks[MAX_USERS];
static uint8_t _Pins[MAX_USERS];

// convert the generalized interrupt into one per specific pin
void in_pin_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
uint8_t i;
	for(i=0; i<MAX_USERS; i++)
	{
		if(pin == _Pins[i])
		{
			if(_Callbacks[i])
				(*_Callbacks[i])();
			break;
		}
	}
}

// Initialize the gpiote system and clear the pin-mapping table
static void gpio_init(void)
{
	for(uint8_t i=0; i<MAX_USERS; i++)
	{
		_Pins[i] = NOPIN;
		_Callbacks[i] = 0;
	}

    ret_code_t err_code = nrfx_gpiote_init();
    NrfErrorCheck(err_code);
}

// attach a callback function to a pin interrupt. 
// Pins can only have one interrupt sense mode and one callback
int attachInterrupt(uint32_t pin, voidFuncPtr callback, uint32_t mode)
{
	nrfx_gpiote_in_config_t inconfig;
	if(!_IsInitialized)
	{
		// lazy initialize
		_IsInitialized = true;
		gpio_init();
	}

	// map the callback function. This has absolutely no checking for too many interrupts
	for(uint8_t i=0; i<MAX_USERS; i++)
	{
		if(_Pins[i] == pin || _Pins[i] == NOPIN)
		{
			_Pins[i] = pin;
			_Callbacks[i] = callback;
			break;
		}
	}

	// this currently supports rising and falling
	switch(mode)
	{
		case RISING:
			{
				nrfx_gpiote_in_config_t aconfig = NRFX_GPIOTE_CONFIG_IN_SENSE_LOTOHI(false);
				inconfig = aconfig;
				inconfig.pull = NRF_GPIO_PIN_NOPULL; // NRF_GPIO_PIN_PULLDOWN;
			}
			break;
		case FALLING :
			{
				nrfx_gpiote_in_config_t bconfig = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(false);
				inconfig = bconfig;
				inconfig.pull = NRF_GPIO_PIN_NOPULL; // NRF_GPIO_PIN_PULLUP;
			}
			break;
		default :
			{
				nrfx_gpiote_in_config_t cconfig = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
				inconfig = cconfig;
				inconfig.pull = NRF_GPIO_PIN_NOPULL;
			}
			break;
	}


    int err_code = nrfx_gpiote_in_init(pin, &inconfig, in_pin_handler);
    NrfErrorCheck(err_code);

    nrfx_gpiote_in_event_enable(pin, true);
}

// get rid of a table entry for this pin
void detachInterrupt(uint32_t pin)
{
	// clear the table entry
	for(uint8_t i=0; i<MAX_USERS; i++)
	{
		if(_Pins[i] == pin)
		{
			_Pins[i] = NOPIN;
			break;
		}
	}
}
