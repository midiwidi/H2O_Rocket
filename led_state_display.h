#ifndef LED_STATE_DISPLAY_H_
#define LED_STATE_DISPLAY_H_

#include "wemos_d1_mini.h"

#define LED_ON_DURATION	100
#define LED_PAUSE_SHORT 200
#define LED_PAUSE_LONG 1000

class LED_state_display
{
	public:
		LED_state_display();
		void setup(int pin);
		void update();
		void set_blinks(unsigned int);
		unsigned int get_blinks();
	private:
		unsigned long curr_millis;
		unsigned long last_millis;
		uint8 total_blinks;
		uint8 curr_blink;
		bool led_on;
		int pin;
};

#endif /* LED_STATE_DISPLAY_H_ */
