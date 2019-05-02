 // TwoWire (I2C) Arduino-like support for the NRF52840
 // Copyright 2018 Mark Zachmann
 // MIT License
 //
#include <AppInclude.h>
#include "Wire.h"
#include "nrfx_twim.h"
// use variant.h to define the sda,scl pins
#include "../particle_xenon/variant.h"

/* TWI instance - must have NRFX_TWIM0_ENABLED=1 . */
static const nrfx_twim_t m_twi = NRFX_TWIM_INSTANCE(1);

static nrf_twim_frequency_t ConvertFrequency(uint32_t inFreq)
{
	nrf_twim_frequency_t fq;
	if (inFreq <= 100000)
    {
      fq = NRF_TWIM_FREQ_100K;
    }
    else if (inFreq <= 250000)
    {
      fq = NRF_TWIM_FREQ_250K;
    }
    else
    {
      fq = NRF_TWIM_FREQ_400K;
    }
	return fq;

}

TwoWire::TwoWire(uint8_t pinSDA, uint8_t pinSCL)
{
  this->_PinSDA = pinSDA;  // pinmap is identity
  this->_PinSCL = pinSCL;
  this->_Frequency = 100000; // NRFX_TWIM_DEFAULT_CONFIG_IRQ_FREQUENCY is very strange
  _IsSending = false;
  _IsMaster = true;
}

void TwoWire::begin(void) {
    ret_code_t err_code;
	nrf_twim_frequency_t setFreq = ConvertFrequency(_Frequency);

    const nrfx_twim_config_t twi_config = {
       .scl                = _PinSCL,
       .sda                = _PinSDA,
       .frequency          = setFreq,    // the config is some strange #
       .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,
       .hold_bus_uninit    = NRFX_TWIM_DEFAULT_CONFIG_HOLD_BUS_UNINIT
    };

    err_code = nrfx_twim_init(&m_twi, &twi_config, NULL, NULL);
    APP_ERROR_CHECK(err_code);

    nrfx_twim_enable(&m_twi);
}

void TwoWire::begin(uint8_t address) {
	// no slave support atm
  begin();
}

void TwoWire::setClock(uint32_t inFreq) 
{
	// in arduino this must be called before begin, so just set var
	this->_Frequency = inFreq;
}

void TwoWire::end() {
  if (_IsMaster)
  {
    nrfx_twim_disable(&m_twi);
	nrfx_twim_uninit(&m_twi);
  }
  else
  {
    //nrfx_twim_disable(&m_twi);
  }
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity, bool stopBit)
{
	_RwxOffset = 0;
	_RwxSize = 0;
	ret_code_t err_code = NRF_SUCCESS;
	if(quantity < ARRAY_SIZE(_RwxBuffer))
	{
		while(nrfx_twim_is_busy(&m_twi)); // ?
		err_code = nrfx_twim_rx(&m_twi, address, _RwxBuffer, quantity);
		if(err_code == NRF_SUCCESS)
		{
			_RwxSize = quantity;
		}
		else
		{
			_RwxSize = 0;
		}
	}
	else
	{
		err_code = 1;
	}
	return err_code;
}

uint8_t TwoWire::requestFrom(uint8_t address, size_t quantity)
{
  return requestFrom(address, quantity, true);
}

void TwoWire::beginTransmission(uint8_t address) {
  // save address of target and clear buffer
  _TwxAddress = address;
  _TwxSize = 0;
  _IsSending = true;
}

// Errors:
//  0 : Success
//  1 : Data too long
//  2 : NACK on transmit of address
//  3 : NACK on transmit of data
//  4 : Other error
uint8_t TwoWire::endTransmission(bool stopBit)
{
ret_code_t err_code = NRF_SUCCESS;
	if(_TwxSize > 0)
	{
		while(nrfx_twim_is_busy(&m_twi)); // ?
        err_code = nrfx_twim_tx(&m_twi, _TwxAddress, _TwxBuffer, _TwxSize, stopBit);
         //printf("Sending %d bytes with result=%d\r\n",(int)_TwxSize, (int)err_code);
	}
    return (uint8_t)err_code;
}

uint8_t TwoWire::endTransmission()
{
	return endTransmission(true);
}

size_t TwoWire::write(uint8_t ucData)
{

	if ( !_IsSending || _TwxSize == ARRAY_SIZE(_TwxBuffer) )
	{
		return 0 ;
	}

	_TwxBuffer[_TwxSize++] = ucData;
	return 1 ;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	size_t i;
	for(i = 0; i < quantity; ++i)
	{
		// if we failed to write a byte, stop
		if(0 == write(data[i]))
			break;
	}

	return i;
}

// write the string up to the null char or error
size_t TwoWire::write(const char* astring)
 {
	size_t i;
	for(i = 0; astring[i]; ++i)
	{
		if(!write(astring[i]))
			break;
	}

	return i;
}


int TwoWire::available(void)
{
	return _RwxOffset < _RwxSize;
}

int TwoWire::read(void)
{
	uint8_t odata = 0;
	if(_RwxOffset < _RwxSize)
	{
		odata = _RwxBuffer[_RwxOffset++];
	}
	return odata;
}

int TwoWire::peek(void)
{
	return _RwxBuffer[_RwxOffset];
}

void TwoWire::flush(void)
{
  // Do nothing, use endTransmission(..) to force
  // data transfer.
}

void TwoWire::onReceive(void(*function)(int))
{
	onReceiveCallback = function;
}

void TwoWire::onRequest(void(*function)(void))
{
	onRequestCallback = function;
}


TwoWire Wire(PIN_WIRE_SDA, PIN_WIRE_SCL);

