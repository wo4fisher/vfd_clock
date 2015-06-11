#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "clock.h"
#include "display.h"
#include "tubechars.h"

void ioinit (void)
{
	//1 = output, 0 = input
	DDRB = 0b11111111;
	DDRC = 0b11111111;
	DDRD = 0b11111111;
	PORTB = 0; 
	PORTC = 0; 
	PORTD = 0; 
}

void count()
{
	uint32_t c_data;
	uint8_t i;

	c_data = 0xfffff;
	tube0.grid->c_data = c_data;
	refresh(&clock_display);
	
	c_data = 0;
	tube0.grid->c_data = c_data;
	refresh(&clock_display);

	for (i = 0; i < DIGITS_LEN; ++i) {
		c_data = 0;
		render_tubechar(&c_data, &tube0, DIGITS[i]);
		tube0.grid->c_data = c_data;
		refresh(&clock_display);
	}

	for (i = 0; i < LETTERS_LEN; ++i) {
		c_data = 0;
		render_tubechar(&c_data, &tube0, LETTERS[i]);
		tube0.grid->c_data = c_data;
		refresh(&clock_display);
	}
}

void say(char *what)
{
	size_t len = strlen(what);
	size_t i;
	tubechar_t tc;

	for (i = 0; i < len; ++i) {
		if ((tc = to_tubechar(what[i])) == -1) {
			continue;
		}
		tube0.grid->c_data = 0;
		render_tubechar(&tube0.grid->c_data, &tube0, tc);
		refresh(&clock_display);
	}
}

int main()
{
	clock_init();
	ioinit();
	send_controller(&controller0, 0);
	say("Hello World");

	for(;;) {
		PORTC = 0xFF;
		_delay_ms(500.0);
		PORTC = 0x0;
		_delay_ms(500.0);
	}
}