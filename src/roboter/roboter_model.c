#include "tools/logger.h"

#include "roboter/roboter_model.h"

#include "configuration/robot_settings.h"
#include <stdlib.h>

static t_roboter g_t_roboter_instance;
static uint8_t g_b_roboter_initialized = 0;

void ROBOTER_init(t_roboter *robi) {
  if (NULL == robi) {
    return;
  }

  robi->ui16_LF_left_threshold = SETTING_LF_LEFT_DEFAULT_THRESHOLD;
  robi->ui16_LF_middle_threshold = SETTING_LF_MIDDLE_DEFAULT_THRESHOLD;
  robi->ui16_LF_right_threshold = SETTING_LF_MIDDLE_DEFAULT_THRESHOLD;

  robi->ui8_LF_ADC_avg_samples = SETTING_LF_AVG_SAMPLES;

  robi->i8_current_round = 0;
  robi->i8_max_rounds = SETTING_DEFAULT_ROUNDS;

  robi->b_round_timing = 0;
  robi->ui32_last_lap_time_ms = 0;

  robi->vp_dto = NULL;

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
