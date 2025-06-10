#include <avr/io.h>
#include <util/delay.h>
#include "logger.h"

/********************
 * START SHIFT REGISTER LOGIC
 ********************/

#define SHIFT_DATA_DDR DDRB
#define SHIFT_DATA_PORT PORTB
#define SHIFT_DATA_PIN PINB
#define SHIFT_DATA_BIT 2

#define SHIFT_CLOCK_DDR DDRD
#define SHIFT_CLOCK_PORT PORTD
#define SHIFT_CLOCK_PIN PIND
#define SHIFT_CLOCK_BIT 4

#define SHIFT_HIGH 1
#define SHIFT_LOW 0

/**
 * @brief sets data pin high or low
 * @param value either SHIFT_HIGH or SHIFT_LOW, gets set into the pin
 * @return 0 on success
 * */
static int _SHIFT_set_data_pin(unsigned int ui_value) {
  // set data pin as needed
  if (SHIFT_HIGH == ui_value) {
    SHIFT_DATA_PORT |= (1 << SHIFT_DATA_BIT);
  } else {
    SHIFT_DATA_PORT &= ~(1 << SHIFT_DATA_BIT);
	}
	_delay_us(1);
  TRACE("Shift register data PORT set to %u.\n", (SHIFT_HIGH == ui_value) ? SHIFT_HIGH : SHIFT_LOW);

  return 0;
}

/**
 * @brief cicles the shift register one time
 * @return 0 on success
 * */
int _SHIFT_cicle() {
  SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_BIT);

  SHIFT_CLOCK_PORT |= (1 << SHIFT_CLOCK_BIT);
	_delay_us(1);
  SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_BIT);
	_delay_us(1);
  TRACE("Shift register cicle send.\n");


  return 0;
}

/**
 * @brief pushes one bit into the shift register
 * on false input the modolo 2 of the number is pushed
 * @param value either SHIFT_HIGH or SHIFT_LOW
 * @return
 * 0 on success
 * 1 on set data error
 * 2 on send clock cicle error
 * */
int SHIFT_push(unsigned int ui_value) {
  unsigned int ui_value_to_push = ui_value % 2;

  // set data
  int rc = _SHIFT_set_data_pin(ui_value_to_push);
  if (0 != rc) {
    ERROR("Shift register set data pin failed.\n");
    return 1;
  }

  // toggle clock
  rc = _SHIFT_cicle();
  if (0 != rc) {
    ERROR("Shift register cicle failed\n");
    return 2;
  }
	TRACE("Shift register date %u pushed.\n", ui_value_to_push);
	_delay_us(1);

  return 0;
}

/**
 * @brief Pushes an array of values to the shift register in reverse order
 * @param uiarrayp_values Pointer to array of unsigned int values
 * (SHIFT_HIGH/SHIFT_LOW)
 * @param ui_size Number of elements in the array (must be > 0)
 * @return
 * 0 on success
 * 1 on invalid parameters (NULL pointer or zero size)
 * 2 on SHIFT_push failure
 */
int SHIFT_push_state(unsigned int *uiarray_values, unsigned int ui_size) {
  if (uiarray_values == NULL || ui_size == 0) {
    ERROR("Invalid parameters for SHIFT_push_state\n");
    return 1;
  }

  // Iterate backwards over array
	TRACE("State [");
  for (int i = ui_size - 1; i >= 0; i--) {
    int rc = SHIFT_push(uiarray_values[i]);
    if (rc != 0) {
      ERROR("SHIFT_push failed during state push\n");
      return 2;
    }
		TRACE("%u,", uiarray_values[i]);
  }
  TRACE("] pushed to shift register\n");

  return 0;
}

/**
 * @brief Initializes DDR and Port of the shift register
 * @return 0 on success
 * */
int SHIFT_init() {
  // set as output
  SHIFT_DATA_DDR |= (1 << SHIFT_DATA_BIT);
  SHIFT_CLOCK_DDR |= (1 << SHIFT_CLOCK_BIT);
  TRACE("Shift register setup DDR of data and clock\n");

  // set default as low
  SHIFT_DATA_PORT &= ~(1 << SHIFT_DATA_BIT);
  SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_BIT);
  TRACE("Shift register initialized PORT of data and clock to low\n");

  unsigned int uiarray_low[3] = {SHIFT_LOW, SHIFT_LOW, SHIFT_LOW};
  int rc = SHIFT_push_state(uiarray_low, 3);
  if (0 != rc) {
    ERROR("Shift register init failed");
    return 1;
  }

  return 0;
}

/********************
 * END SHIFT REGISTER LOGIC
 ********************/
