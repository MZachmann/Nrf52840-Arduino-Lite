#ifndef DIGITALOUT_H
#define DIGITALOUT_H
#include "DigitalPin.h"

// Simple digital output facade
class DigitalOut {

public:

	DigitalOut();	// blank digitalout
    // Create a DigitalOut connected to the specified pin
	// and set the initial value
    DigitalOut(uint8_t pin, int value);
	// define the pin# and set the initial value
    void SetPin(uint8_t pin, int value);
	uint32_t GetPin(void);
	bool IsInitialized(void);
    void Write(int value);    // Set the output, specified as 0 or 1 (int)
    int Read();    // Return the output setting, represented as 0 or 1 (int)
    DigitalOut &operator= (bool value);	// write, assign the bool
    DigitalOut &operator= (int value);	// write, assign an int
    DigitalOut &operator= (DigitalOut &rhs);	// another write but with rhs (x=srcobj)
    operator int();	// read shorthand  readresult = (int)obj

protected:
    uint8_t _Pin;
};

#endif
