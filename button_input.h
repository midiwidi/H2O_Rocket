// Class for button functions
// It can detect single, double and multiple clicks of a button and if the button was held down for a while (long press).
// If the button is down for less than BUTTON_TIMEOUT, it is considered a click. If the button is down for more than BUTTON_TIMEOUT,
// the event is considered a long press.
// The class uses the library Bounce2 for de-bouncing of the button.
//
// Member functions:
//		void setup(int pin);
//		-> Sets the pin as an input and configures the de-bouncing (timeout...). Is usually called once in the setup() function.
//
//		void update();
//		-> Needs to be called periodically in the Loop() function (before the other functions are called).
//
//
//		The following functions are used to check for button events. If the event which was checked for has happened,
//		the function returns true and deletes the event. A second call of the same checker function will return false unless another
//		event has happened between the first and the second call. In other words: Each event is only reported once.
//
//		bool is_single_click();
//		-> Returns true if the previous event was a single click. Use get_duration() for the duration of the click.
//
//		bool is_double_click();
//		-> Returns true if the previous event was a double click. Use get_duration() for the duration of the last click.
//
//		bool is_multi_click();
//		-> Returns true if the previous event was a multi click which means that there was more than 3 clicks.
//		Use get_duration() for the duration of the last click and get_click_count() to find out how many times the button was clicked.
//
//		bool is_long_press();
//		-> Returns true if the button was held down for more than BUTTON_TIMEOUT milliseconds. Use get_duration() for the duration of the long press.
//
//		unsigned int get_duration();
//		-> Returns the duration of the last event (in milliseconds)
//
//		uint8 get_click_count();
//		-> Returns the number of clicks from the last event.
//
//
//		Example:
//
//		button_input button1;
//
//		void setup()
//		{
//			...
//			button1.setup(PIN_BUTTON);
//			...
//		}
//
//		void loop()
//		{
//			button1.update();
//
//			if (button1.is_single_click())
//			{
//				Serial.println();
//				Serial.print("button single click, duration: ");
//				Serial.println(button1.get_duration());
//			}
//			if (button1.is_double_click())
//			{
//				Serial.println();
//				Serial.print("button double click, duration: ");
//				Serial.println(button1.get_duration());
//			}
//			if (button1.is_multi_click())
//			{
//				Serial.println();
//				Serial.print("button multi click, count: ");
//				Serial.print(button1.get_click_count());
//				Serial.print(", duration: ");
//				Serial.println(button1.get_duration());
//			}
//			if (button1.is_long_press())
//			{
//				Serial.println();
//				Serial.print("button long press, duration: ");
//				Serial.println(button1.get_duration());
//			}
//		}


#ifndef BUTTON_INPUT_H_
#define BUTTON_INPUT_H_

#include "wemos_d1_mini.h"
#include <Bounce2.h>

#define BUTTON_TIMEOUT	500

typedef enum btn_event_t
{
	none = 0,
	click = 1,
	press = 2
};

class button_input
{
	public:
		button_input();
		void setup(int pin);
		void update();
		bool is_single_click();
		bool is_double_click();
		bool is_multi_click();
		bool is_long_press();
		unsigned int get_duration();
		uint8 get_click_count();
	private:
		int pin;
		Bounce db_button;
		uint8 click_count;
		unsigned long t_timeout;
		unsigned long t_last_btn_down;
		unsigned int duration;

		btn_event_t last_event;
		unsigned int last_event_duration;
		unsigned int last_event_click_count;
};

#endif /* BUTTON_INPUT_H_ */
