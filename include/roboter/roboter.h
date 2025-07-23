#ifndef ROBOTER_H
#define ROBOTER_H

#include <stdint.h>

typedef struct roboter {
	uint16_t ui16_LF_left_threshold;
	uint16_t ui16_LF_middle_threshold;
	uint16_t ui16_LF_right_threshold;
	uint8_t ui8_LF_ADC_avg_samples;

	int8_t i8_rounds;
	void *vp_dto;
} t_roboter;

void ROBOTER_init(t_roboter *robi);
t_roboter *ROBOTER_get_instance();
#define ROBOTER_set_dto(t_data) do { \
	t_roboter *tp_robi = ROBOTER_get_instance(); \
	if (tp_robi->vp_dto != NULL) { \
		free(tp_robi->vp_dto); \
		tp_robi->vp_dto = NULL; \
	} \
	void *vp_data = malloc(sizeof(typeof(t_data))); \
	typeof(t_data) t_data_copy = (t_data); \
	memcpy(vp_data, &t_data_copy, sizeof(typeof(t_data_copy))); \
	tp_robi = vp_data; \
} while (0)

#endif // !ROBOTER_H
