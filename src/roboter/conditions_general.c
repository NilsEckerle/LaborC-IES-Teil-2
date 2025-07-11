#include "roboter/conditions_general.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_allways(t_state *inst __attribute__((unused))) {
	TRACE("[condition_allways] called");
	return 1;
}
