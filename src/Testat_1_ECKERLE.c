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

#define DEBUG_LEVEL DEBUG_LEVEL_INFO

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


typedef enum { 
	LF_UNDEFINED,
	LF_NONE,
	LF_LMR,
	LF_LR,
	LF_L,
	LF_LM,
	LF_M,
	LF_MR,
	LF_R
} LF_detection_state;

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
int SHIFT_push_state(LF_detection_state lf_state) {
  if ((LF_detection_state)LF_UNDEFINED == lf_state) {
    ERROR("Invalid linienfolger state for SHIFT_push_state\n");
    return 1;
  }

	switch (lf_state) {
		case (LF_detection_state)LF_NONE:
			SHIFT_push(0);
			SHIFT_push(0);
			SHIFT_push(0);
			break;
		case (LF_detection_state)LF_LMR:
			SHIFT_push(1);
			SHIFT_push(1);
			SHIFT_push(1);
			break;
		case (LF_detection_state)LF_L:
			SHIFT_push(1);
			SHIFT_push(0);
			SHIFT_push(0);
			break;
		case (LF_detection_state)LF_LM:
			SHIFT_push(1);
			SHIFT_push(1);
			SHIFT_push(0);
			break;
		case (LF_detection_state)LF_M:
			SHIFT_push(0);
			SHIFT_push(1);
			SHIFT_push(0);
			break;
		case (LF_detection_state)LF_MR:
			SHIFT_push(0);
			SHIFT_push(1);
			SHIFT_push(1);
			break;
		case (LF_detection_state)LF_R:
			SHIFT_push(0);
			SHIFT_push(0);
			SHIFT_push(1);
			break;
		case (LF_detection_state)LF_LR:
			SHIFT_push(1);
			SHIFT_push(0);
			SHIFT_push(1);
			break;
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

  int rc = SHIFT_push_state((LF_detection_state)LF_NONE);
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

LF_detection_state LF_bitstring_to_state(unsigned int ui_lf_detection_bitstring) {
  // sanitize input
	unsigned int mask = 0;
	mask |= (1 << 0);
	mask |= (1 << 1);
	mask |= (1 << 2);
	unsigned int ui_cleaned_lf_detection_bitstring = ui_lf_detection_bitstring & mask;

	// a lot of magic numbers representing the different bit strings
	switch (ui_cleaned_lf_detection_bitstring) {
		case 0: // no lf sees line
			return (LF_detection_state)LF_NONE;
		case 1: // right lf sees line
			return (LF_detection_state)LF_R;
		case 2: // middle lf sees line
			return (LF_detection_state)LF_M;
		case 3: // middle, right lf sees line
			return (LF_detection_state)LF_MR;
		case 4: // left lf sees line
			return (LF_detection_state)LF_L;
		case 5: // left, right lf sees line (not possible)
			return (LF_detection_state)LF_LR;
		case 6: // left, middle lf sees line
			return (LF_detection_state)LF_LM;
		case 7: // left, middle, right lf sees line
			return (LF_detection_state)LF_LMR;
	}

	return (LF_detection_state)LF_UNDEFINED;
}

/**
 * @brief Reads all three line follower sensor states and stores them in output array
 * @param uiarray_output Pointer to array of 3 unsigned int elements [left, center, right]
 * @return 0 on success, 1 on sensor error, 2 on NULL pointer
 */
LF_detection_state LF_get_states() {
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
	unsigned int lf_state_bitstring = 0;
  lf_state_bitstring |= (i_lf0_state << 0);
  lf_state_bitstring |= (i_lf1_state << 1);
  lf_state_bitstring |= (i_lf2_state << 2);

  return LF_bitstring_to_state(lf_state_bitstring);
}

/********************
 * END LINIENFOLGER LOGIC
 ********************/


/********************
 * START MOTOREN LOGIC
 ********************/

#define LMR_FORWARD_DELAY_IN_ITTERATIONS 1

#define ENGINE_HB_ENA_DDR DDRD
#define ENGINE_HB_ENA_PORT PORTD
#define ENGINE_HB_ENA_PIN PIND
#define ENGINE_HB_ENA_BIT 5

#define ENGINE_HB_ENB_DDR DDRD
#define ENGINE_HB_ENB_PORT PORTD
#define ENGINE_HB_ENB_PIN PIND
#define ENGINE_HB_ENB_BIT 6

#define ENGINE_HB_IN1_DDR DDRD
#define ENGINE_HB_IN1_PORT PORTD
#define ENGINE_HB_IN1_PIN PIND
#define ENGINE_HB_IN1_BIT 7

#define ENGINE_HB_IN2_DDR DDRB
#define ENGINE_HB_IN2_PORT PORTB
#define ENGINE_HB_IN2_PIN PINB
#define ENGINE_HB_IN2_BIT 0

#define ENGINE_HB_IN3_DDR DDRB
#define ENGINE_HB_IN3_PORT PORTB
#define ENGINE_HB_IN3_PIN PINB
#define ENGINE_HB_IN3_BIT 1

#define ENGINE_HB_IN4_DDR DDRB
#define ENGINE_HB_IN4_PORT PORTB
#define ENGINE_HB_IN4_PIN PINB
#define ENGINE_HB_IN4_BIT 3

typedef enum { 
	ENGINE_UNDEFINED,
	ENGINE_STOP,
	ENGINE_BACKWARDS,
	ENGINE_FORWARD,
	ENGINE_HARD_LEFT,
	ENGINE_LEFT,
	ENGINE_HARD_RIGHT,
	ENGINE_RIGHT
} ENGINE_drive_direction;

int ENGINE_init() {
	// set DDR a output
	ENGINE_HB_ENA_DDR |= (1 << ENGINE_HB_ENA_BIT);
	ENGINE_HB_ENB_DDR |= (1 << ENGINE_HB_ENB_BIT);
  ENGINE_HB_IN1_DDR |= (1 << ENGINE_HB_IN1_BIT);
  ENGINE_HB_IN2_DDR |= (1 << ENGINE_HB_IN2_BIT);
  ENGINE_HB_IN3_DDR |= (1 << ENGINE_HB_IN3_BIT);
  ENGINE_HB_IN4_DDR |= (1 << ENGINE_HB_IN4_BIT);

	// enable engines
	ENGINE_HB_ENA_PORT |= (1 << ENGINE_HB_ENA_BIT); // left engines
	ENGINE_HB_ENB_PORT |= (1 << ENGINE_HB_ENB_BIT); // right engines
	
	return 0;
}

int ENGINE_drive(ENGINE_drive_direction direction){
	INFO("");
	switch (direction) {
		case ENGINE_STOP:
			INFO("Robi is: STOP\n");
			// left engines stop
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// right engines stop
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_BACKWARDS:
			INFO("Robi is: BACKWARDS\n");
			// left engines backwards
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);
			// right engines backwards
			ENGINE_HB_IN3_PORT |= (1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_FORWARD:
			INFO("Robi is: FORWARD\n");
			// left engines forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// right engines forward
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_HARD_LEFT:
			INFO("Robi is: HARD_LEFT\n");
			// left engines backwards
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);
			// right engines forwards
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_LEFT:
			INFO("Robi is: LEFT\n");
			// left engines stop
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// right engines forwards
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_HARD_RIGHT:
			INFO("Robi is: HARD_RIGHT\n");
			// left engines forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// right engines Backwards
			ENGINE_HB_IN3_PORT |= (1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
		case ENGINE_RIGHT:
			INFO("Robi is: RIGHT\n");
			// left engines forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// right engines stop
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
	}
	return 0;
}

int ENGINE_drive_logic(LF_detection_state new_lf_state, LF_detection_state old_lf_state, unsigned int *LMR_itterations_since_entry){
	// change drive state only when LF has detected changes
	if (new_lf_state == old_lf_state 									// check change
			&& new_lf_state != (LF_detection_state)LF_LMR // if LF_LMR then it shold go anyway
			) {
		return 0;
	}

	switch (new_lf_state){
		case (LF_detection_state)LF_NONE:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_BACKWARDS);
			break;
		case (LF_detection_state)LF_LMR:
			if (*LMR_itterations_since_entry > LMR_FORWARD_DELAY_IN_ITTERATIONS){
				ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
				return 0;
			}
			ENGINE_drive((ENGINE_drive_direction)ENGINE_FORWARD);
			*LMR_itterations_since_entry += 1;
			return 0;
		case (LF_detection_state)LF_L:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_HARD_LEFT);
			break;
		case (LF_detection_state)LF_LM:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_LEFT);
			break;
		case (LF_detection_state)LF_M:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_FORWARD);
			break;
		case (LF_detection_state)LF_MR:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_RIGHT);
			break;
		case (LF_detection_state)LF_R:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_HARD_RIGHT);
			break;
		case (LF_detection_state)LF_LR:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
			FATAL("ENGINE_drive_logic LR should not be possible!\n");
			return -1;
		case (LF_detection_state)LF_UNDEFINED:
			ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
			FATAL("ENGINE_drive_logic UNDEFINED should not be possible!\n");
			return -1;
	}

	*LMR_itterations_since_entry = 0;
	return 0;
}

/********************
 * END MOTOREN LOGIC
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

	// init Engines sensor
  rc = ENGINE_init();
  if (0 == rc) {
    INFO("Engine DDR setup successful.\n");
  } else {
    ERROR("Engine DDR setup FAILED!\n");
  }

	// main loop

	// variables to detect state change to optimize runtime
  LF_detection_state lf_state_old = (LF_detection_state)LF_UNDEFINED;

	// variable to delay stopping when hitting LMR with LF sensor
	unsigned int LMR_delay = 0;

  while (1) {

		// Update Inputs
		
    LF_detection_state lf_state_current = LF_get_states();
    if ((LF_detection_state)LF_UNDEFINED == lf_state_current) {
      WARNING("Sensor read failed\n");
		}
		INFO("Sensors state: %d\n", lf_state_current);

		// Run Logic

		// Controll Motors

		ENGINE_drive_logic(lf_state_current, lf_state_old, &LMR_delay);

		// Update LED

		// change shift register only when LF has detected changes
		if (lf_state_old != lf_state_current) {
			rc = SHIFT_push_state(lf_state_current);
			if (0 != rc) {
				ERROR("Failed to push states to shift register\n");
			}
		}

		// Update lf_state_old
		lf_state_old = lf_state_current;
  }

  return 0;
}
