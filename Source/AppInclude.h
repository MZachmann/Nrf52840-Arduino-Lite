#ifndef APP_INCLUDE_H
#define APP_INCLUDE_H

// the analog to Arduino.h

#include "sdk_common.h"
#include "config/sdk_config.h"
#include <nrf_delay.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(NRF_SDH_STACK_OBSERVER)
static inline uint8_t noInterrupts()
{
uint8_t isNested = false;
	#ifdef SOFTDEVICE_PRESENT
		if( nrf_sdh_is_enabled())
			sd_nvic_critical_region_enter(&isNested);
		else
	#endif
		__disable_irq();
    return isNested;
}
static inline void interrupts(uint8_t wasNested)
{
	#ifdef SOFTDEVICE_PRESENT
		if( nrf_sdh_is_enabled())
			sd_nvic_critical_region_exit(wasNested);
		else
	#endif
		__enable_irq();
}
#endif

static inline void delay(uint32_t timeMs)
{
   nrf_delay_ms(timeMs);
}

static inline void delayMicroseconds(uint32_t timeUs)
{
   nrf_delay_us(timeUs);
}

extern uint32_t millis( void );
extern void NrfErrorCheck(uint32_t errorCode);	// print error if happens, don't crash

#ifdef __cplusplus 
class String;
#endif

typedef enum
{
 MSBFIRST,
 LSBFIRST
} BitOrder;

#ifdef __cplusplus
}
#endif

#define min(x,y) (((x)>(y))?(y):(x))
#define max(x,y) (((x)>(y))?(x):(y))
#define digitalPinToInterrupt(P)   ( P )

#endif
