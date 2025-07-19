#include "roboter/conditions_general.h"

// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

uint8_t condition_allways(t_state *inst __attribute__((unused)), void *vp_dto) {
	TRACE("[condition_allways] called\n");
	return 1;
}
