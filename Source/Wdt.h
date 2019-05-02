#ifndef WATCH_DOG_H
#define WATCH_DOG_H


// A simple watchdog timer class. Start it then ping it more often than timeSeconds
// or the system will reboot
class WatchDog
{
	public:
		WatchDog();
		void StartWithTimeout(int timeSeconds);	// set the watchdog timer timeout
		void Ping(void);					// tell it we're awake
};

#endif