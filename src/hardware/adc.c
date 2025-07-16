#include "hardware/adc.h"
#include "tools/bit_functions.h"
#include <avr/io.h>
#include <stdint.h>

void ADC_init() {
	ADMUX = 0;
	SET_BIT(ADMUX, REFS0); // set AVcc as ADC voltage source

	// set how fast and accurate the ADC is running
	ADCSRA = 0;
	SET_BIT(ADCSRA, ADPS0);
	SET_BIT(ADCSRA, ADPS1);
	SET_BIT(ADCSRA, ADPS2);

	SET_BIT(ADCSRA, ADEN); // enables ADC

	while (IS_BIT_SET(ADCSRA, ADSC)) {
		// wait for complete measurment
	}
	ADCW; // read to destroy firs inacurate measurment
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

uint16_t ADC_get(t_adc_pin channel) {
	UNSET_BIT(ADMUX, 0);
	UNSET_BIT(ADMUX, 1);
	UNSET_BIT(ADMUX, 2);
	UNSET_BIT(ADMUX, 3);
	ADMUX |= channel; // sets more than just one bit

	SET_BIT(ADCSRA, ADSC); // Start read
	while (IS_BIT_SET(ADCSRA, ADSC)) {
		// wait for complete measurment
	}
	return ADCW;
}

uint16_t ADC_get_avg(t_adc_pin channel, uint8_t nsamples) {
	uint32_t sum = 0;
	for (uint8_t i = 0; i < nsamples; i++) {
		sum += ADC_get(channel);
	}
	return (uint16_t)(sum/(float)nsamples);
}
