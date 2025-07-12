#ifndef ROBOTER_H
#define ROBOTER_H

#include <stdint.h>

typedef struct roboter {
	uint8_t ui8_LF_left_threshold;
	uint8_t ui8_LF_middle_threshold;
	uint8_t ui8_LF_right_threshold;

	int8_t i8_rounds;

	unsigned char uc_usart_input;
} t_roboter;

void ROBOTER_init(t_roboter *robi);
t_roboter *ROBOTER_get_instance();

#endif // !ROBOTER_H
