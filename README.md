# Nrf52840-Arduino-Lite
These are relatively high level versions things like I2C, SPI, Interrupt handlers that use an Arduino-like syntax to make creating apps using the Nrf5 sdk easier. This code is specific to the nrf52840.

This is not a complete library. It uses an sdk_config.h that is not supplied (since it is dependent on which modules are used) and it uses some simple helper functions such as NrfErrorCheck. In most cases pins are not mapped like usual Arduino but the pin numbers are just used directly.

# Winterrupts
A simple interrupt-handling scheme that uses gpiote to allow an interrupt per pin change.

# WString
The usual arduino string-handling code

# Wire
A quick two-wire (I2C) implementation. This creates a single static TwoWire object like Arduino does.

# Wdt
A really easy watchdog timer implementation

# SPI
A simple SPI library.

# DigitalPin and DigitalIn/Out
An abstraction layer on top of pin i/o to make it simpler to do pins as output and input for nrf5 and arduino code reuse.

