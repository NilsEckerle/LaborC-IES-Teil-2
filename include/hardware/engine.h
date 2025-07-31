/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-31
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "hardware/linienfolger.h"
#include <avr/io.h>
#include <stdint.h>

// Drive Forward delay configuration
#if LOG_LEVEL <= DEBUG_LEVEL_INFO
#define LMR_FORWARD_DELAY_IN_ITTERATIONS 1
#else
#define LMR_FORWARD_DELAY_IN_ITTERATIONS 20000
#endif

// H-Bridge Enable pins
// Left H-Bridge
#define ENGINE_HB_ENA_DDR DDRD
#define ENGINE_HB_ENA_PORT PORTD
#define ENGINE_HB_ENA_PIN PIND
#define ENGINE_HB_ENA_BIT 5

// Right H-Bridge
#define ENGINE_HB_ENB_DDR DDRD
#define ENGINE_HB_ENB_PORT PORTD
#define ENGINE_HB_ENB_PIN PIND
#define ENGINE_HB_ENB_BIT 6

// H-Bridge direction control pins
// Left H-Bridge
#define ENGINE_HB_IN1_DDR DDRD
#define ENGINE_HB_IN1_PORT PORTD
#define ENGINE_HB_IN1_PIN PIND
#define ENGINE_HB_IN1_BIT 7

#define ENGINE_HB_IN2_DDR DDRB
#define ENGINE_HB_IN2_PORT PORTB
#define ENGINE_HB_IN2_PIN PINB
#define ENGINE_HB_IN2_BIT 0

// Right H-Bridge
#define ENGINE_HB_IN3_DDR DDRB
#define ENGINE_HB_IN3_PORT PORTB
#define ENGINE_HB_IN3_PIN PINB
#define ENGINE_HB_IN3_BIT 1

#define ENGINE_HB_IN4_DDR DDRB
#define ENGINE_HB_IN4_PORT PORTB
#define ENGINE_HB_IN4_PIN PINB
#define ENGINE_HB_IN4_BIT 3

// PWM
#define ENGINE_PWM_LEFT 0
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
 * - LEFT/RIGHT: One motor pair stops, other continues (gentle turn)
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
 * 
 * @return nothing, this function can't fail.
 * 
 * @note After initialization, motors are enabled but stopped (direction pins not set)
 */
void ENGINE_init();

/**
 * @brief sets the dudy cicle of the pwm for the engine.
 *
 * @param ui8_side (ENGINE_PWM_LEFT/ENGINE_PWM_RIGHT) is the side which the pwm is set.
 * @param ui8_pwm_compare_value (0 - 255 = 0% - 100%) is the compare value for the pwm clock.
 *
 * @return nothing, this function can't fail.
 */
void ENGINE_set_duty_cicle(uint8_t ui8_side, uint8_t ui8_pwm_compare_value);

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
void ENGINE_drive(ENGINE_drive_direction direction);

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
uint8_t ENGINE_drive_logic(LF_detection_state new_lf_state, LF_detection_state old_lf_state,
                           uint16_t *LMR_itterations_since_entry);

#endif  // !ENGINE_H
