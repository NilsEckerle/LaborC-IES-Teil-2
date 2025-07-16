#include "hardware/adc.h"

#include "tools/iesusart.h"
#define LOG_LEVEL LOG_LEVEL_INFO_SPAM
#include "tools/logger.h"

#include <avr/io.h>
#include <stdint.h>

int main() {
	USART_init(UBRR_SETTING);
	INFO("USART_init\n");


	ADC_init();
	ADC_init_pin(ADC_LF_LEFT);
	ADC_init_pin(ADC_LF_MIDDLE);
	ADC_init_pin(ADC_LF_RIGHT);

	while(1) {
		INFO_SPAM("L: %5u M: %5u R: %5u\n", ADC_get_avg(ADC_LF_LEFT, 5), ADC_get_avg(ADC_LF_MIDDLE, 5), ADC_get_avg(ADC_LF_RIGHT, 5));
	}

	return 0;
}
