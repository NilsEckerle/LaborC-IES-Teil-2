#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include "tools/bit_functions.h"

#define ADC_AVG_WINDOW 10

#define ADC_MUX_STEUER_REG ADMUX
#define ADC_READ ADCW

#define ADC0_DDR DDRC
#define ADC0_PIN PINC
#define ADC0_BIT PINC0

#define ADC1_DDR DDRC
#define ADC1_PIN PINC
#define ADC1_BIT PINC1

#define ADC2_DDR DDRC
#define ADC2_PIN PINC
#define ADC2_BIT PINC2

#define ADC3_DDR DDRC
#define ADC3_PIN PINC
#define ADC3_BIT PINC3

#define ADC4_DDR DDRC
#define ADC4_PIN PINC
#define ADC4_BIT PINC4

#define ADC5_DDR DDRC
#define ADC5_PIN PINC
#define ADC5_BIT PINC5

/** @brief Check if ADC is currently measuring
 *  @return true if ADC conversion is in progress, false otherwise
 */
#define IS_ADC_MEASURING IS_BIT_SET(ADCSRA, ADSC)

/**
 * @brief ADC pin enumeration
 *
 * Enumeration defining available ADC pins with their specific functions.
 * Each pin corresponds to a specific ADC channel and application.
 */
typedef enum ADC_pin {
	ADC_LF_RIGHT = ADC0_BIT,
	ADC_LF_MIDDLE = ADC1_BIT,
	ADC_LF_LEFT = ADC2_BIT,
	A3_Voltage_Divider_LiPo = ADC3_BIT,
	C4 = ADC4_BIT,
	C5 = ADC5_BIT
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

#endif // !ADC_H
