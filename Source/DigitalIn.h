#ifndef DIGITALIN_H
#define DIGITALIN_H

#include "DigitalPin.h"

// Simple digital input facade. Can be constructed immediately or delayed
class DigitalIn 
{
public:
    DigitalIn();
    // Create a DigitalIn connected to the specified pin
	DigitalIn(uint8_t pin);
    // Create a DigitalIn connected to the specified pin
    DigitalIn(uint8_t pin, uint8_t mode);
    void SetPin(uint8_t pin);
    void SetPin(uint8_t pin, uint8_t mode);
	uint8_t GetPin(void);
	bool IsInitialized(void);
    int Read();    // Read the pin, return 0, 1
    void Mode(uint8_t pull);    // Set the input pin mode
    operator int();    // A shorthand for read()

protected:
    uint8_t _Pin;
};


#endif
