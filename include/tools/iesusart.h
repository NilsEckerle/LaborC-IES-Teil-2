#ifndef IESUSART_h
#define IESUSART_h

#include <stdint.h>

extern volatile char usart_rx_buffer[];
extern volatile uint8_t usart_str_valid;
extern volatile uint8_t usart_buffer_index;
extern volatile uint8_t usart_overflow;

/// CPU clock speed
#ifndef F_CPU
#define F_CPU 16E6
#endif
/// Desired baudrate
#define BAUD 9600
/// What to write into the UBRR register
#define UBRR_SETTING F_CPU/16.0/BAUD-1

/**
 * @brief Writes a single byte to the USART transmit buffer
 * @param data Byte that shall be transmitted
 */
void USART_transmitByte(unsigned char data);

/**
 * @brief Transmittes a string (char by char) until '\0’ is reached
 */
void USART_print(const char *c);

char* USART_get_string(void);

void USART_consume_string(void);

uint8_t USART_has_string(void);

uint8_t USART_buffer_overflow(void);

void USART_consume_on_second_call_string(void);

void USART_reset_clear_counter(void);

/**
 * @brief Sets up the USART port (The USART baudrate register)
 * @param ubrr Content to write into the UBRR register
 */
void USART_init(unsigned long ubrr);

#endif
