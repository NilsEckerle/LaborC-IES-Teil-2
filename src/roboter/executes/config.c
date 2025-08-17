#include "roboter/executes/config.h"
#include "configuration/serial_messages.h"
#include "roboter/roboter_model.h"
#include "tools/logger.h"
#include <stdint.h>
#include <stdlib.h>

void execute_set_robi_lf_l_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	if (vp_dto == NULL) {
		FATAL("[execute_set_robi_lf_l_threshold] dto is NULL.\n");
		return;
	}
	uint16_t *value = (uint16_t *)vp_dto;
	ROBOTER_get_instance()->ui16_LF_left_threshold = *value;
	UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	if (vp_dto == NULL) {
		FATAL("[execute_set_robi_lf_m_threshold] dto is NULL.\n");
		return;
	}
	uint16_t *value = (uint16_t*)vp_dto;
	ROBOTER_get_instance()->ui16_LF_middle_threshold = *value;
	UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	if (vp_dto == NULL) {
		FATAL("[execute_set_robi_lf_r_threshold] dto is NULL.\n");
		return;
	}
	uint16_t *value = (uint16_t*)vp_dto;
	ROBOTER_get_instance()->ui16_LF_right_threshold = *value;
	UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_rounds(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
	if (vp_dto == NULL) {
		FATAL("[execute_set_robi_rounds] dto is NULL.\n");
		return;
	}
	uint8_t *value = (uint8_t *)vp_dto;
	UI(MSG_FMT_ROUNDS_SET_TO, *value);
	ROBOTER_get_instance()->i8_max_rounds = *value;
}

void execute_enable_round_timing(t_state *tp_current_state __attribute__((unused)), void *vp_dto __attribute__((unused))) {
  t_roboter *t_robi = ROBOTER_get_instance();
  t_robi->b_round_timing = 1;
}
