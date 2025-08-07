#include "roboter/states_general.h"
#include "configuration/robot_settings.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"
#include "tools/bit_functions.h"

#include "tools/logger.h"

void error_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_NONE);
  INFO("error\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, SETTING_ENGINE_PWM_POWER_MAX);   // set to full speed
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, SETTING_ENGINE_PWM_POWER_MAX);  // set to full speed

  // Stop left motors
  UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Stop right motors
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void error_on_update(t_state *inst __attribute__((unused))) {
  FATAL("ERROR STATE!\n");
  return;
}

void nothing_on_entry(t_state *inst __attribute__((unused))) { return; }

void nothing_on_update(t_state *inst __attribute__((unused))) { return; }
