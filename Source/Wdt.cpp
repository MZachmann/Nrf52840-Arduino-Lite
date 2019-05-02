#include "AppInclude.h"
#include "wdt.h"
#include "nrfx_wdt.h"
#include "ResetData.h"

// A Simple Watchdog Timer Interface
 // based on the watchdog timer example provided by Nordic
 nrfx_wdt_channel_id m_channel_id;

// called by the interrupt routine before we reboot
static void WatchTimer(void)
{
	// add personal code to say we were rebooted by the watchdog
}

WatchDog::WatchDog()
{
}

void WatchDog::StartWithTimeout(int timeSeconds)
{
	// Configure WDT using the NRFX_WDT_CONFIG_... values in sdk_config.h
	nrfx_wdt_config_t config = NRFX_WDT_DEAFULT_CONFIG;	// spelling? duh
	config.reload_value = timeSeconds * 1000;

	int err_code;
	err_code = nrfx_wdt_init(&config, WatchTimer);    // init watchdog with callback
	APP_ERROR_CHECK(err_code);
	err_code = nrfx_wdt_channel_alloc(&m_channel_id); // allocate a channel to feed it from
	APP_ERROR_CHECK(err_code);
    nrfx_wdt_enable();      // enable
}

// call this to reset the watchdog timer (start over)
void WatchDog::Ping(void)
{
	nrfx_wdt_channel_feed(m_channel_id);  // ping the watchdog
}
