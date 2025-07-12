#include "roboter/conditions_roboter.h"
#include "roboter/roboter.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_has_rounds(t_state *inst __attribute__((unused))) {
	TRACE("[condition_has_rounds] called");
	if (ROBOTER_get_instance()->i8_rounds > 0) {
		return 1;
	}
	return 0;
}

uint8_t condition_has_no_rounds(t_state *inst __attribute__((unused))) {
	TRACE("[condition_has_rounds] called");
	if (ROBOTER_get_instance()->i8_rounds <= 0) {
		return 1;
	}
	return 0;
}
