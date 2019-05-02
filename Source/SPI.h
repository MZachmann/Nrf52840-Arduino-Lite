#ifndef SPI_ARD_LAYER_H
#define SPI_ARD_LAYER_H

#include <StdInt.h>

// hacky include for arduino mode
#define SPI_MODE0 0x02

class SPISettings
{
	public:
		// init and provide values (last chance to do that)
		SPISettings(uint32_t maxSpeed, BitOrder bitOrder, uint8_t dataMode)
		{
			_MaxSpeed = maxSpeed;
			_BitOrder = bitOrder;
			_DataMode = dataMode;
		}

		// a default one below can be edited
		SPISettings() :
			_MaxSpeed(100000),
			_BitOrder(MSBFIRST),
			_DataMode(SPI_MODE0)
                    {}
					
		uint32_t	_MaxSpeed;
		BitOrder	_BitOrder;
		uint8_t		_DataMode;
};

// model for SPI object defined below
// Arduino methods defined as Deprecated aren't included
class SPIModel
{
      public:
                // create the model and store the settings
		SPIModel(uint8_t pinMosi, uint8_t pinMiso, uint8_t pinSck, uint8_t pinCs);
                // initialize, uninit methods - currently nops
		void begin();
		void end();
                // start, end transaction for each read/write
		void beginTransaction(const SPISettings& settings);
		void endTransaction();
                // bidirectional transfers
		uint8_t transfer(uint8_t data);
		uint16_t transfer16(uint16_t data);
		void transfer(void* data, uint16_t count);
                // block a specific interrupt
		void usingInterrupt(uint8_t irq) {}	// n/a

	private:
		uint8_t _PinMosi;
		uint8_t _PinMiso;
		uint8_t _PinSck;
		uint8_t _PinCs;
		uint32_t	_MaxSpeed;
		BitOrder	_BitOrder;
		uint8_t		_DataMode;
                bool  _IsInitialized;
};

extern SPIModel SPI;  // you must build this somewhere with the right arguments

#endif