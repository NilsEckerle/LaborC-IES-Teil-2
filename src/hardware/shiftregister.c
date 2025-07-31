#include "hardware/shiftregister.h"
#include "tools/bit_functions.h"
#include "tools/logger.h"
#include <stdint.h>
#include <util/delay.h>

void _SHIFT_set_data_pin(uint8_t ui_value) {
  _delay_us(1);
  UNSET_BIT(SHIFT_DATA_PORT, SHIFT_DATA_BIT);  // ensure data pin is zero

  // set data pin as needed
  if (SHIFT_HIGH == ui_value) {
    _delay_us(1);
    SET_BIT(SHIFT_DATA_PORT, SHIFT_DATA_BIT);
  }
  TRACE("Shift register Data set.\n");

  return;
}

void _SHIFT_cicle() {
  _delay_us(1);
  UNSET_BIT(SHIFT_CLOCK_PORT, SHIFT_CLOCK_BIT);  // ensure clock pin is zero
  _delay_us(1);

  // toggle clock
  SET_BIT(SHIFT_CLOCK_PORT, SHIFT_CLOCK_BIT);
  _delay_us(1);
  UNSET_BIT(SHIFT_CLOCK_PORT, SHIFT_CLOCK_BIT);
  TRACE("Shift register cicle send.\n");

  return;
}

void SHIFT_push(uint8_t ui_value) {
  uint8_t ui_value_to_push = ui_value % 2;

  // set data
  _SHIFT_set_data_pin(ui_value_to_push);

  // toggle clock
  _SHIFT_cicle();

  return;
}

void SHIFT_push_state(uint8_t bitstring_to_push) {
  for (int8_t i = 3; 0 < i; i--) {  // itterate over last 3 bits
    uint8_t mask = (1 << (i - 1));
    uint8_t masked_bitstring = bitstring_to_push & mask;

    if (masked_bitstring > 0) {
      SHIFT_push(1);
    } else {
      SHIFT_push(0);
    }
  }

  return;
}

void SHIFT_init() {
  // set as output
  SET_BIT(SHIFT_DATA_DDR, SHIFT_DATA_BIT);
  SET_BIT(SHIFT_CLOCK_DDR, SHIFT_CLOCK_BIT);
  TRACE("Shift register setup DDR of data and clock\n");

  // set default as low
  UNSET_BIT(SHIFT_DATA_PORT, SHIFT_DATA_BIT);
  UNSET_BIT(SHIFT_CLOCK_PORT, SHIFT_CLOCK_BIT);
  TRACE("Shift register initialized PORT of data and clock to low\n");

  SHIFT_push_state(0);
  INFO("Shift register initialized and 000 send to it.\n");

  return;
}
