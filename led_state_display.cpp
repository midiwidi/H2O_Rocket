#include <Arduino.h>
#include "led_state_display.h"

LED_state_display::LED_state_display()
{
	this->curr_millis = millis();
	this->last_millis = this->curr_millis;
	this->total_blinks = 0;
	this->curr_blink = 0;
	this->led_on = false;
	this->pin = -1;
}

void LED_state_display::setup(int pin)
{
	this->pin = pin;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	this->led_on = false;
}

void LED_state_display::set_blinks(unsigned int blinks)
{
	this->curr_blink = 0;
	this->total_blinks = blinks;
}

unsigned int LED_state_display::get_blinks(void)
{
	return this->total_blinks;
}

void LED_state_display::update(void)
{
	this->curr_millis = millis();

	if (this->total_blinks != 0)
	{
		if (this->led_on == false) // LED is off
		{
			if (this->curr_blink < this->total_blinks)
			{
				if ((this->curr_millis - this->last_millis) > LED_PAUSE_SHORT)
				{
					digitalWrite(this->pin, LOW);
					this->led_on = true;
					this->curr_blink++;
					if (this->curr_blink > this->total_blinks)
						this->curr_blink = 1;
					this->last_millis = this->curr_millis;
				}
			}
			else
			{
				if ((this->curr_millis - this->last_millis) > LED_PAUSE_LONG)
				{
					digitalWrite(this->pin, LOW);
					this->led_on = true;
					this->curr_blink++;
					if (this->curr_blink > this->total_blinks)
						this->curr_blink = 1;
					this->last_millis = this->curr_millis;
				}
			}
		}
		else // LED is on
		{
			if ((this->curr_millis - this->last_millis) > LED_ON_DURATION)
			{
				digitalWrite(this->pin, HIGH);
				this->led_on = false;
				this->last_millis = this->curr_millis;
			}
		}
	}
}
