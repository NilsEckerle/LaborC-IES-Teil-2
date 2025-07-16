#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include "tools/bit_functions.h"

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

#define IS_ADC_MEASURING IS_BIT_SET(ADCSRA, ADSC)

typedef enum ADC_pin {
	ADC_LF_RIGHT = ADC0_BIT,
	ADC_LF_MIDDLE = ADC1_BIT,
	ADC_LF_LEFT = ADC2_BIT,
	A3_Voltage_Divider_LiPo = ADC3_BIT,
	C4 = ADC4_BIT,
	C5 = ADC5_BIT
} t_adc_pin;

void ADC_init();

#define _ADC_init_pin(adc_reg, adc_pin_bit) UNSET_BIT(adc_reg, adc_pin_bit)

void ADC_init_pin(t_adc_pin pin);

uint16_t ADC_get(t_adc_pin pin);

uint16_t ADC_get_avg(t_adc_pin pin, uint8_t nsamples);

#endif // !ADC_H
