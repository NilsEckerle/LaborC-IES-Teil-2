/**
 * @file usart.h
 * @brief USART communication interface for AVR microcontrollers
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef USART_h
#define USART_h

#include <stdint.h>

/**
 * @brief Receive buffer for incoming USART data
 */
extern volatile char usart_rx_buffer[];

/**
 * @brief Flag indicating if a complete string is available
 */
extern volatile uint8_t usart_str_valid;

/**
 * @brief Current index in the receive buffer
 */
extern volatile uint8_t usart_buffer_index;

/**
 * @brief Flag indicating if buffer overflow occurred
 */
extern volatile uint8_t usart_overflow;

/// CPU clock speed
#ifndef F_CPU
#define F_CPU 16E6
#endif
/// Desired baudrate
#define BAUD 9600
/// What to write into the UBRR register
#define UBRR_SETTING F_CPU / 16.0 / BAUD - 1

/**
 * @brief Writes a single byte to the USART transmit buffer
 * @param data Byte that shall be transmitted
 */
void USART_transmitByte(unsigned char data);

/**
 * @brief Transmits a string character by character until '\0' is reached
 * @param c Null-terminated string to transmit
 */
void USART_print(const char *c);

/**
 * @brief Gets pointer to received string if available
 * @return Pointer to string buffer or NULL if no string available
 */
char *USART_get_string(void);

/**
 * @brief Consumes and clears the current received string
 */
void USART_consume_string(void);

/**
 * @brief Checks if a complete string is available
 * @return 1 if string is available, 0 otherwise
 */
uint8_t USART_has_string(void);

/**
 * @brief Checks if buffer overflow occurred
 * @return 1 if overflow occurred, 0 otherwise
 */
uint8_t USART_buffer_overflow(void);

/**
 * @brief Delayed string consumption with threshold counter
 */
void USART_consume_on_second_call_string(void);

/**
 * @brief Resets the delayed consumption counter
 */
void USART_reset_clear_counter(void);

/**
 * @brief Sets up the USART port (The USART baudrate register)
 * @param ubrr Content to write into the UBRR register
 */
void USART_init(unsigned long ubrr);

#endif
