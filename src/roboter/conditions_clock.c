#include "roboter/conditions_clock.h"
#include "hardware/clock.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused))) {
	TRACE("[condition_start_field_delay] called");
	if (CLOCK_get_milliseconds() - *inst->ui32p_state_entry_time_ms >= START_FIELD_DELAY_MS) {
		return 1;
	}
	return 0;
}

