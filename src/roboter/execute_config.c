#include "roboter/execute_config.h"
#include "roboter/roboter.h"
#include <stdint.h>

void execute_set_robi_lf_l_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	uint16_t value = (uint16_t)vp_dto;
	ROBOTER_get_instance()->ui16_LF_left_threshold = value;
}

void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	uint16_t value = (uint16_t)vp_dto;
	ROBOTER_get_instance()->ui16_LF_middle_threshold = value;
}

void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	uint16_t value = (uint16_t)vp_dto;
	ROBOTER_get_instance()->ui16_LF_right_threshold = value;
}
