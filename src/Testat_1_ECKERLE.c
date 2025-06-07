// #define F_CPU 16E6
#include "iesusart.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

/********************
 * START DEBUG LOGIC
 ********************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_LEVEL_TRACE 0
#define DEBUG_LEVEL_INFO 1
#define DEBUG_LEVEL_WARNING 2
#define DEBUG_LEVEL_ERROR 3
#define DEBUG_LEVEL_FATAL 4
#define DEBUG_LEVEL_DISABLE 100

#define DEBUG_LEVEL DEBUG_LEVEL_DISABLE

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBDEBUG_LEVEL_ERROR
#endif /* ifndef DEBUG_LEVEL */

// Buffer for formatted debug messages
static char debug_buffer[256];

// Helper function for formatted debug output
static void debug_printf(const char *prefix, const char *format, ...) {
  va_list args;
  va_start(args, format);

  // Format the message
  snprintf(debug_buffer, sizeof(debug_buffer), "%s", prefix);
  vsnprintf(debug_buffer + strlen(debug_buffer),
            sizeof(debug_buffer) - strlen(debug_buffer), format, args);

  va_end(args);

  // Send to USART
  USART_print(debug_buffer);
}

// TRACE: Very detailed execution flow
#if DEBUG_LEVEL <= 0
#define TRACE(format, ...) debug_printf("[TRACE] ", format, ##__VA_ARGS__)
#else
#define TRACE(format, ...) ((void)0)
#endif

// INFO: General information
#if DEBUG_LEVEL <= 1
#define INFO(format, ...) debug_printf("[INFO] ", format, ##__VA_ARGS__)
#else
#define INFO(format, ...) ((void)0)
#endif

// WARNING: Something unusual but not critical
#if DEBUG_LEVEL <= 2
#define WARNING(format, ...) debug_printf("[WARN] ", format, ##__VA_ARGS__)
#else
#define WARNING(format, ...) ((void)0)
#endif

// ERROR: Something went wrong but program can continue
#if DEBUG_LEVEL <= 3
#define ERROR(format, ...) debug_printf("[ERROR] ", format, ##__VA_ARGS__)
#else
#define ERROR(format, ...) ((void)0)
#endif

// FATAL: Critical error, program should stop
#if DEBUG_LEVEL <= 4
#define FATAL(format, ...) debug_printf("[FATAL] ", format, ##__VA_ARGS__)
#else
#define FATAL(format, ...) ((void)0)
#endif

/********************
 * END DEBUG LOGIC
 ********************/

/********************
 * START SHIFT REGISTER LOGIC
 ********************/

#define SHIFT_DATA_DDR DDRB
#define SHIFT_DATA_PORT PORTB
#define SHIFT_DATA_PIN PINB
#define SHIFT_DATA_BIT PINB2

#define SHIFT_CLOCK_DDR DDRD
#define SHIFT_CLOCK_PORT PORTD
#define SHIFT_CLOCK_PIN PIND
#define SHIFT_CLOCK_BIT PIND4

#define SHIFT_HIGH 1
#define SHIFT_LOW 0

/**
 * @brief sets data pin high or low
 * @param value either SHIFT_HIGH or SHIFT_LOW, gets set into the pin
 * @return 0 on success
 * */
int _SHIFT_set_data_pin(unsigned int ui_value) {
  SHIFT_DATA_PORT &= ~(1 << SHIFT_DATA_BIT); // enshure data pin is zero

  // set data pin as needed
  if (SHIFT_HIGH == ui_value) {
    SHIFT_DATA_PORT |= (1 << SHIFT_DATA_BIT);
  }
  TRACE("Shift register Data set.\n");

  return 0;
}

/**
 * @brief cicles the shift register one time
 * @return 0 on success
 * */
int _SHIFT_cicle() {
  SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_BIT); // ensure clock pin is zero

  // toggle clock
  SHIFT_CLOCK_PORT |= (1 << SHIFT_CLOCK_BIT);
  SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_BIT);
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
  for (int i = ui_size - 1; i >= 0; i--) {
    int rc = SHIFT_push(uiarray_values[i]);
    if (rc != 0) {
      ERROR("SHIFT_push failed during state push\n");
      return 2;
    }
  }

  TRACE("State pushed to shift register\n");
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

/********************
 * START LINIENFOLGER LOGIC
 ********************/

#define LF_0_DDR DDRC
#define LF_0_PORT PORTC
#define LF_0_PIN PINC
#define LF_0_BIT PINC0

#define LF_1_DDR DDRC
#define LF_1_PORT PORTC
#define LF_1_PIN PINC
#define LF_1_BIT PINC1

#define LF_2_DDR DDRC
#define LF_2_PORT PORTC
#define LF_2_PIN PINC
#define LF_2_BIT PINC2

/**
 * @brief setup DDR and PORT of input pins
 * @return 0 on success
 */
int LF_init() {
	// set DDR as input
  LF_0_DDR &= ~(1 << LF_0_BIT);
  LF_1_DDR &= ~(1 << LF_1_BIT);
  LF_2_DDR &= ~(1 << LF_2_BIT);

	// set input as pull-up (so i need a HIGH to get a 1)
  LF_0_PORT |= (1 << LF_0_BIT);
  LF_1_PORT |= (1 << LF_1_BIT);
  LF_2_PORT |= (1 << LF_2_BIT);

  return 0;
}

/**
 * @brief gets the state of the line sensor of index
 * @param ui_lf_index is the index of the line sensor
 * @return the sensor state (0 or 1) on success, -1 on failure 
 * e.g. index not valid
 */
int LF_get_state(unsigned int ui_lf_index) {
  switch (ui_lf_index) {
  case 0:
    return (LF_0_PIN & (1 << LF_0_BIT)) ? 1 : 0;
  case 1:
    return (LF_1_PIN & (1 << LF_1_BIT)) ? 1 : 0;
  case 2:
    return (LF_2_PIN & (1 << LF_2_BIT)) ? 1 : 0;
  default:
    ERROR("Invalid line follower sensor index: %u\n", ui_lf_index);
    break;
  }
  return -1;
}

/**
 * @brief Reads all three line follower sensor states and stores them in output array
 * @param uiarray_output Pointer to array of 3 unsigned int elements [left, center, right]
 * @return 0 on success, 1 on sensor error, 2 on NULL pointer
 */
int LF_get_states(unsigned int *uiarray_output) {
  if (uiarray_output == NULL) {
    ERROR("NULL pointer passed to LF_get_states\n");
    return 2;
  }

  TRACE("Reading all line follower sensor states\n");

  int i_lf0_state = LF_get_state(0);
  int i_lf1_state = LF_get_state(1);
  int i_lf2_state = LF_get_state(2);

  TRACE("LF0 state: %i\n", i_lf0_state);
  TRACE("LF1 state: %i\n", i_lf1_state);
  TRACE("LF2 state: %i\n", i_lf2_state);

  // Validate all sensor readings
  if (i_lf0_state < 0 || i_lf1_state < 0 || i_lf2_state < 0) {
    ERROR("Reading line sensor states failed - sensor errors detected\n");
    return 1;
  }

  // Store valid results in output array
  uiarray_output[0] = (unsigned int)i_lf0_state;
  uiarray_output[1] = (unsigned int)i_lf1_state;
  uiarray_output[2] = (unsigned int)i_lf2_state;

  return 0;
}

/********************
 * END LINIENFOLGER LOGIC
 ********************/

int main(void) {

	// Initialize
	
	// init usart
  USART_init(UBRR_SETTING);
  INFO("USART working! Hooray!\n");

	// init shift register
  int rc = SHIFT_init();
  if (0 == rc) {
    INFO("Shift register DDR setup successful.\n");
  } else {
    ERROR("Shift register DDR setup FAILED!\n");
  }

	// init Line sensor
  rc = LF_init();
  if (0 == rc) {
    INFO("Line sensor DDR setup successful.\n");
  } else {
    ERROR("Line sensor DDR setup FAILED!\n");
  }

	// main loop

	// variables to detect state change to optimize runtime
  unsigned int ui_lf_state[3] = {0};
  unsigned int ui_lf_state_old_eigenvalue = ~(0);

  while (1) {

		// Update Inputs
		
    rc = LF_get_states(ui_lf_state);
    if (0 != rc) {
      WARNING("Sensor read failed\n");
			continue;
		}
		INFO("Sensors: [%u, %u, %u]\n", ui_lf_state[0], ui_lf_state[1], ui_lf_state[2]);

		// Run Logic
		
		// calculate new eigenvalue (check change)
		int ui_lf_state_eigenvalue = 0;
		for (int i = 0; 3 > i; i++) {
			ui_lf_state_eigenvalue |= ((1 << i) * (ui_lf_state[i]));
		}
		TRACE("Eigenvalues: (old)%u (new)%u\n", ui_lf_state_old_eigenvalue, ui_lf_state_eigenvalue);

		if (ui_lf_state_old_eigenvalue == ui_lf_state_eigenvalue) {
			continue;
		}
		ui_lf_state_old_eigenvalue = ui_lf_state_eigenvalue;
		INFO("Eigenvalues: (old)%u (new)%u\n", ui_lf_state_old_eigenvalue, ui_lf_state_eigenvalue);

		// Update Outputs

		INFO("Sensors: [%u, %u, %u]\n", ui_lf_state[0], ui_lf_state[1], ui_lf_state[2]);

		rc = SHIFT_push_state(ui_lf_state, 3);
		if (0 != rc) {
			ERROR("Failed to push states to shift register\n");
		}
  }

  return 0;
}
