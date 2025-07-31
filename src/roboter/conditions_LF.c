
#include "roboter/conditions_LF.h"
#include "hardware/linienfolger.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

uint8_t condition_forward_to_left(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused))) {
  if (LF_get_state(LF_LEFT) && !LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_forward_to_right(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT) && LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_LMR(t_state *inst __attribute__((unused)),
                         void *vp_dto __attribute__((unused))) {
  if (LF_get_states() == (LF_detection_state)LF_LMR) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_NOT_LMR(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT) || !LF_get_state(LF_MIDDLE) || !LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_LF_NONE(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT) && !LF_get_state(LF_MIDDLE) && !LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_forward_to_backwards(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused))) {
  if (LF_get_states() == (LF_detection_state)LF_NONE) {
    return 1;
  }
  return 0;
}

uint8_t condition_backwards_to_forward(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused))) {
  if (LF_get_states() != LF_NONE) {
    return 1;
  }
  return 0;
}

uint8_t condition_nothing_to_forward(t_state *inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused))) {
  if (LF_get_states() == (LF_detection_state)LF_NONE) {
    return 1;
  }
  return 0;
}

uint8_t condition_left_to_forward(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_LEFT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_left_to_hard_left(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_MIDDLE)) {
    return 1;
  }
  return 0;
}

uint8_t condition_hard_left_to_left(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused))) {
  if (LF_get_state(LF_MIDDLE) == 1) {
    return 1;
  }
  return 0;
}

uint8_t condition_right_to_forward(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_RIGHT)) {
    return 1;
  }
  return 0;
}

uint8_t condition_right_to_hard_right(t_state *inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused))) {
  if (!LF_get_state(LF_MIDDLE)) {
    return 1;
  }
  return 0;
}

uint8_t condition_hard_right_to_right(t_state *inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused))) {
  if (LF_get_state(LF_MIDDLE)) {
    return 1;
  }
  return 0;
}

uint8_t condition_check_for_start_to_forward(t_state *inst __attribute__((unused)),
                                             void *vp_dto __attribute__((unused))) {
  if (LF_get_states() != LF_LMR) {
    return 1;
  }
  return 0;
}
