#include "hardware/adc.h"
#include "tools/bit_functions.h"
#include <avr/io.h>
#include <stdint.h>

/**
 * @brief Initialize a specific ADC pin for input
 * 
 * Configures the specified pin as an input for ADC operations by clearing
 * the corresponding bit in the data direction register.
 * 
 * @param pin The ADC pin to initialize (from t_adc_pin enumeration)
 * 
 * @note This is a macro that expands to UNSET_BIT(adc_reg, adc_pin_bit)
 */
#define _ADC_init_pin(adc_reg, adc_pin_bit) UNSET_BIT(adc_reg, adc_pin_bit)

void ADC_init() {
  ADC_MUX_STEUER_REG = 0;
  SET_BIT(ADC_MUX_STEUER_REG, REFS0);  // set AVcc as ADC voltage source

  // set how fast and accurate the ADC is running
  ADCSRA = 0;
  SET_BIT(ADCSRA, ADPS0);
  SET_BIT(ADCSRA, ADPS1);
  SET_BIT(ADCSRA, ADPS2);

  SET_BIT(ADCSRA, ADEN);  // enables ADC

  while (IS_ADC_MEASURING) {
    // wait for complete measurment
  }
  ADC_READ;  // read to destroy firs inacurate measurment
}

void ADC_init_pin(t_adc_pin pin) {
  switch (pin) {
  case ADC_LF_LEFT:
    _ADC_init_pin(ADC0_DDR, ADC0_BIT);
    break;
  case ADC_LF_MIDDLE:
    _ADC_init_pin(ADC1_DDR, ADC1_BIT);
    break;
  case ADC_LF_RIGHT:
    _ADC_init_pin(ADC2_DDR, ADC2_BIT);
    break;
  case A3_Voltage_Divider_LiPo:
    _ADC_init_pin(ADC3_DDR, ADC3_BIT);
    break;
  case C4:
    _ADC_init_pin(ADC4_DDR, ADC4_BIT);
    break;
  case C5:
    _ADC_init_pin(ADC5_DDR, ADC5_BIT);
    break;
  }
}

uint16_t ADC_get(t_adc_pin pin) {
  UNSET_BIT(ADC_MUX_STEUER_REG, 0);
  UNSET_BIT(ADC_MUX_STEUER_REG, 1);
  UNSET_BIT(ADC_MUX_STEUER_REG, 2);
  UNSET_BIT(ADC_MUX_STEUER_REG, 3);
  ADC_MUX_STEUER_REG |= pin;  // sets more than just one bit

  SET_BIT(ADCSRA, ADSC);  // Start read
  while (IS_BIT_SET(ADCSRA, ADSC)) {
    // wait for complete measurment
  }
  return ADC_READ;
}

uint16_t ADC_get_avg(t_adc_pin pin, uint8_t nsamples) {
  if (nsamples == 0) {  // prevent division by zero
    return 0;
  }

  uint32_t sum = 0;
  for (uint8_t i = 0; i < nsamples; i++) {
    sum += ADC_get(pin);
  }
  return (uint16_t)(sum / (float)nsamples);
}
