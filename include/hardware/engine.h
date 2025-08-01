/**
 * @file engine.h
 * @brief Motor control system for differential drive robot with H-bridge drivers
 * @author Nils Eckerle
 * @date 2025-08-01
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "hardware/linienfolger.h"
#include <avr/io.h>
#include <stdint.h>

// H-Bridge Enable pins

/**
 * @brief Left H-Bridge enable pin data direction register
 * @note Controls PD5 pin direction for left motor enable signal
 */
#define ENGINE_HB_ENA_DDR DDRD

/**
 * @brief Left H-Bridge enable pin output port
 * @note Controls PD5 pin output state for left motor enable signal
 */
#define ENGINE_HB_ENA_PORT PORTD

/**
 * @brief Left H-Bridge enable pin input register
 * @note Reads PD5 pin state for left motor enable signal
 */
#define ENGINE_HB_ENA_PIN PIND

/**
 * @brief Left H-Bridge enable pin bit position
 * @note Bit 5 in PORTD/PIND/DDRD registers for left motor enable
 */
#define ENGINE_HB_ENA_BIT 5

/**
 * @brief Right H-Bridge enable pin data direction register
 * @note Controls PD6 pin direction for right motor enable signal
 */
#define ENGINE_HB_ENB_DDR DDRD

/**
 * @brief Right H-Bridge enable pin output port
 * @note Controls PD6 pin output state for right motor enable signal
 */
#define ENGINE_HB_ENB_PORT PORTD

/**
 * @brief Right H-Bridge enable pin input register
 * @note Reads PD6 pin state for right motor enable signal
 */
#define ENGINE_HB_ENB_PIN PIND

/**
 * @brief Right H-Bridge enable pin bit position
 * @note Bit 6 in PORTD/PIND/DDRD registers for right motor enable
 */
#define ENGINE_HB_ENB_BIT 6

// H-Bridge direction control pins

/**
 * @brief Left H-Bridge direction control pin 1 data direction register
 * @note Controls PD7 pin direction for left motor direction control
 */
#define ENGINE_HB_IN1_DDR DDRD

/**
 * @brief Left H-Bridge direction control pin 1 output port
 * @note Controls PD7 pin output state for left motor direction control
 */
#define ENGINE_HB_IN1_PORT PORTD

/**
 * @brief Left H-Bridge direction control pin 1 input register
 * @note Reads PD7 pin state for left motor direction control
 */
#define ENGINE_HB_IN1_PIN PIND

/**
 * @brief Left H-Bridge direction control pin 1 bit position
 * @note Bit 7 in PORTD/PIND/DDRD registers for left motor direction
 */
#define ENGINE_HB_IN1_BIT 7

/**
 * @brief Left H-Bridge direction control pin 2 data direction register
 * @note Controls PB0 pin direction for left motor direction control
 */
#define ENGINE_HB_IN2_DDR DDRB

/**
 * @brief Left H-Bridge direction control pin 2 output port
 * @note Controls PB0 pin output state for left motor direction control
 */
#define ENGINE_HB_IN2_PORT PORTB

/**
 * @brief Left H-Bridge direction control pin 2 input register
 * @note Reads PB0 pin state for left motor direction control
 */
#define ENGINE_HB_IN2_PIN PINB

/**
 * @brief Left H-Bridge direction control pin 2 bit position
 * @note Bit 0 in PORTB/PINB/DDRB registers for left motor direction
 */
#define ENGINE_HB_IN2_BIT 0

/**
 * @brief Right H-Bridge direction control pin 3 data direction register
 * @note Controls PB1 pin direction for right motor direction control
 */
#define ENGINE_HB_IN3_DDR DDRB

/**
 * @brief Right H-Bridge direction control pin 3 output port
 * @note Controls PB1 pin output state for right motor direction control
 */
#define ENGINE_HB_IN3_PORT PORTB

/**
 * @brief Right H-Bridge direction control pin 3 input register
 * @note Reads PB1 pin state for right motor direction control
 */
#define ENGINE_HB_IN3_PIN PINB

/**
 * @brief Right H-Bridge direction control pin 3 bit position
 * @note Bit 1 in PORTB/PINB/DDRB registers for right motor direction
 */
#define ENGINE_HB_IN3_BIT 1

/**
 * @brief Right H-Bridge direction control pin 4 data direction register
 * @note Controls PB3 pin direction for right motor direction control
 */
#define ENGINE_HB_IN4_DDR DDRB

/**
 * @brief Right H-Bridge direction control pin 4 output port
 * @note Controls PB3 pin output state for right motor direction control
 */
#define ENGINE_HB_IN4_PORT PORTB

/**
 * @brief Right H-Bridge direction control pin 4 input register
 * @note Reads PB3 pin state for right motor direction control
 */
#define ENGINE_HB_IN4_PIN PINB

/**
 * @brief Right H-Bridge direction control pin 4 bit position
 * @note Bit 3 in PORTB/PINB/DDRB registers for right motor direction
 */
#define ENGINE_HB_IN4_BIT 3

/**
 * @brief PWM channel identifier for left motor
 * @note Used with ENGINE_set_duty_cicle() to control left motor speed
 */
#define ENGINE_PWM_LEFT 0

/**
 * @brief PWM channel identifier for right motor
 * @note Used with ENGINE_set_duty_cicle() to control right motor speed
 */
#define ENGINE_PWM_RIGHT 1

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
 * - LEFT/RIGHT: One motor pair is slower, the other is faster (gentle turn)
 * - HARD_LEFT/HARD_RIGHT: Motor pairs move in opposite directions (sharp turn)
 */
typedef enum {
  ENGINE_UNDEFINED,  /**< Invalid/uninitialized state */
  ENGINE_STOP,       /**< Both motor pairs stopped */
  ENGINE_BACKWARDS,  /**< Both motor pairs reverse direction */
  ENGINE_FORWARD,    /**< Both motor pairs forward direction */
  ENGINE_HARD_LEFT,  /**< Left motors reverse, right motors forward (sharp left) */
  ENGINE_LEFT,       /**< Left motors stop, right motors forward (gentle left) */
  ENGINE_HARD_RIGHT, /**< Left motors forward, right motors reverse (sharp right) */
  ENGINE_RIGHT       /**< Left motors forward, right motors stop (gentle right) */
} ENGINE_drive_direction;

/**
 * @brief Initializes motor control system
 * 
 * Sets up pins for H-bridge motor control and enables both motor pairs.
 * Configures all control pins as outputs and enables motor power.
 * Also initializes PWM system with:
 * - Timer0 configured for Fast PWM mode
 * - Prescaler set to 64 for appropriate PWM frequency
 * - Both motors set to full power initially
 * 
 * @return nothing, this function can't fail.
 * 
 * @note After initialization, motors are enabled and set to full power but stopped
 * @note PWM frequency = F_CPU / (PRESCALER * 2^8) = 16MHz / (64 * 256) ≈ 976 Hz
 */
void ENGINE_init();

/**
 * @brief Sets the duty cycle of the PWM for the engine
 *
 * Controls motor speed by adjusting the PWM duty cycle. Uses Timer0 compare
 * match functionality to generate PWM signals on PD5 (right) and PD6 (left).
 * 
 * Special cases:
 * - Value 0: Disconnects timer and sets pin LOW (0% duty cycle)
 * - Value 255: Disconnects timer and sets pin HIGH (100% duty cycle)
 * - Other values: Uses hardware PWM generation
 *
 * @param ui8_side Motor side selector (ENGINE_PWM_LEFT or ENGINE_PWM_RIGHT)
 * @param ui8_pwm_compare_value PWM compare value (0-255 representing 0%-100% duty cycle)
 *
 * @return nothing, this function can't fail.
 * 
 * @note Left motor uses Timer0 Channel A (OC0A/PD6)
 * @note Right motor uses Timer0 Channel B (OC0B/PD5)
 * @note Non-inverting PWM mode: HIGH at BOTTOM, LOW on compare match
 */
void ENGINE_set_duty_cicle(uint8_t ui8_side, uint8_t ui8_pwm_compare_value);

#endif  // !ENGINE_H
