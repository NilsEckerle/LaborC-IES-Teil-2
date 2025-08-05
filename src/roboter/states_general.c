#ifndef GENERATE_STATE_MACHINE_DIAGRAM
#include "roboter/states_general.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"
#include "tools/bit_functions.h"

#include "tools/logger.h"

void error_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_NONE);
  INFO("error\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, 255);   // set to full speed
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, 255);  // set to full speed

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
#else
#include "roboter/states_general.h"

void error_on_entry(t_state *inst __attribute__((unused))) {
}

void error_on_update(t_state *inst __attribute__((unused))) {
}

void nothing_on_entry(t_state *inst __attribute__((unused))) {}

void nothing_on_update(t_state *inst __attribute__((unused))) {}

#endif // !GENERATE_STATE_MACHINE_DIAGRAM
