#include "roboter/executes/config.h"
#include "configuration/robot_settings.h"
#include "configuration/serial_messages.h"
#include "hardware/adc.h"
#include "roboter/roboter_model.h"
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
  UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_lf_m_threshold] dto is NULL.\n");
    return;
  }
  uint16_t *value = (uint16_t *)vp_dto;
  ROBOTER_get_instance()->ui16_LF_middle_threshold = *value;
  UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_lf_r_threshold] dto is NULL.\n");
    return;
  }
  uint16_t *value = (uint16_t *)vp_dto;
  ROBOTER_get_instance()->ui16_LF_right_threshold = *value;
  UI(MSG_FMT_LF_TRESHOLD_SET_TO, *value);
}

void execute_set_robi_rounds(t_state *tp_current_state __attribute__((unused)), void *vp_dto) {
  if (vp_dto == NULL) {
    FATAL("[execute_set_robi_rounds] dto is NULL.\n");
    return;
  }
  uint8_t *value = (uint8_t *)vp_dto;
  UI(MSG_FMT_ROUNDS_SET_TO, *value);
  ROBOTER_get_instance()->i8_max_rounds = *value;
}

void execute_set_robi_lf_tresholds(t_state *tp_current_state __attribute__((unused)),
                                   void *vp_dto __attribute__((unused))) {
  uint16_t ui16_adc_measurement_left = ADC_get_avg((t_adc_pin)ADC_LF_LEFT, ADC_AVG_WINDOW) -
                                       SETTING_LF_AUTO_TRESHOLD_PUFFER;
  uint16_t ui16_adc_measurement_middle = ADC_get_avg((t_adc_pin)ADC_LF_MIDDLE, ADC_AVG_WINDOW) -
                                         SETTING_LF_AUTO_TRESHOLD_PUFFER;
  uint16_t ui16_adc_measurement_right = ADC_get_avg((t_adc_pin)ADC_LF_RIGHT, ADC_AVG_WINDOW) -
                                        SETTING_LF_AUTO_TRESHOLD_PUFFER;

  t_roboter *tp_robi = ROBOTER_get_instance();

  uint16_t ui16_new_left = MIN(ui16_adc_measurement_left, tp_robi->ui16_LF_left_threshold);
  uint16_t ui16_new_middle = MIN(ui16_adc_measurement_middle, tp_robi->ui16_LF_middle_threshold);
  uint16_t ui16_new_right = MIN(ui16_adc_measurement_right, tp_robi->ui16_LF_right_threshold);


  tp_robi->ui16_LF_left_threshold = (ui16_new_left < SETTING_LF_MIN_THRESHOLD) ? tp_robi->ui16_LF_left_threshold : ui16_new_left;
  tp_robi->ui16_LF_middle_threshold = (ui16_new_middle < SETTING_LF_MIN_THRESHOLD) ? tp_robi->ui16_LF_middle_threshold : ui16_new_middle;
  tp_robi->ui16_LF_right_threshold = (ui16_new_right < SETTING_LF_MIN_THRESHOLD) ? tp_robi->ui16_LF_right_threshold : ui16_new_right;

  UI("Current LF values:\n");
  UI("LEFT   - MIDDLE - RIGHT\n");
  UI("%4d   - %4d   - %4d\n\n", tp_robi->ui16_LF_left_threshold, tp_robi->ui16_LF_middle_threshold, tp_robi->ui16_LF_right_threshold);
}
