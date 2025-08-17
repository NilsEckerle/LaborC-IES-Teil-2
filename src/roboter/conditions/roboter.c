#include "roboter/conditions/roboter.h"
#include "roboter/roboter_model.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

#include "state_machine/state.h"

uint8_t condition_has_rounds(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused))) {
  TRACE("[condition_has_rounds] called");
  t_roboter *tp_robi = ROBOTER_get_instance();
  if (tp_robi->i8_current_round < tp_robi->i8_max_rounds) {
    return 1;
  }
  return 0;
}

uint8_t condition_has_no_rounds(t_state *inst __attribute__((unused)),
                                void *vp_dto __attribute__((unused))) {
  TRACE("[condition_has_rounds] called");
  t_roboter *tp_robi = ROBOTER_get_instance();
  if (tp_robi->i8_current_round >= tp_robi->i8_max_rounds) {
    return 1;
  }
  return 0;
}
