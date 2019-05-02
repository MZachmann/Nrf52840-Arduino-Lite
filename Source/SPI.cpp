#include <AppInclude.h>
#include "SPI.h"
#include "nrfx_spi.h"
#include <variant.h>

// this is a simple Arduino-similar SPI interface
// it uses the predefined 0 SPI instance (using NRFX_SPI0_ENABLE)
// This shares interfaces with I2C so use a different one there
// Arduino doesn't use interrupt routines for spi, so just run synchronous

// random arduino defines leftover
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

#define SPI_INSTANCE  0   // change if needed
// dependent on NRFX_SPI0_ENABLED
static const nrfx_spi_t _SpiInstance = NRFX_SPI_INSTANCE(SPI_INSTANCE);  // instance address and ptr to spi settings

static uint8_t _RxBuffer[100];	// these need to be static for asynch
static uint8_t _TxBuffer[100];	// and it's simpler for synch, so...

// this static instance uses the arduino defines from variant.h (sigh)
// ------------------------------------------------
SPIModel SPI(MOSI, MISO, SCK, SS);
// ------------------------------------------------

// notes on the chip-select from nordic below
// pinCs Slave Select pin number (optional).
//    Set to @ref NRF_DRV_SPI_PIN_NOT_USED (0xff) if this signal is not needed. The driver
//      supports only active low for this signal. If the signal should be active high,
//      it must be controlled externally.

	SPIModel::SPIModel(uint8_t pinMosi, uint8_t pinMiso, uint8_t pinSck, uint8_t pinCs) 
	{
		_PinMosi = pinMosi;
		_PinMiso = pinMiso;
		_PinSck = pinSck;
		_PinCs = pinCs;
		SPISettings dummy;	// inits to default
		_BitOrder = dummy._BitOrder;
		_MaxSpeed = dummy._MaxSpeed;
		_DataMode = dummy._DataMode;	
		_IsInitialized = false;
	}

	void SPIModel::begin()
	{
	}

	void SPIModel::end()
	{
	}

    // convert arduino std to nrf std
	static nrf_spi_mode_t ConvertMode(uint8_t dataMode)
	{
		switch(dataMode)
		{
		case SPI_MODE0 :
			return NRF_SPI_MODE_0;
		case SPI_MODE1 :
			return NRF_SPI_MODE_1;
		case SPI_MODE2 :
			return NRF_SPI_MODE_2;
		case SPI_MODE3 :
			return NRF_SPI_MODE_3;
		default:
			// error?
			break;
		}
		return NRF_SPI_MODE_0;
	}

    // convert arduino std to nrf std
	static nrf_spi_bit_order_t ConvertBitOrder(BitOrder bo)
	{
		return (bo == MSBFIRST) ? NRF_SPI_BIT_ORDER_MSB_FIRST : NRF_SPI_BIT_ORDER_LSB_FIRST;
	}

    // convert arduino std to nrf std
	static nrf_spi_frequency_t ConvertSpeed(uint32_t maxSpeed)
	{
	static const uint32_t speedLimits[] = {125000L, 250000L, 500000L, 1000000L, 2000000L, 4000000L, 8000000L };
	static const nrf_spi_frequency_t nrfLimits[] = {
		NRF_SPI_FREQ_125K, ///< 125 kbps.
		NRF_SPI_FREQ_250K , ///< 250 kbps.
		NRF_SPI_FREQ_500K , ///< 500 kbps.
		NRF_SPI_FREQ_1M   ,   ///< 1 Mbps.
		NRF_SPI_FREQ_2M   ,   ///< 2 Mbps.
		NRF_SPI_FREQ_4M   ,   ///< 4 Mbps.
		NRF_SPI_FREQ_8M       ///< 8 Mbps.
		};
	uint8_t i;

		for(i=0; i<ARRAY_SIZE(speedLimits); i++)
		{
			if(maxSpeed <= speedLimits[i])
				return nrfLimits[i];
		}

		return nrfLimits[i-1]; // 8M if over 8M requested
	}

	void SPIModel::beginTransaction(const SPISettings& settings)
	{
        bool uu = !_IsInitialized;	// putting this inline below failed because ?
		if(uu || (settings._BitOrder != _BitOrder || settings._MaxSpeed != _MaxSpeed || settings._DataMode != _DataMode))
		{
			_BitOrder = settings._BitOrder;
			_MaxSpeed = settings._MaxSpeed;
			_DataMode = settings._DataMode;

			// reinit
			if(_IsInitialized)
			{
				nrfx_spi_uninit(&_SpiInstance);	// the way to change settings
				_IsInitialized = false;
			}

			nrfx_spi_config_t spiConfig = NRFX_SPI_DEFAULT_CONFIG;
			spiConfig.ss_pin   = _PinCs;
			spiConfig.miso_pin = _PinMiso;
			spiConfig.mosi_pin = _PinMosi;
			spiConfig.sck_pin  = _PinSck;

			spiConfig.bit_order = ConvertBitOrder(settings._BitOrder);
			spiConfig.frequency = ConvertSpeed(settings._MaxSpeed);
			spiConfig.mode = ConvertMode(settings._DataMode);

			// Execute
			NrfErrorCheck(nrfx_spi_init(&_SpiInstance, &spiConfig, NULL, NULL));
			_IsInitialized = true;
		}
	}

	void SPIModel::endTransaction()
	{

	}

	static void SynchTransfer(uint8_t length)
	{
		nrfx_spi_xfer_desc_t xferDesc =
		{
			.p_tx_buffer = _TxBuffer,
			.tx_length = length,
			.p_rx_buffer = _RxBuffer,
			.rx_length = length
		};

		// Execute
		NrfErrorCheck(nrfx_spi_xfer(&_SpiInstance, &xferDesc, 0));
	}

	uint8_t SPIModel::transfer(uint8_t data)
	{
		_TxBuffer[0] = data;
		SynchTransfer(1);
		return _RxBuffer[0];
	}

	uint16_t SPIModel::transfer16(uint16_t data)
	{
		_TxBuffer[0] = (uint8_t)data;
		_TxBuffer[1] = (uint8_t)data>>8;
		SynchTransfer(2);
		uint16_t result = _RxBuffer[1];	// ensure convert to right size
		result =  _RxBuffer[1] + result << 8;
		return result;
	}

	void SPIModel::transfer(void* data, uint16_t count)
	{
		if(count > ARRAY_SIZE(_RxBuffer))
		{
			// fail?
			count = ARRAY_SIZE(_RxBuffer);
		}
		memcpy(_TxBuffer, data, count);
		SynchTransfer(count);
		memcpy(data, _RxBuffer, count);
	}
