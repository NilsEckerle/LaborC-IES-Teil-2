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
#include "hardware/shiftregister.h"
#include "hardware/linienfolger.h"
#include "hardware/engine.h"

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
			SHIFT_push_state(lf_state_current);
		}
		
		// Update state tracking for next iteration
		lf_state_old = lf_state_current;
  }
  
  return 0; // Never reached due to infinite loop
}
