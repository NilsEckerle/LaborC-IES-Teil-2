#include "hardware/clock.h"
#include "tools/bit_functions.h"
#include "tools/logger.h"

// Clock variables
volatile uint16_t milliseconds = 0;
volatile uint32_t seconds = 0;

// Timer1 overflow interrupt service routine
ISR(TIMER1_OVF_vect) {
  milliseconds += 32;

	if (milliseconds >= 1000) {
		milliseconds -= 1000;
		seconds++;
	}
}

void CLOCK_init() {
  // Disable interrupts during setup
  cli();

  // Reset Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // Configure Timer1 for Normal mode (WGM13:0 = 0000)
  UNSET_BIT(TCCR1A, CLOCK_WGM10_BIT);
  UNSET_BIT(TCCR1A, CLOCK_WGM11_BIT);
  UNSET_BIT(TCCR1B, CLOCK_WGM12_BIT);
  UNSET_BIT(TCCR1B, CLOCK_WGM13_BIT);

  // Set prescaler to 64 (CS12:0 = 011)
  // This gives us: 16MHz / 8 = 2MHz timer frequency
  // Timer period = 1/2MHz = 5e-7 microseconds per tick
  // Overflow occurs every 65536 ticks = 65536 * 5e-7 = 32,768 microseconds
  UNSET_BIT(TCCR1B, CLOCK_CS10_BIT);
  SET_BIT(TCCR1B, CLOCK_CS11_BIT);
  UNSET_BIT(TCCR1B, CLOCK_CS12_BIT);

  // Enable Timer1 overflow interrupt
  SET_BIT(TIMSK1, CLOCK_TOIE1_BIT);

  // Enable global interrupts
  sei();

	INFO("[CLOCK_init] Clock initialization successful\n");
}


uint32_t CLOCK_get_seconds(void) { 
  uint32_t s;

  // Disable interrupts to ensure atomic read
  cli();
  s = seconds;
  sei();

  return s;
}

// Get current time in milliseconds
uint32_t CLOCK_get_milliseconds() {
  uint32_t ms;

  // Disable interrupts to ensure atomic read
  cli();
  ms = milliseconds + seconds * 1000;
  sei();

  return ms;
}
