#ifndef ROBOTER_H
#define ROBOTER_H

#include <stdint.h>

typedef struct roboter {
	uint16_t ui16_LF_left_threshold;
	uint16_t ui16_LF_middle_threshold;
	uint16_t ui16_LF_right_threshold;
	uint8_t ui8_LF_ADC_avg_samples;

	int8_t i8_rounds;
} t_roboter;

void ROBOTER_init(t_roboter *robi);
t_roboter *ROBOTER_get_instance();

#endif // !ROBOTER_H
