# define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Set Data Direction Register B, Pin 5 as output.
    DDRB = (1<<DDB5);
    // Set pins/bits 5 to high (VCC).
		while (1) {
			PORTB |= (1<<PORTB5);
			_delay_ms(1000);

			PORTB &= ~(1<<PORTB5);
			_delay_ms(1000);
		}

		return 0;
}

