#include "roboter/roboter.h"

#include "tools/logger.h"

static t_roboter g_t_roboter_instance;
static uint8_t g_b_roboter_initialized = 0;

void ROBOTER_init(t_roboter *robi) {
	if (NULL == robi) {
		return;
	}

	robi->ui8_LF_left_threshold = 1023/2;
	robi->ui8_LF_middle_threshold = 1023/2;
	robi->ui8_LF_right_threshold = 1023/2;

	robi->i8_rounds = 1;

	TRACE("[ROBOTER_init] roboter initialized.\n");

	return;
}

t_roboter *ROBOTER_get_instance() {
	if (!g_b_roboter_initialized) {
		ROBOTER_init(&g_t_roboter_instance);
		g_b_roboter_initialized = 1;
	}

	INFO("[ROBOTER_get_instance] returning.\n");
	return &g_t_roboter_instance;
}
