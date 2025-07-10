#include "hardware/engine.h"
#include "tools/bit_functions.h"
#include "tools/logger.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Initializes motor pulse width modulation (pwm) system
 *
 * Sets up pins for H-bridge pwm control.
 *
 * @return nothing, this function can't fail.
 *
 * @note This function is coppied from iesmotors.c
 */
static void ENGINE_init_pwm() {
  // Disable all interrupts
  cli();
  // Set prescaler to 64, cf. datasheet for TCCR0B
  // (TCCR0B: Timer/Counter Control Register 0 B)
  TCCR0B = 0;
  SET_BIT(TCCR0B, CS00);
  SET_BIT(TCCR0B, CS01);
  // Set waveform generation mode to Fast PWM, frequency = F_CPU / (PRESCALER *
  // 2^8)
  TCCR0A = 0;
  SET_BIT(TCCR0A, WGM00);
  SET_BIT(TCCR0A, WGM01);
  // Re-enable all interrupts
  sei();
}

void ENGINE_init() {
  // Configure all H-bridge control pins as outputs
  SET_BIT(ENGINE_HB_ENA_DDR, ENGINE_HB_ENA_BIT); // Left motor enable
  SET_BIT(ENGINE_HB_ENB_DDR, ENGINE_HB_ENB_BIT); // Right motor enable
  SET_BIT(ENGINE_HB_IN1_DDR, ENGINE_HB_IN1_BIT); // Left motor direction 1
  SET_BIT(ENGINE_HB_IN2_DDR, ENGINE_HB_IN2_BIT); // Left motor direction 2
  SET_BIT(ENGINE_HB_IN3_DDR, ENGINE_HB_IN3_BIT); // Right motor direction 1
  SET_BIT(ENGINE_HB_IN4_DDR, ENGINE_HB_IN4_BIT); // Right motor direction 2

  // Enable both motor pairs (set enable pins high)
  SET_BIT(ENGINE_HB_ENA_PORT, ENGINE_HB_ENA_BIT); // Enable left motors
  SET_BIT(ENGINE_HB_ENB_PORT, ENGINE_HB_ENB_BIT); // Enable right motors

  ENGINE_init_pwm();
  INFO("ENGINE pwm init successful.\n");

  ENGINE_set_duty_cicle(ENGINE_LEFT, ~0);  // set to full power
  ENGINE_set_duty_cicle(ENGINE_RIGHT, ~0); // set to full power

  return;
}

// void setDutyCycle(uint8_t pin, uint8_t value)
//  This function is coppied from iesmotors.c and then modified
void ENGINE_set_duty_cicle(uint8_t ui8_side, uint8_t ui8_pwm_compare_value) {
  // TODO:
  // Suggestion to handle PD6 - note the code-clones wrt. PD5 below!
  // Code-clones are extraordinary f cky! Correct this (tricky though
  // due to the PP-macros, which you cannot simply pass to functions)!
  // (But PP-macros can help you here....)

  TRACE("set_duty_cicle side: %s value %d\n", ui8_side == 0 ? "LEFT" : "RIGHT",
        ui8_pwm_compare_value);

  if (ui8_side == ENGINE_PWM_LEFT) {
    if (ui8_pwm_compare_value == 0) {
      UNSET_BIT(TCCR0A, COM0A1);
      UNSET_BIT(TCCR0A, COM0A0); // Normal port operation mode
      UNSET_BIT(PORTD, PD6);     // PD6 LOW, equals 0% duty,
    } // timer disconnected
    else if (ui8_pwm_compare_value == 255) {
      UNSET_BIT(TCCR0A, COM0A1);
      UNSET_BIT(TCCR0A, COM0A0); // Normal port operation mode
      SET_BIT(PORTD, PD6);       // PD6 HIGH, equals 100% duty,
    } // timer disconnected
    else {
      SET_BIT(TCCR0A, COM0A1);       // OC0A to LOW on Compare Match,
      UNSET_BIT(TCCR0A, COM0A0);     // to HIGH at BOTTOM (non-inverting mode)
      OCR0A = ui8_pwm_compare_value; // generates sequences of 1-0-1-0...
    } // for certain periods of time
  } else if (ui8_side == ENGINE_PWM_RIGHT) {
    if (ui8_pwm_compare_value == 0) {
      UNSET_BIT(TCCR0A, COM0B1);
      UNSET_BIT(TCCR0A, COM0B0);
      UNSET_BIT(PORTD, PD5);
    } else if (ui8_pwm_compare_value == 255) {
      UNSET_BIT(TCCR0A, COM0B1);
			UNSET_BIT(TCCR0A, COM0B0);
      SET_BIT(PORTD, PD5);
    } else {
      SET_BIT(TCCR0A, COM0B1);
      UNSET_BIT(TCCR0A, COM0B0);
      OCR0B = ui8_pwm_compare_value;
    }
  }
}

void ENGINE_drive(ENGINE_drive_direction direction) {
  switch (direction) {
  case ENGINE_STOP:
    INFO("Robi is: STOP\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, 255);  // set to full speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, 255); // set to full speed

    // Stop left motors
    UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Stop right motors
    UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_BACKWARDS:
    INFO("Robi is: BACKWARDS\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, 255 / 2);  // set to half speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, 255 / 2); // set to half speed

    // Left motors backwards
    UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors backwards
    SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_FORWARD:
    INFO("Robi is: FORWARD\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255 / 4)); // set to 3/4 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                          ~(255 / 4)); // set to 3/4 speed
                                       // 1111 1111 = 255
                                       // 0011 1111 = 63  = 255/4
                                       // 1100 0000 = 192 = ~63

    // Left motors forward
    SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors forward
    UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_HARD_LEFT:
    INFO("Robi is: HARD_LEFT\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255 / 8));   // set to 1/8 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255 / 4)); // set to 3/4 speed

    // Left motors backwards
    UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors forward
    UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_LEFT:
    INFO("Robi is: LEFT\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255 / 4));   // set to 1/4 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255 / 6)); // set to 5/6 speed

    // Left motors stop
    UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors forward
    UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_HARD_RIGHT:
    INFO("Robi is: HARD_RIGHT\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255 / 4)); // set to 3/4 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, (255 / 8)); // set to 1/8 speed

    // Left motors forward
    SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors backwards
    SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_RIGHT:
    INFO("Robi is: RIGHT\n");

    ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255 / 6)); // set to 5/6 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, (255 / 4)); // set to 1/4 speed

    // Left motors forward
    SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors stop
    UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
    break;

  case ENGINE_UNDEFINED:
    break;
  }

  return;
}

uint8_t ENGINE_drive_logic(LF_detection_state new_lf_state,
                           LF_detection_state old_lf_state,
                           uint16_t *LMR_itterations_since_entry) {
  // Optimize performance: only change direction when sensor state changes
  // Exception: LF_LMR always processes (intersection handling)
  if (new_lf_state == old_lf_state                  // No state change
      && new_lf_state != (LF_detection_state)LF_LMR // Not an intersection
  ) {
    return 0; // No action needed
  }

  // Convert line sensor state to motor command
  switch (new_lf_state) {
  case (LF_detection_state)LF_NONE:
    // Lost line - back up to reacquire
    ENGINE_drive((ENGINE_drive_direction)ENGINE_BACKWARDS);
    break;

  case (LF_detection_state)LF_LMR:
    // start/end detected - controlled forward movement with delay
    if (*LMR_itterations_since_entry > LMR_FORWARD_DELAY_IN_ITTERATIONS) {
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
