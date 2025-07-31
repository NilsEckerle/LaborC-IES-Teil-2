#include "roboter/execute_config.h"
#include "roboter/roboter.h"
#include "tools/logger.h"
#include <stdint.h>
#include <stdlib.h>

void execute_set_robi_lf_l_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_lf_l_threshold] dto is NULL.\n");
    return;
  }
  uint16_t *value = (uint16_t *)vp_dto;
  ROBOTER_get_instance()->ui16_LF_left_threshold = *value;
  UI("left lf threshold set to: %u\n", *value);
}

void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_lf_m_threshold] dto is NULL.\n");
    return;
  }
  uint16_t *value = (uint16_t *)vp_dto;
  ROBOTER_get_instance()->ui16_LF_middle_threshold = *value;
  UI("middle lf threshold set to: %u\n", *value);
}

void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_lf_r_threshold] dto is NULL.\n");
    return;
  }
  uint16_t *value = (uint16_t *)vp_dto;
  ROBOTER_get_instance()->ui16_LF_right_threshold = *value;
  UI("right lf threshold set to: %u\n", *value);
}

void execute_set_robi_rounds(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_rounds] dto is NULL.\n");
    return;
  }
  uint8_t *value = (uint8_t *)vp_dto;
  UI("rounds set to: %d\n", *value);
  ROBOTER_get_instance()->i8_max_rounds = *value;
}
