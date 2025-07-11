#ifndef CLOCK_H
#define CLOCK_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

#define CLOCK_CTL_A_REG TCCR1A
#define CLOCK_CTL_B_REG TCCR1B

// Timer1 Control Register A (TCCR1A) bit definitions
#define CLOCK_WGM10_BIT 0  // Waveform Generation Mode bit 0
#define CLOCK_WGM11_BIT 1  // Waveform Generation Mode bit 1
#define CLOCK_COM1B0_BIT 4 // Compare Output Mode B bit 0
#define CLOCK_COM1B1_BIT 5 // Compare Output Mode B bit 1
#define CLOCK_COM1A0_BIT 6 // Compare Output Mode A bit 0
#define CLOCK_COM1A1_BIT 7 // Compare Output Mode A bit 1

// Timer1 Control Register B (TCCR1B) bit definitions
#define CLOCK_CS10_BIT 0  // Clock Select bit 0
#define CLOCK_CS11_BIT 1  // Clock Select bit 1
#define CLOCK_CS12_BIT 2  // Clock Select bit 2
#define CLOCK_WGM12_BIT 3 // Waveform Generation Mode bit 2
#define CLOCK_WGM13_BIT 4 // Waveform Generation Mode bit 3
#define CLOCK_ICES1_BIT 6 // Input Capture Edge Select
#define CLOCK_ICNC1_BIT 7 // Input Capture Noise Canceler

// Timer1 Interrupt Mask Register (TIMSK1) bit definitions
#define CLOCK_TOIE1_BIT 0  // Timer1 Overflow Interrupt Enable
#define CLOCK_OCIE1A_BIT 1 // Timer1 Output Compare A Match Interrupt Enable
#define CLOCK_OCIE1B_BIT 2 // Timer1 Output Compare B Match Interrupt Enable
#define CLOCK_ICIE1_BIT 5  // Timer1 Input Capture Interrupt Enable

void CLOCK_init();

// Get current time in milliseconds
uint32_t CLOCK_get_milliseconds();

// Get current time in milliseconds
uint32_t CLOCK_get_seconds();

#endif // !CLOCK_H
