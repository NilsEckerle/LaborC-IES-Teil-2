#ifndef GENERATE_STATE_MACHINE_DIAGRAM
#include "roboter/conditions_general.h"

// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

uint8_t condition_allways(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused))) {
  TRACE("[condition_allways] called\n");
  return 1;
}
#else
#include "roboter/conditions_general.h"

uint8_t condition_allways(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused))) {
  return 0;
}
#endif // !GENERATE_STATE_MACHINE_DIAGRAM
