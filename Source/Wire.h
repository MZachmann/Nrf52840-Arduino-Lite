/*
 * TWI/I2C library for mRF5x
 * Copyright (c) 2015 Arduino LLC. All rights reserved.
 * Copyright (c) 2016 Sandeep Mistry All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef TwoWire_h
#define TwoWire_h

#define WIRE_INTERFACES_COUNT 1

#include <stdint.h>
#include <stddef.h>

class TwoWire
{
  public:
    TwoWire(uint8_t pinSDA, uint8_t pinSCL);

	// setup
    void begin();
    void begin(uint8_t);

	// shutdown
    void end();

	// change clock _Frequency
    void setClock(uint32_t inFreq);

	// start transmission to slave
    void beginTransmission(uint8_t);

	// end transmission to slave
    uint8_t endTransmission(bool stopBit);
    uint8_t endTransmission(void);

	// request slave transfer to buffer
    uint8_t requestFrom(uint8_t address, size_t quantity, bool stopBit);
    uint8_t requestFrom(uint8_t address, size_t quantity);

	// write to slave
    size_t write(uint8_t data);
    size_t write(const uint8_t * data, size_t quantity);
    size_t write(const char* astring);

	// read from slave buffer
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);

	// register interrupt routine handlers
    void onReceive(void(*)(int));
    void onRequest(void(*)(void));

  private:
	uint32_t _Frequency;

    uint8_t _PinSDA;
    uint8_t _PinSCL;

    bool _IsMaster;
    bool _IsSending;

    // RX Buffer
    uint8_t _RwxBuffer[64];
	uint8_t _RwxOffset;
	uint8_t _RwxSize;

    // TX buffer
    uint8_t _TwxBuffer[64];
	uint8_t _TwxSize;		// no tx offset since we don't read it

    uint8_t _TwxAddress;

    // Callback user functions
    void (*onRequestCallback)(void);
    void (*onReceiveCallback)(int);
};

#if WIRE_INTERFACES_COUNT > 0
extern TwoWire Wire;
#endif

#endif
