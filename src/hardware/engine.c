#include "hardware/engine.h"
#include "logger.h"

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

 void ENGINE_drive(ENGINE_drive_direction direction){
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

		case ENGINE_UNDEFINED:
			break;
	}

	return;
}

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
