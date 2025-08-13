/**
 * @file clock.h
 * @brief Timer-based clock system for millisecond and second timekeeping
 * @author Nils Eckerle
 * @date 2025-08-01
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Timer1 Control Register A alias
 * @note Maps to TCCR1A register for waveform generation and compare output mode configuration
 */
#define CLOCK_CTL_A_REG TCCR1A

/**
 * @brief Timer1 Control Register B alias
 * @note Maps to TCCR1B register for clock selection and waveform generation mode configuration
 */
#define CLOCK_CTL_B_REG TCCR1B

// Timer1 Control Register A (TCCR1A) bit definitions

/**
 * @brief Waveform Generation Mode bit 0
 * @note Used in combination with WGM11, WGM12, WGM13 to select timer operation mode
 */
#define CLOCK_WGM10_BIT 0

/**
 * @brief Waveform Generation Mode bit 1
 * @note Used in combination with WGM10, WGM12, WGM13 to select timer operation mode
 */
#define CLOCK_WGM11_BIT 1

/**
 * @brief Compare Output Mode for Channel B bit 0
 * @note Controls behavior of OC1B pin on compare match events
 */
#define CLOCK_COM1B0_BIT 4

/**
 * @brief Compare Output Mode for Channel B bit 1
 * @note Controls behavior of OC1B pin on compare match events
 */
#define CLOCK_COM1B1_BIT 5

/**
 * @brief Compare Output Mode for Channel A bit 0
 * @note Controls behavior of OC1A pin on compare match events
 */
#define CLOCK_COM1A0_BIT 6

/**
 * @brief Compare Output Mode for Channel A bit 1
 * @note Controls behavior of OC1A pin on compare match events
 */
#define CLOCK_COM1A1_BIT 7

// Timer1 Control Register B (TCCR1B) bit definitions

/**
 * @brief Clock Select bit 0
 * @note Used with CS11 and CS12 to select timer clock source and prescaler
 */
#define CLOCK_CS10_BIT 0

/**
 * @brief Clock Select bit 1
 * @note Used with CS10 and CS12 to select timer clock source and prescaler
 */
#define CLOCK_CS11_BIT 1

/**
 * @brief Clock Select bit 2
 * @note Used with CS10 and CS11 to select timer clock source and prescaler
 */
#define CLOCK_CS12_BIT 2

/**
 * @brief Waveform Generation Mode bit 2
 * @note Used in combination with WGM10, WGM11, WGM13 to select timer operation mode
 */
#define CLOCK_WGM12_BIT 3

/**
 * @brief Waveform Generation Mode bit 3
 * @note Used in combination with WGM10, WGM11, WGM12 to select timer operation mode
 */
#define CLOCK_WGM13_BIT 4

/**
 * @brief Input Capture Edge Select
 * @note Selects which edge on ICP1 pin triggers input capture event
 * - 0: falling edge
 * - 1: rising edge
 */
#define CLOCK_ICES1_BIT 6

/**
 * @brief Input Capture Noise Canceler
 * @note Enables 4-cycle noise cancellation on input capture pin
 * - 0: noise canceler disabled
 * - 1: noise canceler enabled
 */
#define CLOCK_ICNC1_BIT 7

// Timer1 Interrupt Mask Register (TIMSK1) bit definitions

/**
 * @brief Timer1 Overflow Interrupt Enable
 * @note Enables interrupt when Timer1 overflows (TCNT1 wraps from 0xFFFF to 0x0000)
 */
#define CLOCK_TOIE1_BIT 0

/**
 * @brief Timer1 Output Compare A Match Interrupt Enable
 * @note Enables interrupt when TCNT1 matches OCR1A register value
 */
#define CLOCK_OCIE1A_BIT 1

/**
 * @brief Timer1 Output Compare B Match Interrupt Enable
 * @note Enables interrupt when TCNT1 matches OCR1B register value
 */
#define CLOCK_OCIE1B_BIT 2

/**
 * @brief Timer1 Input Capture Interrupt Enable
 * @note Enables interrupt when input capture event occurs on ICP1 pin
 */
#define CLOCK_ICIE1_BIT 5

#define CLOCK_STEP_MS 32

/**
 * @brief Initialize the clock system
 *
 * @note This function must be called before using any clock functions
 * @note Global interrupts are enabled after initialization
 * @note The function logs successful initialization via INFO macro
 */
void CLOCK_init();

/**
 * @brief Get current system time in milliseconds
 *
 * Returns the total elapsed time since clock initialization in milliseconds.
 * The value is calculated from both the millisecond and second counters
 * maintained by the timer interrupt.
 *
 * @return uint32_t Current time in milliseconds since initialization
 *
 * @note This function performs atomic reading by temporarily disabling interrupts
 * @note Maximum representable time: ~49.7 days (2^32 milliseconds)
 * @note Resolution: approximately 32 milliseconds (timer overflow period)
 */
uint32_t CLOCK_get_milliseconds();

/**
 * @brief Get current system time in seconds
 *
 * Returns the total elapsed time since clock initialization in seconds.
 * The value is read from the second counter maintained by the timer interrupt.
 *
 * @return uint32_t Current time in seconds since initialization
 *
 * @note This function performs atomic reading by temporarily disabling interrupts
 * @note Maximum representable time: ~136 years (2^32 seconds)
 * @note Resolution: 1 second (counter increments every 1000 milliseconds)
 */
uint32_t CLOCK_get_seconds();

#endif  // !CLOCK_H
