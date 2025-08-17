// #define LOG_LEVEL LOG_LEVEL_INFO_TRACE
#include "tools/logger.h"

#include "roboter/states/config.h"
#include "configuration/robot_settings.h"
#include "configuration/serial_messages.h"
#include "roboter/roboter_model.h"

#include "hardware/adc.h"
#include "hardware/clock.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"

#include <stdint.h>
#include <util/delay.h>

void init_robi_on_entry(t_state *inst __attribute__((unused))) {
  INFO("init\n");
  SHIFT_init();
  ADC_init();
  LF_init();
  ENGINE_init();
  CLOCK_init();
  return;
}

void init_robi_on_update(t_state *inst __attribute__((unused))) {
  INFO_SPAM("init update\n");
  return;
}

void config_on_entry(t_state *inst __attribute__((unused))) {
  INFO("config\n");
  PRINT_CONFIG_HELP();
  return;
}

void config_on_update(t_state *inst __attribute__((unused))) { return; }

void config_rounds_on_entry(t_state *inst __attribute__((unused))) {
  INFO("config_rounds");
  UI(MSG_ROUNDS_ASK_NUMBER);
  return;
}

void config_rounds_on_update(t_state *inst __attribute__((unused))) { return; }

void config_lf_static_on_entry(t_state *inst __attribute__((unused))) {
  INFO("config_lf_static_on_entry");
  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
  return;
}

void config_lf_static_on_update(t_state *inst __attribute__((unused))) {
  // show LF state in shift register to see what you configure
  SHIFT_push_state(LF_get_states());

  uint32_t delay = 1000;
  if (CLOCK_get_milliseconds() - *inst->ui32p_state_entry_time_ms > delay) {
    INFO("config_lf_static_rounds");
    PRINT_CONFIG_LINE_FOLLOWER_THRESHOLDS();
    UI("Left  : '%d' - Middle: '%d' - Right : '%d'\n\n", ADC_get_avg(ADC_LF_LEFT, ADC_AVG_WINDOW),
       ADC_get_avg(ADC_LF_MIDDLE, ADC_AVG_WINDOW), ADC_get_avg(ADC_LF_RIGHT, ADC_AVG_WINDOW));
    *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
  }
  return;
}

// WAITING state

static uint32_t ui32_WAITING_LIGHTS_last_update_time_ms = 0;
static uint8_t b_WAITING_lights_on_toggle = 0;

static uint32_t ui32_WAITING_MSG_last_update_time_ms = 0;

void waiting_on_entry(t_state *inst __attribute__((unused))) {
  INFO("[waiting_on_entry]'\n");
  UI(MSG_WAITING_UI);

  t_roboter *tp_robi = ROBOTER_get_instance();
  tp_robi->i8_current_round = 0;

  ui32_WAITING_LIGHTS_last_update_time_ms = CLOCK_get_milliseconds();
  ui32_WAITING_MSG_last_update_time_ms = CLOCK_get_milliseconds();
  b_WAITING_lights_on_toggle = 0;
  return;
}

void waiting_on_update(t_state *inst __attribute__((unused))) {
  INFO_SPAM("[waiting_on_update]'\n");
  // print UI
  if ((CLOCK_get_milliseconds() - ui32_WAITING_MSG_last_update_time_ms) > WAITING_MSG_PERIOD_MS) {
    ui32_WAITING_MSG_last_update_time_ms = CLOCK_get_milliseconds();
    UI(MSG_WAITING_UI);
  }

  // Blink lights
  if ((CLOCK_get_milliseconds() - ui32_WAITING_LIGHTS_last_update_time_ms) >
      SETTING_WAITING_BLINK_PERIOD_MS) {
    ui32_WAITING_LIGHTS_last_update_time_ms = CLOCK_get_milliseconds();
    b_WAITING_lights_on_toggle = !b_WAITING_lights_on_toggle;

    // push new state
    for (uint8_t i = 3; i > 0; i--) {
      SHIFT_push(b_WAITING_lights_on_toggle);
      _delay_us(1);
    }
  }
  return;
}

// END WAITING state

// SEARCHING state

static uint32_t ui32_SEARCHING_MSG_last_update_time_ms = 0;
static LF_detection_state t_LF_old_state = ~0;

void searching_on_entry(t_state *inst __attribute__((unused))) {
  UI(MSG_SEARCHING);
  ui32_SEARCHING_MSG_last_update_time_ms = CLOCK_get_milliseconds();
}

void searching_on_update(t_state *inst __attribute__((unused))) {
  // prints
  if ((CLOCK_get_milliseconds() - ui32_SEARCHING_MSG_last_update_time_ms) >
      SEARCHING_MSG_PERIOD_MS) {
    UI(MSG_SEARCHING);
    ui32_SEARCHING_MSG_last_update_time_ms = CLOCK_get_milliseconds();
  }

  // update LF
  LF_detection_state t_LF_new_state = LF_get_states();
  if (t_LF_old_state != t_LF_new_state) {
    SHIFT_push_state(t_LF_new_state);
    t_LF_old_state = t_LF_new_state;
  }
}

// END SEARCHING state

// RESETT state

void reset_on_entry(t_state *inst __attribute__((unused))) {
  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
  t_roboter *tp_robi = ROBOTER_get_instance();
  tp_robi->b_round_timing = 0;
  UI(MSG_RESTING);
  return;
}

void reset_on_update(t_state *inst __attribute__((unused))) { return; }


void lf_set_treshold_prompt_on_entry(t_state *inst __attribute__((unused))) {
	UI(MSG_ENTER_VALUE_LF_TRESHOLD);
}

// end RESETT state
