#include "logger.h"

#include <avr/io.h>
#include <util/delay.h>

#include "shift_register.h"

#define BTN_DATA_DDR DDRD
#define BTN_DATA_PORT PORTD
#define BTN_DATA_PIN PIND
#define BTN_DATA_Bit 2

#define BTN_CLOCK_DDR DDRD
#define BTN_CLOCK_PORT PORTD
#define BTN_CLOCK_PIN PIND
#define BTN_CLOCK_Bit 7

static int get_BTN_DATA_state() {
	return (BTN_DATA_PIN & (1 << BTN_DATA_Bit)) ? 0 : 1;
}

static int get_BTN_CLOCK_state() {
	return (BTN_CLOCK_PIN & (1 << BTN_CLOCK_Bit)) ? 0 : 1;
}

int main(void) {
  LOGGER_init_USART();
	SHIFT_init();

  BTN_DATA_DDR &= ~(1 << BTN_DATA_Bit);
	INFO("Button DDR set\n");

	BTN_DATA_PORT |= (1 << BTN_DATA_Bit);
	INFO("Button PORT set to pull-down\n");

  BTN_CLOCK_DDR &= ~(1 << BTN_CLOCK_Bit);
	INFO("Button DDR set\n");

	BTN_CLOCK_PORT |= (1 << BTN_CLOCK_Bit);
	INFO("Button PORT set to pull-down\n");

	int BTN_CLOCK_state_old = get_BTN_CLOCK_state();
	int BTN_CLOCK_state = BTN_CLOCK_state_old;

	int push_int = 0;
	int debounce = 0;

  while (1) {
		int BTN_DATA_state = get_BTN_DATA_state();
		BTN_CLOCK_state = get_BTN_CLOCK_state();
		TRACE("Data BTN = %d, CLOCK BTN = %d\n", BTN_DATA_state, BTN_CLOCK_state);
		if (1 == BTN_DATA_state) {
			push_int = 1;
		} else {
			push_int = 0;
		}

		if (1 == BTN_CLOCK_state && 0 == BTN_CLOCK_state_old && 20 < debounce) {
			SHIFT_push(push_int);
			debounce = 0;
		}

		BTN_CLOCK_state_old = BTN_CLOCK_state;
		debounce += 1;
  }

  return 0;
}
