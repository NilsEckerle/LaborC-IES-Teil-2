#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

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
 * @brief pushes one bit into the shift register
 * on false input the modolo 2 of the number is pushed
 * @param value either SHIFT_HIGH or SHIFT_LOW
 * @return
 * 0 on success
 * 1 on set data error
 * 2 on send clock cicle error
 * */
int SHIFT_push(unsigned int ui_value);

/**
 * @brief Pushes an array of values to the shift register in reverse order
 * @param uiarrayp_values Pointer to array of unsigned int values
 * (SHIFT_HIGH/SHIFT_LOW)
 * @param ui_size Number of elements in the array (must be > 0)
 * @return
 * 0 on success
 * 1 on invalid parameters (NULL pointer or zero size)
 * 2 on SHIFT_push failure
 */
int SHIFT_push_state(unsigned int *uiarray_values, unsigned int ui_size);

/**
 * @brief Initializes DDR and Port of the shift register
 * @return 0 on success
 * */
int SHIFT_init();

/********************
 * END SHIFT REGISTER LOGIC
 ********************/

#endif // END SHIFT_REGISTER_H
