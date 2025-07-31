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
  SET_BIT(ENGINE_HB_ENA_DDR, ENGINE_HB_ENA_BIT);  // Left motor enable
  SET_BIT(ENGINE_HB_ENB_DDR, ENGINE_HB_ENB_BIT);  // Right motor enable
  SET_BIT(ENGINE_HB_IN1_DDR, ENGINE_HB_IN1_BIT);  // Left motor direction 1
  SET_BIT(ENGINE_HB_IN2_DDR, ENGINE_HB_IN2_BIT);  // Left motor direction 2
  SET_BIT(ENGINE_HB_IN3_DDR, ENGINE_HB_IN3_BIT);  // Right motor direction 1
  SET_BIT(ENGINE_HB_IN4_DDR, ENGINE_HB_IN4_BIT);  // Right motor direction 2

  // Enable both motor pairs (set enable pins high)
  SET_BIT(ENGINE_HB_ENA_PORT, ENGINE_HB_ENA_BIT);  // Enable left motors
  SET_BIT(ENGINE_HB_ENB_PORT, ENGINE_HB_ENB_BIT);  // Enable right motors

  ENGINE_init_pwm();
  INFO("ENGINE pwm init successful.\n");

  ENGINE_set_duty_cicle(ENGINE_LEFT, ~0);   // set to full power
  ENGINE_set_duty_cicle(ENGINE_RIGHT, ~0);  // set to full power

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
      UNSET_BIT(TCCR0A, COM0A0);  // Normal port operation mode
      UNSET_BIT(PORTD, PD6);      // PD6 LOW, equals 0% duty,
    }  // timer disconnected
    else if (ui8_pwm_compare_value == 255) {
      UNSET_BIT(TCCR0A, COM0A1);
      UNSET_BIT(TCCR0A, COM0A0);  // Normal port operation mode
      SET_BIT(PORTD, PD6);        // PD6 HIGH, equals 100% duty,
    }  // timer disconnected
    else {
      SET_BIT(TCCR0A, COM0A1);        // OC0A to LOW on Compare Match,
      UNSET_BIT(TCCR0A, COM0A0);      // to HIGH at BOTTOM (non-inverting mode)
      OCR0A = ui8_pwm_compare_value;  // generates sequences of 1-0-1-0...
    }  // for certain periods of time
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
