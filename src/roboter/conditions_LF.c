
#include "roboter/conditions_LF.h"
#include "hardware/linienfolger.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_LF_L_X_nR(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused))) {
  if (LF_get_state(LF_LEFT) && !LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_nL_X_R(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT) && LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_L_M_R(t_state *inst __attribute__((unused)),
                         void *vp_dto __attribute__((unused))) {
  if (LF_get_states() == (LF_detection_state)LF_LMR) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_NEITHER_L_M_R(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT) || !LF_get_state(LF_MIDDLE) || !LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_nL_nM_nR(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused))) {
  if (LF_get_states() == (LF_detection_state)LF_NONE) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_ANY(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused))) {
  if (LF_get_states() != LF_NONE) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_nL_X_X(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_X_nM_X(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_MIDDLE)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_X_X_nR(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_X_M_X(t_state *inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused))) {
  if (LF_get_state(LF_MIDDLE)) {
    return 1;
  }
  return 0;
}
