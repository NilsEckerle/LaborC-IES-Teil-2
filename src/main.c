/* DISCLAIMER
 * a lot of comment documentation is generated with 
 * Claude Sonnet 4 (https://claude.ai/) and was verifyed 
 * and corrected by a human. Please don't ever push 
 * something you don't understand created by an AI.
 * -Nils Eckerle 2025-06-20 
 */

// #define F_CPU 16E6
#include "iesusart.h"
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>
#include "logger.h"


/********************
 * START LINIENFOLGER LOGIC
 ********************/

/**
 * @brief Enumeration representing line detection states from a 3-sensor line follower
 * 
 * This enum defines the possible states detected by a line following sensor array
 * consisting of three sensors arranged as Left (L), Middle (M), and Right (R).
 * Each state represents which combination of sensors detect a line.
 * 
 * The sensors are mapped as follows:
 * - L (Left): Sensor 0 (LF_0)
 * - M (Middle): Sensor 1 (LF_1) 
 * - R (Right): Sensor 2 (LF_2)
 * 
 * @note LF_LR represents an edge case where only left and right sensors detect
 *       a line simultaneously, which may indicate a wide line, intersection,
 *       or sensor malfunction.
 *       Enum member values got chosen for its bit representation to work with 
 *       the shift register (last 3 bit representing Left Middle Right in this order).
 */
typedef enum { 
	LF_UNDEFINED = 8,  /**< Invalid/uninitialized state or sensor read error */
	LF_NONE = 0,       /**< No sensors detect a line (000) */
	LF_LMR = 7,        /**< All sensors detect a line (111) - wide line or intersection */
	LF_LR = 5,         /**< Left and right sensors detect a line (101) - edge case */
	LF_L = 4,          /**< Only left sensor detects a line (100) */
	LF_LM = 6,         /**< Left and middle sensors detect a line (110) */
	LF_M = 2,          /**< Only middle sensor detects a line (010) - centered on line */
	LF_MR = 3,         /**< Middle and right sensors detect a line (011) */
	LF_R = 1           /**< Only right sensor detects a line (001) */
} LF_detection_state;


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
 * @return nothing, this function can't fail
 */
void LF_init() {
	// set DDR as input
  LF_0_DDR &= ~(1 << LF_0_BIT);
  LF_1_DDR &= ~(1 << LF_1_BIT);
  LF_2_DDR &= ~(1 << LF_2_BIT);

	// set input as pull-up (so i need a HIGH to get a 1)
  LF_0_PORT |= (1 << LF_0_BIT);
  LF_1_PORT |= (1 << LF_1_BIT);
  LF_2_PORT |= (1 << LF_2_BIT);

  return;
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

  return -1; // return error
}

/**
 * @brief converts a bitstring of 3 bits (starting at LSB) to a LF_detection_state
 * @return the converted state
 */
LF_detection_state LF_bitstring_to_state(unsigned int ui_lf_detection_bitstring) {
  // sanitize input
	unsigned int mask = 0;
	mask |= (1 << 0);
	mask |= (1 << 1);
	mask |= (1 << 2);
	unsigned int ui_cleaned_lf_detection_bitstring = ui_lf_detection_bitstring & mask;

	// a lot of magic numbers representing the different bit strings 
	// e.g. 6 = b_110 = left & middle
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
 * @return LF_detection_state
 */
LF_detection_state LF_get_states() {
	// get new sensor readings
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
    return (LF_detection_state)LF_UNDEFINED;
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

// Forward delay configuration
#if DEBUG_LEVEL <= DEBUG_LEVEL_INFO
#define LMR_FORWARD_DELAY_IN_ITTERATIONS 1
#else
#define LMR_FORWARD_DELAY_IN_ITTERATIONS 20000
#endif

// H-Bridge Enable pins
#define ENGINE_HB_ENA_DDR DDRD
#define ENGINE_HB_ENA_PORT PORTD
#define ENGINE_HB_ENA_PIN PIND
#define ENGINE_HB_ENA_BIT 5

#define ENGINE_HB_ENB_DDR DDRD
#define ENGINE_HB_ENB_PORT PORTD
#define ENGINE_HB_ENB_PIN PIND
#define ENGINE_HB_ENB_BIT 6

// H-Bridge direction control pins
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

/**
 * @brief Enumeration representing robot movement directions
 * 
 * This enum defines the possible movement states with left and right motor 
 * pairs controlled via H-bridge circuits.
 * 
 * Motor control mapping:
 * - Left motors: controlled by ENA, IN1, IN2
 * - Right motors: controlled by ENB, IN3, IN4
 * 
 * Movement behaviors:
 * - FORWARD/BACKWARD: Both motor pairs move in same direction
 * - LEFT/RIGHT: One motor pair stops, other continues (gentle turn)
 * - HARD_LEFT/HARD_RIGHT: Motor pairs move in opposite directions (sharp turn)
 */
typedef enum { 
	ENGINE_UNDEFINED,   /**< Invalid/uninitialized state */
	ENGINE_STOP,        /**< Both motor pairs stopped */
	ENGINE_BACKWARDS,   /**< Both motor pairs reverse direction */
	ENGINE_FORWARD,     /**< Both motor pairs forward direction */
	ENGINE_HARD_LEFT,   /**< Left motors reverse, right motors forward (sharp left) */
	ENGINE_LEFT,        /**< Left motors stop, right motors forward (gentle left) */
	ENGINE_HARD_RIGHT,  /**< Left motors forward, right motors reverse (sharp right) */
	ENGINE_RIGHT        /**< Left motors forward, right motors stop (gentle right) */
} ENGINE_drive_direction;

/**
 * @brief Initializes motor control system
 * 
 * Sets up pins for H-bridge motor control and enables both motor pairs.
 * Configures all control pins as outputs and enables motor power.
 * 
 * @return nothing, this function can't fail.
 * 
 * @note After initialization, motors are enabled but stopped (direction pins not set)
 */
void ENGINE_init() {
	// Configure all H-bridge control pins as outputs
	ENGINE_HB_ENA_DDR |= (1 << ENGINE_HB_ENA_BIT);  // Left motor enable
	ENGINE_HB_ENB_DDR |= (1 << ENGINE_HB_ENB_BIT);  // Right motor enable
  ENGINE_HB_IN1_DDR |= (1 << ENGINE_HB_IN1_BIT);  // Left motor direction 1
  ENGINE_HB_IN2_DDR |= (1 << ENGINE_HB_IN2_BIT);  // Left motor direction 2
  ENGINE_HB_IN3_DDR |= (1 << ENGINE_HB_IN3_BIT);  // Right motor direction 1
  ENGINE_HB_IN4_DDR |= (1 << ENGINE_HB_IN4_BIT);  // Right motor direction 2

	// Enable both motor pairs (set enable pins high)
	ENGINE_HB_ENA_PORT |= (1 << ENGINE_HB_ENA_BIT); // Enable left motors
	ENGINE_HB_ENB_PORT |= (1 << ENGINE_HB_ENB_BIT); // Enable right motors
	
	return;
}

/**
 * @brief Controls robot movement direction
 * 
 * Sets H-bridge control pins to achieve the specified movement direction.
 * Uses differential drive logic where left and right motor pairs can be
 * controlled independently for steering.
 * 
 * @param direction The desired movement direction from ENGINE_drive_direction enum
 * @return nothing, this function can't fail.
 * 
 * @note H-bridge control logic:
 *       - Forward: IN1=1, IN2=0 (left), IN3=0, IN4=1 (right)
 *       - Reverse: IN1=0, IN2=1 (left), IN3=1, IN4=0 (right)
 *       - Stop: Both direction pins low
 */
 void ENGINE_drive(ENGINE_drive_direction direction){
	INFO("");
	switch (direction) {
		case ENGINE_STOP:
			INFO("Robi is: STOP\n");
			// Stop left motors
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// Stop right motors
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_BACKWARDS:
			INFO("Robi is: BACKWARDS\n");
			// Left motors backwards
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);
			// Right motors backwards
			ENGINE_HB_IN3_PORT |= (1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_FORWARD:
			INFO("Robi is: FORWARD\n");
			// Left motors forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// Right motors forward
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_HARD_LEFT:
			INFO("Robi is: HARD_LEFT\n");
			// Left motors backwards
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);
			// Right motors forward
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_LEFT:
			INFO("Robi is: LEFT\n");
			// Left motors stop
			ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// Right motors forward
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_HARD_RIGHT:
			INFO("Robi is: HARD_RIGHT\n");
			// Left motors forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// Right motors backwards
			ENGINE_HB_IN3_PORT |= (1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
			
		case ENGINE_RIGHT:
			INFO("Robi is: RIGHT\n");
			// Left motors forward
			ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
			ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
			// Right motors stop
			ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
			ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
			break;
	}

	return;
}

/**
 * @brief Line follower logic for autonomous robot movement
 * 
 * Implements line following behavior by converting line sensor states into
 * appropriate motor commands. Includes special handling for start/end
 * and error conditions.
 * 
 * @param new_lf_state Current line follower sensor state
 * @param old_lf_state Previous line follower sensor state  
 * @param LMR_itterations_since_entry Pointer to counter for LMR state duration
 * @return 0 on success, -1 on fatal error conditions
 * 
 * @note Movement logic:
 *       - LF_NONE: Back up to find line
 *       - LF_M: Go straight (centered on line)
 *       - LF_L/LF_R: Sharp turns to correct course
 *       - LF_LM/LF_MR: Gentle turns to correct course
 *       - LF_LMR: start/end handling with delay
 *       - LF_LR/LF_UNDEFINED: Error states, stop robot
 */
int ENGINE_drive_logic(
		LF_detection_state new_lf_state, 
		LF_detection_state old_lf_state, 
		unsigned int *LMR_itterations_since_entry
		){
	// Optimize performance: only change direction when sensor state changes
	// Exception: LF_LMR always processes (intersection handling)
	if (new_lf_state == old_lf_state 									// No state change
			&& new_lf_state != (LF_detection_state)LF_LMR // Not an intersection
			) {
		return 0; // No action needed
	}

	// Convert line sensor state to motor command
	switch (new_lf_state){
		case (LF_detection_state)LF_NONE:
			// Lost line - back up to reacquire
			ENGINE_drive((ENGINE_drive_direction)ENGINE_BACKWARDS);
			break;
			
		case (LF_detection_state)LF_LMR:
			// start/end detected - controlled forward movement with delay
			if (*LMR_itterations_since_entry > LMR_FORWARD_DELAY_IN_ITTERATIONS){
				// Delay exceeded, stop at start/end
				ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
				return 0;
			}
			// Continue forward through start/end
			ENGINE_drive((ENGINE_drive_direction)ENGINE_FORWARD);
			*LMR_itterations_since_entry += 1; // Increment delay counter
			return 0;
			
		case (LF_detection_state)LF_L:
			// Line detected only on left - sharp left turn
			ENGINE_drive((ENGINE_drive_direction)ENGINE_HARD_LEFT);
			break;
			
		case (LF_detection_state)LF_LM:
			// Line on left and middle - gentle left correction
			ENGINE_drive((ENGINE_drive_direction)ENGINE_LEFT);
			break;
			
		case (LF_detection_state)LF_M:
			// Line centered - go straight
			ENGINE_drive((ENGINE_drive_direction)ENGINE_FORWARD);
			break;
			
		case (LF_detection_state)LF_MR:
			// Line on middle and right - gentle right correction
			ENGINE_drive((ENGINE_drive_direction)ENGINE_RIGHT);
			break;
			
		case (LF_detection_state)LF_R:
			// Line detected only on right - sharp right turn
			ENGINE_drive((ENGINE_drive_direction)ENGINE_HARD_RIGHT);
			break;
			
		case (LF_detection_state)LF_LR:
			// Error: Left and right sensors active, middle inactive
			// This should not occur in normal line following
			ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
			FATAL("ENGINE_drive_logic LR should not be possible!\n");
			return -1;
			
		case (LF_detection_state)LF_UNDEFINED:
			// Error: Invalid sensor state
			ENGINE_drive((ENGINE_drive_direction)ENGINE_STOP);
			FATAL("ENGINE_drive_logic UNDEFINED should not be possible!\n");
			return -1;
	}

	// Reset start/end counter for non-LMR states
	*LMR_itterations_since_entry = 0;
	return 0;
}

/********************
 * END MOTOREN LOGIC
 ********************/

/********************
 * START MAIN LOGIC 
 ********************/

/**
 * @brief Initializes all robot subsystems in proper sequence
 * 
 * Performs sequential initialization of all robot components including:
 * - USART communication system
 * - Shift register for LED status display
 * - Line follower sensors
 * - Motor control system
 * 
 * Each subsystem is initialized and validated before proceeding to the next.
 * If any initialization fails, the function returns immediately with error code.
 * 
 * @return 0 on successful initialization of all components
 * @return 1 if any component initialization fails
 * 
 * @note Initialization order is important - USART must be first for error reporting
 */
static int init_robi() {
	// Initialize USART communication (required first for debug output)
  USART_init(UBRR_SETTING);
	INFO("USART init successful.\n");
  
	// Initialize shift register for LED status display
  SHIFT_init();
	INFO("Shift register init successful.\n");
  
	// Initialize line follower sensor array
  LF_init();
	INFO("Line sensor init successful.\n");
  
	// Initialize motor control system
  ENGINE_init();
	INFO("Engine init successful.\n");
  
	return 0; // All systems initialized successfully
}

/**
 * @brief drive robi forward over start/end
 */
static void start_robi() {
	ENGINE_drive((ENGINE_drive_direction)ENGINE_FORWARD);
	return;
}

/**
 * @brief Main program entry point and control loop
 * 
 * Implements the main robot behavior loop.
 * The loop runs continuously until power loss or system reset.
 * If initialization fails, the program enters an error state with
 * continuous error messaging.
 * 
 * @return 0 (never reached due to infinite loop)
 * 
 * @note Performance optimization: LED updates and motor commands only 
 *       execute when sensor state changes to reduce processing overhead
 */
int main(void) {
	// Initialize all robot subsystems
	int rc = init_robi();
	
	// Handle initialization failure - enter safe error state
	if (0 != rc) { 
		// Infinite error loop prevents undefined behavior
		// Continuous error output helps with debugging
		while (1) {
			FATAL("Roboter initialization failed. Please fix and flash new firmware.\n");
			_delay_ms(1000);
		}
	}
	
  INFO("Robi initialized! Hooray!\n");
  
	// Main control loop variables
	// State change detection for performance optimization
  LF_detection_state lf_state_old = (LF_detection_state)LF_UNDEFINED;
  
	// Intersection delay counter for LMR state handling
	unsigned int LMR_delay = 0;

	unsigned int ui_robi_has_moved = 0;
	
  // Infinite main control loop
  while (1) {
		/*
		 * INPUT PROCESSING PHASE
		 */
		
		// Read current line follower sensor state
    LF_detection_state lf_state_current = LF_get_states();
    if ((LF_detection_state)LF_UNDEFINED == lf_state_current) {
      WARNING("Sensor read failed\n");
      // Continue execution with undefined state - error handling in motor logic
		}
		INFO_SPAM("Sensors state: %d\n", lf_state_current);
		
		/*
		 * CONTROL LOGIC PHASE
		 */
		
		if (ui_robi_has_moved) { // TODO: remove this auto start logic
			// Execute line following motor control logic
			// Handles all movement decisions based on sensor input
			ENGINE_drive_logic(lf_state_current, lf_state_old, &LMR_delay);
		} else {
			start_robi();
		}
		
		
		// Handle USART communication - echo any received data
		if (UCSR0A & (1 << RXC0)) { // Check if data available in receive buffer
			char received_byte = USART_receiveByte();
			USART_transmitByte(received_byte); // Echo back to sender
		}

		/*
		 * OUTPUT UPDATE PHASE
		 */
		
		// Update LED status display (only when state changes for efficiency and preventing flashing LED)
		if (lf_state_old != lf_state_current) { 
			if (LF_UNDEFINED != lf_state_old) { // TODO: remove this auto start logic
				ui_robi_has_moved = 1;
			}
			rc = SHIFT_push_state(lf_state_current);
			if (0 != rc) {
				WARNING("Failed to push states to shift register\n");
				// Non-critical - continue operation without LED updates
			}
		}
		
		// Update state tracking for next iteration
		lf_state_old = lf_state_current;
  }
  
  return 0; // Never reached due to infinite loop
}

/********************
 * END MAIN LOGIC 
 ********************/
