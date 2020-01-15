#include <Arduino.h>
#include "button_input.h"

button_input::button_input()
{
	this->pin = -1;
	this->click_count = 0;
	this->t_timeout = 0;
	this->t_last_btn_down = 0;
	this->duration = 0;
	this->last_event = none;
	this->last_event_click_count = 0;
	this->last_event_duration = 0;
}

void button_input::setup(int pin)
{
		this->pin = pin;
		pinMode(this->pin, INPUT);
		this->db_button.attach(this->pin);
		this->db_button.interval(50);
}

void button_input::update(void)
{
	unsigned long current_millis;

	db_button.update();
	current_millis = millis();

	if (this->db_button.fell()) // Button down
	{
		this->t_timeout = current_millis + BUTTON_TIMEOUT;
		this->t_last_btn_down = current_millis;

	}
	if (this->db_button.rose()) // Button up
	{
		this->click_count++;
		this->duration = current_millis - this->t_last_btn_down;

		if (this->t_timeout == 0)
		{
			this->last_event = press;
			this->last_event_click_count = this->click_count;
						this->last_event_duration = this->duration;
			//Serial.println();
			//Serial.print("long press duration: ");
			//Serial.println(this->duration);
			this->duration = 0;
			this->click_count = 0;
		}
	}

	if (this->t_timeout != 0 && current_millis > this->t_timeout)
	{
		this->t_timeout = 0;
		if (this->db_button.read() == LOW && this->click_count == 0)
		{
			// Long Click
		}
		else
		{
			this->last_event = click;
			this->last_event_click_count = this->click_count;
			this->last_event_duration = this->duration;
			//Serial.println();
			//Serial.print("button press count: ");
			//Serial.println(this->click_count);
			this->click_count = 0;
			//Serial.print("duration: ");
			//Serial.println(this->duration);
			this->duration = 0;

		}
	}
}

bool button_input::is_single_click()
{
	if (this->last_event == click && this->last_event_click_count == 1)
	{
		this->last_event = none;
		return true;
	}
	else
		return false;
}

bool button_input::is_double_click()
{
	if (this->last_event == click && this->last_event_click_count == 2)
	{
		this->last_event = none;
		return true;
	}
	else
		return false;
}

bool button_input::is_multi_click()
{
	if (this->last_event == click && this->last_event_click_count > 2)
	{
		this->last_event = none;
		return true;
	}
	else
		return false;
}

bool button_input::is_long_press()
{
	if (this->last_event == press)
	{
		this->last_event = none;
		return true;
	}
	else
		return false;
}

unsigned int button_input::get_duration()
{
	return this->last_event_duration;
}

uint8 button_input::get_click_count()
{
	return this->last_event_click_count;
}
