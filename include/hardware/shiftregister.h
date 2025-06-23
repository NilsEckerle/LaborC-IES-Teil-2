#include <stdint.h>
#include <avr/io.h>

#define SHIFT_DATA_DDR DDRB
#define SHIFT_DATA_PORT PORTB
#define SHIFT_DATA_PIN PINB
#define SHIFT_DATA_BIT PINB2

#define SHIFT_CLOCK_DDR DDRD
#define SHIFT_CLOCK_PORT PORTD
#define SHIFT_CLOCK_PIN PIND
#define SHIFT_CLOCK_BIT PIND4

#define SHIFT_HIGH 1
#define SHIFT_LOW 0

/**
 * @brief sets data pin high or low
 * @param value either SHIFT_HIGH or SHIFT_LOW, gets set into the pin
 * @return nothing, this function can't fail
 * */
void _SHIFT_set_data_pin(unsigned int ui_value);

/**
 * @brief cicles the shift register one time
 * @return nothing, this function can't fail
 * */
void _SHIFT_cicle();

/**
 * @brief pushes one bit into the shift register
 * on false input the modolo 2 of the number is pushed
 * @param value either SHIFT_HIGH or SHIFT_LOW
 * @return nothing, this function can't fail
 * */
void SHIFT_push(unsigned int ui_value);

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
 * @brief Initializes DDR and Port of the shift register
 * @return nothing, this function can't fail
 * */
void SHIFT_init();
