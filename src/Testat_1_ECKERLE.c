// #define F_CPU 16E6
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "iesusart.h"

/********************
* START DEBUG LOGIC
********************/

#define DEBUG_LEVEL 1

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 3
#endif /* ifndef DEBUG_LEVEL */

#if DEBUG_LEVEL<=0
#define TRACE(msg) USART_print("[TRACE] " msg)
#else
// ((void)0) needed for correctly handle the semicolon when using the function
// anything possible which is beeing deleted by the compiler.
#define TRACE(msg) ((void)0) 
#endif /* if DEBUG_LEVEL>=0 */

#if DEBUG_LEVEL<=1
#define INFO(msg) USART_print("[INFO] " msg)
#else
// ((void)0) needed for correctly handle the semicolon when using the function
// anything possible which is beeing deleted by the compiler.
#define INFO(msg) ((void)0) 
#endif /* if DEBUG_LEVEL>=1 */

#if DEBUG_LEVEL<=2
#define WARNING(msg) USART_print("[WARNING] " msg)
#else
// ((void)0) needed for correctly handle the semicolon when using the function
// anything possible which is beeing deleted by the compiler.
#define WARNING(msg) ((void)0)
#endif /* if DEBUG_LEVEL>=2 */

#if DEBUG_LEVEL<=3
#define ERROR(msg) USART_print("[ERROR] " msg)
#else
// ((void)0) needed for correctly handle the semicolon when using the function
// anything possible which is beeing deleted by the compiler.
#define ERROR(msg) ((void)0)
#endif /* if DEBUG_LEVEL>=3 */

#if DEBUG_LEVEL<=4
#define FATAL(msg) USART_print("[FATAL] " msg)
#else
// ((void)0) needed for correctly handle the semicolon when using the function
// anything possible which is beeing deleted by the compiler.
#define FATAL(msg) ((void)0)
#endif /* if DEBUG_LEVEL>=4 */

/********************
* END DEBUG LOGIC
********************/

/********************
* START SHIFT REGISTER LOGIC
********************/

#define SHIFT_DATA_DDR   DDRB
#define SHIFT_DATA_PORT  PORTB
#define SHIFT_DATA_PIN   PORTB2

#define SHIFT_CLOCK_DDR  DDRD
#define SHIFT_CLOCK_PORT PORTD
#define SHIFT_CLOCK_PIN  PORTD4

#define SHIFT_HIGH 1
#define SHIFT_LOW  0

/**
 * @brief sets data pin high or low
 * @param value either SHIFT_HIGH or SHIFT_LOW, gets set into the pin
 * @return 0 on success
 * */
int _SHIFT_set_data_pin(unsigned int ui_value) {
	SHIFT_DATA_PORT &= ~(1 << SHIFT_DATA_PIN); // enshure data pin is zero
																						 
	// set data pin as needed
	if (SHIFT_HIGH == ui_value){
		SHIFT_DATA_PORT |= (1 << SHIFT_DATA_PIN);
	}
	TRACE("Shift register Data set.\n");

	return 0;
}

/**
 * @brief cicles the shift register one time
 * @return 0 on success
 * */
int	_SHIFT_cicle() {
	SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_PIN); // ensure clock pin is zero
																							 
	// toggle clock
	SHIFT_CLOCK_PORT |= (1 << SHIFT_CLOCK_PIN);
	SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_PIN);
	TRACE("Shift register cicle send.\n");

	return 0;
}

/**
 * @brief pushes one bit into the shift register
 * on false input the modolo 2 of the number is pushed
 * @param value either SHIFT_HIGH or SHIFT_LOW
 * @return 
 * 0 on success
 * 1 on set data error
 * 2 on send clock cicle error
 * */
int SHIFT_push(unsigned int ui_value) {
	unsigned int ui_value_to_push = ui_value%2;

	// set data
	int rc = _SHIFT_set_data_pin(ui_value_to_push);
	if (0 != rc) { 
		ERROR("Shift register set data pin failed.\n");
		return 1; 
	}

	// toggle clock
	rc = _SHIFT_cicle();
	if (0 != rc) { 
		ERROR("Shift register cicle failed\n");
		return 2; 
	}

	return 0;
}

/**
 * @brief Pushes an array of values to the shift register in reverse order
 * @param uiarrayp_values Pointer to array of unsigned int values (SHIFT_HIGH/SHIFT_LOW)
 * @param ui_size Number of elements in the array (must be > 0)
 * @return 
 * 0 on success
 * 1 on invalid parameters (NULL pointer or zero size)
 * 2 on SHIFT_push failure
 */
int SHIFT_push_state(unsigned int* uiarray_values, unsigned int ui_size) {
    if (uiarray_values == NULL || ui_size == 0) {
        ERROR("Invalid parameters for SHIFT_push_state\n");
        return 1;
    }
    
    // Iterate backwards over array    
    for (int i = ui_size - 1; i >= 0; i--) {
        int rc = SHIFT_push(uiarray_values[i]);
        if (rc != 0) {
            ERROR("SHIFT_push failed during state push\n");
            return 2;
        }
    }
    
    INFO("State pushed to shift register\n");
    return 0;
}

/**
 * @brief Initializes DDR and Port of the shift register
 * @return 0 on success
 * */
int SHIFT_init() {
	// set as output
  SHIFT_DATA_DDR |= (1 << SHIFT_DATA_PIN);
  SHIFT_CLOCK_DDR |= (1 << SHIFT_CLOCK_PIN);
	TRACE("Shift register setup DDR of data and clock\n");

	// set default as low
	SHIFT_DATA_PORT &= ~(1 << SHIFT_DATA_PIN);
	SHIFT_CLOCK_PORT &= ~(1 << SHIFT_CLOCK_PIN);
	TRACE("Shift register initialized PORT of data and clock to low\n");

	unsigned int uiarray_low[3] = {SHIFT_LOW, SHIFT_LOW, SHIFT_LOW};
	int rc = SHIFT_push_state(uiarray_low, 3);
	if (0 != rc) {
		ERROR("Shift register init failed");
		return 1;
	}

	return 0;
}

/********************
* END SHIFT REGISTER LOGIC
********************/

/********************
* START LINIENFOLGER LOGIC
********************/

#define LF_0_DDR  DDRC
#define LF_0_PORT PORTD
#define LF_0_PIN  PORTD1

#define LF_1_DDR  DDRC
#define LF_1_PORT PORTD
#define LF_1_PIN  PORTD2

#define LF_2_DDR  DDRC
#define LF_2_PORT PORTD
#define LF_2_PIN  PORTD3

int LF_get_state(unsigned int ui_lf_index) {
	int state =
	return -1;
}

int LF_get_states(unsigned int *uiarray_output) {
	return 0;
}

/********************
* END LINIENFOLGER LOGIC
********************/

int main(void) {
	USART_init(UBRR_SETTING);
	INFO("USART working! Hooray!\n");

	int rc = SHIFT_init();
	if (0 == rc) { INFO("Shift register DDR setup successful.\n"); }
	else				 { ERROR("Shift register DDR setup FAILED!\n"); }

	 // Set Data Direction Register B, Pin 5 as output.
	 // DDRB = (1 << DDB5);

	unsigned int pattern1[] = {SHIFT_HIGH, SHIFT_HIGH, SHIFT_LOW};
	unsigned int pattern2[] = {SHIFT_LOW, SHIFT_LOW, SHIFT_HIGH};

	while (1) {
		SHIFT_push_state(pattern1, 3);
		_delay_ms(1000);

		SHIFT_push_state(pattern2, 3);
		_delay_ms(1000);
	}
	return 0;
}
