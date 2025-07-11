#include "roboter/states_config.h"
#include "hardware/clock.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

void init_robi_on_entry(t_state *inst __attribute__((unused))) {
	INFO("init\n");
	SHIFT_init();
	LF_init();
	ENGINE_init();
	CLOCK_init();
	return;
}

void init_robi_on_update(t_state*inst __attribute__((unused))) {
	return;
}
