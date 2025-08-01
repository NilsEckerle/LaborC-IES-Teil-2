/**
 * @file linienfolger.h
 * @brief Line follower sensor interface for 3-sensor array line detection
 * @author Nils Eckerle
 * @date 2025-08-01
 */

#ifndef LINIENFOLGER_H
#define LINIENFOLGER_H

#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Right line follower sensor data direction register
 * @note Controls PC0 pin direction for right sensor input
 */
#define LF_RIGHT_DDR DDRC

/**
 * @brief Right line follower sensor output port
 * @note Controls PC0 pin pull-up resistor for right sensor
 */
#define LF_RIGHT_PORT PORTC

/**
 * @brief Right line follower sensor input register
 * @note Reads PC0 pin state for right sensor digital input
 */
#define LF_RIGHT_PIN PINC

/**
 * @brief Right line follower sensor bit position
 * @note Bit 0 in PORTC/PINC/DDRC registers for right sensor
 */
#define LF_RIGHT_BIT PINC0

/**
 * @brief Middle line follower sensor data direction register
 * @note Controls PC1 pin direction for middle sensor input
 */
#define LF_MIDDLE_DDR DDRC

/**
 * @brief Middle line follower sensor output port
 * @note Controls PC1 pin pull-up resistor for middle sensor
 */
#define LF_MIDDLE_PORT PORTC

/**
 * @brief Middle line follower sensor input register
 * @note Reads PC1 pin state for middle sensor digital input
 */
#define LF_MIDDLE_PIN PINC

/**
 * @brief Middle line follower sensor bit position
 * @note Bit 1 in PORTC/PINC/DDRC registers for middle sensor
 */
#define LF_MIDDLE_BIT PINC1

/**
 * @brief Left line follower sensor data direction register
 * @note Controls PC2 pin direction for left sensor input
 */
#define LF_LEFT_DDR DDRC

/**
 * @brief Left line follower sensor output port
 * @note Controls PC2 pin pull-up resistor for left sensor
 */
#define LF_LEFT_PORT PORTC

/**
 * @brief Left line follower sensor input register
 * @note Reads PC2 pin state for left sensor digital input
 */
#define LF_LEFT_PIN PINC

/**
 * @brief Left line follower sensor bit position
 * @note Bit 2 in PORTC/PINC/DDRC registers for left sensor
 */
#define LF_LEFT_BIT PINC2

/**
 * @brief Enumeration representing line detection states from a 3-sensor line follower
 *
 * This enum defines the possible states detected by a line following sensor array
 * consisting of three sensors arranged as Left (L), Middle (M), and Right (R) in driving direction.
 * Each state represents which combination of sensors detect a line.
 *
 * The sensors are mapped as follows:
 * - L (Left): Sensor 0 (LF_0)
 * - M (Middle): Sensor 1 (LF_1) 
 * - R (Right): Sensor 2 (LF_2)
 * 
 * @note
 *       - LF_LR represents an edge case where only left and right sensors detect
 *       a line simultaneously, which may indicate a wide line, intersection,
 *       or sensor malfunction.
 *
 *       - Enum member values got chosen for its bit representation to work with 
 *       the shift register (last 3 bit representing Left Middle Right in this order).
 */
typedef enum {
  LF_UNDEFINED = 8, /**< Invalid/uninitialized state or sensor read error */
  LF_NONE = 0,      /**< No sensors detect a line (000) */
  LF_LMR = 7,       /**< All sensors detect a line (111) - wide line or intersection */
  LF_LR = 5,        /**< Left and right sensors detect a line (101) - edge case */
  LF_L = 4,         /**< Only left sensor detects a line (100) */
  LF_LM = 6,        /**< Left and middle sensors detect a line (110) */
  LF_M = 2,         /**< Only middle sensor detects a line (010) - centered on line */
  LF_MR = 3,        /**< Middle and right sensors detect a line (011) */
  LF_R = 1          /**< Only right sensor detects a line (001) */
} LF_detection_state;

/**
 * @brief Initialize line follower sensor pins
 * @note Sets pins as inputs with pull-up resistors enabled
 * @return nothing, this function can't fail
 */
void LF_init();

/**
 * @brief Line follower sensor index enumeration
 * @note Used to identify specific sensors in the 3-sensor array
 */
typedef enum { 
  LF_LEFT = 0,   /**< Left sensor (index 0) */
  LF_MIDDLE,     /**< Middle sensor (index 1) */
  LF_RIGHT       /**< Right sensor (index 2) */
} LF_index;

/**
 * @brief gets the state of the line sensor of index
 * @param lf_index is the index of the line sensor
 * @return the sensor state (0 or 1) on success, -1 on failure 
 * e.g. index not valid
 * @note index left middle right has index 0 1 2
 */
int8_t LF_get_state(LF_index lf_index);

/**
 * @brief converts a bitstring of 3 bits (starting at LSB) to a LF_detection_state
 * @param ui_lf_detection_bitstring 3-bit value representing sensor states
 * @return the converted state
 */
LF_detection_state LF_bitstring_to_state(uint8_t ui_lf_detection_bitstring);

/**
 * @brief Reads all three line follower sensor states and stores them in output array
 * @return LF_detection_state representing combined sensor reading
 */
LF_detection_state LF_get_states();

#endif /* ifndef LINIENFOLGER_H */
