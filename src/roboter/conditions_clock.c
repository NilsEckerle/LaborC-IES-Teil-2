#ifndef GENERATE_STATE_MACHINE_DIAGRAM
#include "roboter/conditions_clock.h"
#include "configuration/robot_settings.h"
#include "hardware/clock.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused))) {
  TRACE("[condition_start_field_delay] called");
  if (CLOCK_get_milliseconds() - *inst->ui32p_state_entry_time_ms >= SETTING_START_FIELD_DELAY_MS) {
    return 1;
  }
  return 0;
}

uint8_t condition_5_seconds_after_entry(t_state *inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused))) {
  TRACE("[condition_5_seconds_after_entry] called");
  if (CLOCK_get_milliseconds() - *inst->ui32p_state_entry_time_ms >= 5000) {
    return 1;
  }
  return 0;
}
#else
#include "roboter/conditions_clock.h"

uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused))) {
}

uint8_t condition_5_seconds_after_entry(t_state *inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused))) {
}
#endif // !GENERATE_STATE_MACHINE_DIAGRAM
