#ifndef LINIENFOLGER_H
#define LINIENFOLGER_H

#include <avr/io.h>
#include <stdint.h>

// Linienfolger 0 (left)
#define LF_RIGHT_DDR DDRC
#define LF_RIGHT_PORT PORTC
#define LF_RIGHT_PIN PINC
#define LF_RIGHT_BIT PINC0

// Linienfolger 1 (middle)
#define LF_MIDDLE_DDR DDRC
#define LF_MIDDLE_PORT PORTC
#define LF_MIDDLE_PIN PINC
#define LF_MIDDLE_BIT PINC1

// Linienfolger 2 (right)
#define LF_LEFT_DDR DDRC
#define LF_LEFT_PORT PORTC
#define LF_LEFT_PIN PINC
#define LF_LEFT_BIT PINC2

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
 * @note 
 * 			 - LF_LR represents an edge case where only left and right sensors detect
 *       a line simultaneously, which may indicate a wide line, intersection,
 *       or sensor malfunction.
 *
 *       - Enum member values got chosen for its bit representation to work with 
 *       the shift register (last 3 bit representing Left Middle Right in this order).
 */
typedef enum { 
	LF_UNDEFINED 	= 8, /**< Invalid/uninitialized state or sensor read error */
	LF_NONE 			= 0, /**< No sensors detect a line (000) */
	LF_LMR 				= 7, /**< All sensors detect a line (111) - wide line or intersection */
	LF_LR 				= 5, /**< Left and right sensors detect a line (101) - edge case */
	LF_L 					= 4, /**< Only left sensor detects a line (100) */
	LF_LM 				= 6, /**< Left and middle sensors detect a line (110) */
	LF_M 					= 2, /**< Only middle sensor detects a line (010) - centered on line */
	LF_MR 				= 3, /**< Middle and right sensors detect a line (011) */
	LF_R 					= 1  /**< Only right sensor detects a line (001) */
} LF_detection_state;


/**
 * @brief setup DDR and PORT of input pins
 * @return nothing, this function can't fail
 */
void LF_init();

/**
 * @brief gets the state of the line sensor of index
 * @param ui_lf_index is the index of the line sensor
 * @return the sensor state (0 or 1) on success, -1 on failure 
 * e.g. index not valid
 * @note index left middle right has index 0 1 2
 */
int8_t LF_get_state(uint8_t ui_lf_index);

/**
 * @brief converts a bitstring of 3 bits (starting at LSB) to a LF_detection_state
 * @return the converted state
 */
LF_detection_state LF_bitstring_to_state(uint8_t ui_lf_detection_bitstring);

/**
 * @brief Reads all three line follower sensor states and stores them in output array
 * @param uiarray_output Pointer to array of 3 unsigned int elements [left, center, right]
 * @return LF_detection_state
 */
LF_detection_state LF_get_states();

#endif /* ifndef LINIENFOLGER_H */
