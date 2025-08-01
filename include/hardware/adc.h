/**
 * @file adc.h
 * @brief Analog-to-Digital Converter (ADC) interface for AVR microcontrollers
 * @author Nils Eckerle
 * @date 2025-08-01
 */

#ifndef ADC_H
#define ADC_H

#include "tools/bit_functions.h"
#include <avr/io.h>

/**
 * @brief Default number of samples for averaging operations
 * @note Used as a standard window size for noise reduction in ADC readings
 */
#define ADC_AVG_WINDOW 10

/**
 * @brief ADC multiplexer control register alias
 * @note Maps to ADMUX register for channel selection and reference voltage configuration
 */
#define ADC_MUX_STEUER_REG ADMUX

/**
 * @brief ADC data register alias for reading conversion results
 * @note Maps to ADCW register which provides 16-bit access to ADC result
 */
#define ADC_READ ADCW

/**
 * @brief Data Direction Register for ADC channel 0 (PC0)
 * @note Used to configure PC0 pin as input for ADC operations
 */
#define ADC0_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 0 (PC0)
 * @note Used for digital input reading on PC0 pin
 */
#define ADC0_PIN PINC

/**
 * @brief Bit position for ADC channel 0 (PC0)
 * @note Corresponds to PINC0 bit in PORTC registers
 */
#define ADC0_BIT PINC0

/**
 * @brief Data Direction Register for ADC channel 1 (PC1)
 * @note Used to configure PC1 pin as input for ADC operations
 */
#define ADC1_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 1 (PC1)
 * @note Used for digital input reading on PC1 pin
 */
#define ADC1_PIN PINC

/**
 * @brief Bit position for ADC channel 1 (PC1)
 * @note Corresponds to PINC1 bit in PORTC registers
 */
#define ADC1_BIT PINC1

/**
 * @brief Data Direction Register for ADC channel 2 (PC2)
 * @note Used to configure PC2 pin as input for ADC operations
 */
#define ADC2_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 2 (PC2)
 * @note Used for digital input reading on PC2 pin
 */
#define ADC2_PIN PINC

/**
 * @brief Bit position for ADC channel 2 (PC2)
 * @note Corresponds to PINC2 bit in PORTC registers
 */
#define ADC2_BIT PINC2

/**
 * @brief Data Direction Register for ADC channel 3 (PC3)
 * @note Used to configure PC3 pin as input for ADC operations
 */
#define ADC3_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 3 (PC3)
 * @note Used for digital input reading on PC3 pin
 */
#define ADC3_PIN PINC

/**
 * @brief Bit position for ADC channel 3 (PC3)
 * @note Corresponds to PINC3 bit in PORTC registers
 */
#define ADC3_BIT PINC3

/**
 * @brief Data Direction Register for ADC channel 4 (PC4)
 * @note Used to configure PC4 pin as input for ADC operations
 */
#define ADC4_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 4 (PC4)
 * @note Used for digital input reading on PC4 pin
 */
#define ADC4_PIN PINC

/**
 * @brief Bit position for ADC channel 4 (PC4)
 * @note Corresponds to PINC4 bit in PORTC registers
 */
#define ADC4_BIT PINC4

/**
 * @brief Data Direction Register for ADC channel 5 (PC5)
 * @note Used to configure PC5 pin as input for ADC operations
 */
#define ADC5_DDR DDRC

/**
 * @brief Pin Input Register for ADC channel 5 (PC5)
 * @note Used for digital input reading on PC5 pin
 */
#define ADC5_PIN PINC

/**
 * @brief Bit position for ADC channel 5 (PC5)
 * @note Corresponds to PINC5 bit in PORTC registers
 */
#define ADC5_BIT PINC5

/** 
 * @brief Check if ADC is currently measuring
 * @return true if ADC conversion is in progress, false otherwise
 * @note Checks the ADSC bit in ADCSRA register to determine conversion status
 */
#define IS_ADC_MEASURING IS_BIT_SET(ADCSRA, ADSC)

/**
 * @brief ADC pin enumeration
 *
 * Enumeration defining available ADC pins with their specific functions.
 * Each pin corresponds to a specific ADC channel and application.
 * 
 * Pin assignments:
 * - ADC_LF_LEFT/MIDDLE/RIGHT: Line follower sensor inputs
 * - A3_Voltage_Divider_LiPo: Battery voltage monitoring
 * - ADC_C4/C5: General purpose ADC inputs
 */
typedef enum ADC_pin {
  ADC_LF_RIGHT = ADC0_BIT,            /**< Right line follower sensor (PC0/ADC0) */
  ADC_LF_MIDDLE = ADC1_BIT,           /**< Middle line follower sensor (PC1/ADC1) */
  ADC_LF_LEFT = ADC2_BIT,             /**< Left line follower sensor (PC2/ADC2) */
  A3_Voltage_Divider_LiPo = ADC3_BIT, /**< LiPo battery voltage divider (PC3/ADC3) */
  C4 = ADC4_BIT,                      /**< General purpose ADC input (PC4/ADC4) */
  C5 = ADC5_BIT                       /**< General purpose ADC input (PC5/ADC5) */
} t_adc_pin;

/**
 * @brief Initialize the ADC subsystem
 *
 * Configures the ADC with the following settings:
 * - Reference voltage: AVcc
 * - Prescaler: 128 (ADPS2:0 = 111)
 * - Enables ADC
 * - Performs initial dummy read for accuracy
 *
 * @note This function must be called before using any other ADC functions.
 * @note The function waits for any ongoing conversion to complete before initialization.
 */
void ADC_init();

/**
 * @brief Initialize a specific ADC pin for input
 *
 * Configures the specified pin as an input for ADC operations.
 *
 * @param pin The ADC pin to initialize (from t_adc_pin enumeration)
 */
void ADC_init_pin(t_adc_pin pin);

/**
 * @brief Read a single ADC value from the specified pin
 *
 * Performs a single ADC conversion on the specified pin and returns the result.
 *
 * @param pin The ADC pin to read from (from t_adc_pin enumeration)
 * @return 16-bit ADC conversion result (0-1023 for 10-bit ADC)
 *
 * @note This function blocks until the conversion is complete
 */
uint16_t ADC_get(t_adc_pin pin);

/**
 * @brief Read multiple ADC values and return the average
 *
 * @param pin The ADC pin to read from (from t_adc_pin enumeration)
 * @param nsamples Number of samples to average (1-255)
 * @return 16-bit averaged ADC result
 *
 * @note This function blocks until all conversions are complete
 * @note Higher sample counts provide better noise reduction but take longer
 * @warning Ensure nsamples > 0 to avoid division by zero
 */
uint16_t ADC_get_avg(t_adc_pin pin, uint8_t nsamples);

#endif  // !ADC_H
