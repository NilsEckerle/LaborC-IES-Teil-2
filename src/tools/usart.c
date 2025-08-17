#include "tools/usart.h"
#include "roboter/roboter_model.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdio.h>

#define MAX_USART_BUFFER 64
#define CLEAR_THRESHOLD (((int)(F_CPU / BAUD)) + 1)

volatile char usart_rx_buffer[MAX_USART_BUFFER];
volatile uint8_t usart_str_valid = 0;
volatile uint8_t usart_buffer_index = 0;
volatile uint8_t usart_overflow = 0;

/**
 * @brief Sets up the USART port (The USART baudrate register)
 * @param ubrr Content to write into the UBRR register
 */
void USART_init(unsigned long ubrr) {
  // Set baud rate
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;

  // Enable receiver, transmitter, and RX Complete interrupt
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

  // Frame format: 8 data bits, 1 stop bit, no parity
  UCSR0C = (3 << UCSZ00);

  // Enable global interrupts
  sei();
}

/**
 * @brief Writes a single byte to the USART transmit buffer
 * @param data Byte that shall be transmitted
 */
void USART_transmitByte(unsigned char data) {
  // Wait for empty transmit buffer
  while (!(UCSR0A & (1 << UDRE0))) {}

  // send data (noting is called, everything in UDR0 is send)
  UDR0 = data;
}

/**
 * @brief Transmittes a string (char by char) until '\0’ is reached
 */
void USART_print(const char *c) {
  while (*c != '\0') {
    USART_transmitByte(*c);
    c++;
  }
}

// USART Receive Complete Interrupt Service Routine
ISR(USART_RX_vect) {
  char received_char = UDR0;  // Read character immediately

  // Check for buffer overflow FIRST
  if (usart_buffer_index >= (MAX_USART_BUFFER - 1)) {
    // Buffer full - terminate and mark as valid
    usart_rx_buffer[MAX_USART_BUFFER - 1] = '\0';
    usart_str_valid = 1;
    usart_overflow = 1;
    return;  // Don't process this character
  }

  // Handle termination characters
  if (received_char == '\n' || received_char == '\r') {
    // Null terminate the string
    usart_rx_buffer[usart_buffer_index] = '\0';
    usart_str_valid = 1;  // Mark string as complete
    // Don't reset buffer_index here, this is done, when char gets consumed
    return;
  }

  // Filter out control characters (optional)
  if (received_char < 32 || received_char > 126) {
    // Ignore non-printable characters except CR/LF
    return;
  }

  // Add character to buffer
  usart_rx_buffer[usart_buffer_index] = received_char;
  usart_buffer_index++;
}

// Helper functions
char *USART_get_string(void) {
  if (usart_str_valid) {
    return (char *)usart_rx_buffer;
  }
  return NULL;
}

void USART_consume_string(void) {
  // Disable interrupts while modifying shared data
  cli();
  usart_str_valid = 0;
  usart_buffer_index = 0;
  usart_overflow = 0;
  usart_rx_buffer[0] = '\0';
  sei();
}

uint8_t USART_has_string(void) { return usart_str_valid; }

uint8_t USART_buffer_overflow(void) { return usart_overflow; }

// Two-stage clear implementation
static uint16_t clear_counter = 0;

void USART_consume_on_second_call_string(void) {
  clear_counter++;

  if (clear_counter >= CLEAR_THRESHOLD) {
    USART_consume_string();
    clear_counter = 0;
  }
}
