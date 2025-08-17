// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

#include "roboter/conditions/general.h"

uint8_t condition_allways(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused))) {
  TRACE("[condition_allways] called\n");
  return 1;
}
