#include "roboter/states_drive.h"
#include "hardware/clock.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"
#include "roboter/roboter.h"
#include "tools/bit_functions.h"

#include "configuration/robot_settings.h"
#include "configuration/serial_messages.h"

// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"
#include <stdint.h>

static uint32_t ui32_DRIVE_MSG_time_last_ms = 0;

void drive_logic_super_state_on_entry(t_state *inst __attribute__((unused))) {
  INFO("drive_logic_super_state_on_entry\n");
  return;
}

void drive_logic_super_state_on_update(t_state *inst __attribute__((unused))) {
  if (CLOCK_get_milliseconds() - ui32_DRIVE_MSG_time_last_ms >
      DRIVING_MSG_PERIOD_MS) {
    UI(MSG_FMT_DRIVING_ROUND, ROBOTER_get_instance()->i8_current_round);
    ui32_DRIVE_MSG_time_last_ms = CLOCK_get_milliseconds();
  }
  return;
}

void drive_through_start_on_entry(t_state *inst __attribute__((unused))) {
  t_roboter *tp_robi = ROBOTER_get_instance();
  tp_robi->i8_current_round++;
  UI("Starting round %u/%u\n\n", tp_robi->i8_current_round,
     tp_robi->i8_max_rounds);

  SHIFT_push_state((LF_detection_state)LF_LMR);

  INFO("drive_through_start\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, SETTING_ENGINE_PWM_POWER_FORWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, SETTING_ENGINE_PWM_POWER_FORWARD);

  // Left motors forward
  SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Right motors forward
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void drive_through_start_on_entry_on_update(t_state *inst
                                            __attribute__((unused))) {
  return;
}

void forward_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_M);
  INFO("forward\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, SETTING_ENGINE_PWM_POWER_FORWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, SETTING_ENGINE_PWM_POWER_FORWARD);

  // Left motors forward
  SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Right motors forward
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void forward_on_update(t_state *inst __attribute__((unused))) { return; }

void backwards_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_M);
  INFO("backwards\n");

  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
  return;
}

void backwards_on_update(t_state *inst __attribute__((unused))) {
  if (CLOCK_get_milliseconds() - *inst->ui32p_state_entry_time_ms >
      BACKWAD_DELAY_TRESHOLD) {

    ENGINE_set_duty_cicle(
        ENGINE_PWM_LEFT,
        SETTING_ENGINE_PWM_POWER_BACKWARD); // set to 3/4 speed
    ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, SETTING_ENGINE_PWM_POWER_BACKWARD);

    // Left motors forward
    UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
    SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
    // Right motors forward
    SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
    UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  }
  return;
}

void left_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_LM);
  INFO("left\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_TURN_FORWARD_LOW);
  ENGINE_set_duty_cicle(
      ENGINE_PWM_LEFT,
      SETTING_ENGINE_PWM_POWER_TURN_FORWARD); // set to 7/8 speed

  // Left motors backwards
  UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Right motors forward
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void left_on_update(t_state *inst __attribute__((unused))) { return; }

void hard_left_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_L);
  INFO("hard left\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT,
                        SETTING_ENGINE_PWM_POWER_TURN_BACKWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_TURN_FORWARD);

  // Left motors backwards
  UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);

  // Right motors forward
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
}

void hard_left_on_update(t_state *inst __attribute__((unused))) { return; }

void right_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_MR);
  INFO("right\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_TURN_FORWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT,
                        SETTING_ENGINE_PWM_POWER_TURN_FORWARD_LOW);

  // Left motors forward
  SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Right motors backwards
  SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void right_on_update(t_state *inst __attribute__((unused))) { return; }

void hard_right_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_R);
  INFO("hard right\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, SETTING_ENGINE_PWM_POWER_TURN_FORWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_TURN_BACKWARD);

  // Left motors forward
  SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);

  // Right motors backwards
  SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void hard_right_on_update(t_state *inst __attribute__((unused))) { return; }

void check_is_start_field_on_entry(t_state *inst __attribute__((unused))) {
  INFO("check is start\n");
  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();

  SHIFT_push_state((LF_detection_state)LF_M);

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, SETTING_ENGINE_PWM_POWER_FORWARD);
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, SETTING_ENGINE_PWM_POWER_FORWARD);

  // Left motors forward
  SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Right motors forward
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  return;
}

void check_is_start_field_on_update(t_state *inst __attribute__((unused))) {
  return;
}

void stop_on_entry(t_state *inst __attribute__((unused))) {
  SHIFT_push_state((LF_detection_state)LF_LMR);
  INFO("stop\n");

  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT,
                        SETTING_ENGINE_PWM_POWER_MAX); // set to full speed
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_MAX); // set to full speed

  // Stop left motors
  UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Stop right motors
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);

  // print special round ending
  t_roboter *tp_robi = ROBOTER_get_instance();
  switch (tp_robi->i8_current_round) { // when round finished
  case 1:
    UI(MSG_ROUND_1_COMPLETE);
    break;
  case 2:
    UI(MSG_ROUND_2_COMPLETE);
    break;
  case 3:
    UI(MSG_ROUND_3_COMPLETE);
    break;
  }

  return;
}

void stop_on_update(t_state *inst __attribute__((unused))) { return; }

static uint32_t ui32_PAUSE_SHIFT_time_last_ms = 0;
static uint8_t ui8_PAUSE_SHIFT_state = 0;

static uint32_t ui32_PAUSE_MSG_time_last_ms = 0;

void pause_on_entry(t_state *inst __attribute__((unused))) {
  ENGINE_set_duty_cicle(ENGINE_PWM_LEFT,
                        SETTING_ENGINE_PWM_POWER_MAX); // set to full speed
  ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT,
                        SETTING_ENGINE_PWM_POWER_MAX); // set to full speed

  // Stop left motors
  UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
  UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
  // Stop right motors
  UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
  UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
  SHIFT_push_state(0);
  return;
}

void pause_on_update(t_state *inst __attribute__((unused))) {

  if (CLOCK_get_milliseconds() - ui32_PAUSE_MSG_time_last_ms >
      PAUSE_MSG_PERIOD_MS) {
    UI(MSG_PAUSING);
    ui32_PAUSE_MSG_time_last_ms = CLOCK_get_milliseconds();
  }

  if (CLOCK_get_milliseconds() - ui32_PAUSE_SHIFT_time_last_ms >
      PAUSE_SHIFT_PERIOD_MS) {
    if (ui8_PAUSE_SHIFT_state % 3 == 0) {
      SHIFT_push(1);
      ui8_PAUSE_SHIFT_state = 1;
    } else {
      SHIFT_push(0);
      ui8_PAUSE_SHIFT_state++;
    }
    ui32_PAUSE_SHIFT_time_last_ms = CLOCK_get_milliseconds();
  }
}
