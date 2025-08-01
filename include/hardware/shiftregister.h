/**
 * @file shiftregister.h
 * @brief Serial shift register interface for LED or display control
 * @author Nils Eckerle
 * @date 2025-07-31
 */

#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Shift register data pin data direction register
 * @note Controls PB2 pin direction for serial data output
 */
#define SHIFT_DATA_DDR DDRB

/**
 * @brief Shift register data pin output port
 * @note Controls PB2 pin state for serial data transmission
 */
#define SHIFT_DATA_PORT PORTB

/**
 * @brief Shift register data pin input register
 * @note Reads PB2 pin state for data line
 */
#define SHIFT_DATA_PIN PINB

/**
 * @brief Shift register data pin bit position
 * @note Bit 2 in PORTB/PINB/DDRB registers for data line
 */
#define SHIFT_DATA_BIT PINB2

/**
 * @brief Shift register clock pin data direction register
 * @note Controls PD4 pin direction for clock signal output
 */
#define SHIFT_CLOCK_DDR DDRD

/**
 * @brief Shift register clock pin output port
 * @note Controls PD4 pin state for clock signal generation
 */
#define SHIFT_CLOCK_PORT PORTD

/**
 * @brief Shift register clock pin input register
 * @note Reads PD4 pin state for clock line
 */
#define SHIFT_CLOCK_PIN PIND

/**
 * @brief Shift register clock pin bit position
 * @note Bit 4 in PORTD/PIND/DDRD registers for clock line
 */
#define SHIFT_CLOCK_BIT PIND4

/**
 * @brief Logical high value for shift register operations
 * @note Used to set data pin high (1)
 */
#define SHIFT_HIGH 1

/**
 * @brief Logical low value for shift register operations
 * @note Used to set data pin low (0)
 */
#define SHIFT_LOW 0

/**
 * @brief sets data pin high or low
 * @param ui_value either SHIFT_HIGH or SHIFT_LOW, gets set into the pin
 * @return nothing, this function can't fail
 * */
void _SHIFT_set_data_pin(uint8_t ui_value);

/**
 * @brief cicles the shift register one time
 * @return nothing, this function can't fail
 * */
void _SHIFT_cicle();

/**
 * @brief pushes one bit into the shift register
 * on false input the modolo 2 of the number is pushed
 * @param ui_value either SHIFT_HIGH or SHIFT_LOW
 * @return nothing, this function can't fail
 * */
void SHIFT_push(uint8_t ui_value);

/**
 * @brief Converts line follower detection state to shift register output
 * @param bitstring_to_push the last 3 bits gets pushed, starting at the MSB
 * @return nothing, this function can't fail
 * 
 * @note The function pushes 3 bits to the shift register representing
 *       Left, Middle, Right sensor states in that order.
 *       Each push sends one bit, with 1 = sensor active, 0 = sensor inactive.
 */
void SHIFT_push_state(uint8_t bitstring_to_push);

/**
 * @brief Initialize shift register pins and clear register
 * @note Sets data and clock pins as outputs, initializes to low state, sends 000
 * @return nothing, this function can't fail
 * */
void SHIFT_init();

#endif // !SHIFTREGISTER_H
