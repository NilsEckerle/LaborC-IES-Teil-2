// #define LOG_LEVEL LOG_LEVEL_INFO_TRACE
#include "tools/logger.h"

#include "tools/math_helper_functions.h"

#include "configuration/robot_settings.h"
#include "configuration/serial_messages.h"
#include "roboter/roboter_model.h"
#include "roboter/states/config.h"

#include "hardware/adc.h"
#include "hardware/clock.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
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
      WAITING_BLINK_PERIOD_MS) {
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

// LF AUTO

static uint16_t *ui16_prev_adc_measurement_left = NULL;
static uint16_t *ui16_prev_adc_measurement_middle = NULL;
static uint16_t *ui16_prev_adc_measurement_right = NULL;

void config_lf_auto_on_entry(t_state *inst __attribute__((unused))) {
  PRINT_CONFIG_LF_AUTO_HELP();
  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
  SHIFT_push(0);
  SHIFT_push(0);
  SHIFT_push(0);
}

void config_lf_auto_on_update(t_state *inst __attribute__((unused))) {
  // take new measurement
  uint16_t ui16_adc_measurement_left = ADC_get_avg((t_adc_pin)ADC_LF_LEFT, ADC_AVG_WINDOW);
  uint16_t ui16_adc_measurement_middle = ADC_get_avg((t_adc_pin)ADC_LF_MIDDLE, ADC_AVG_WINDOW);
  uint16_t ui16_adc_measurement_right = ADC_get_avg((t_adc_pin)ADC_LF_RIGHT, ADC_AVG_WINDOW);

  // init globals if not already
  if (ui16_prev_adc_measurement_left == NULL) {
    ui16_prev_adc_measurement_left = malloc(sizeof(typeof(ui16_prev_adc_measurement_left)));
    if (ui16_prev_adc_measurement_left == NULL) {
      FATAL("[config_lf_auto_on_update] malloc failed for left");
    } else {
      *ui16_prev_adc_measurement_left = (uint16_t)~0;
    }
  }
  if (ui16_prev_adc_measurement_middle == NULL) {
    ui16_prev_adc_measurement_middle = malloc(sizeof(typeof(ui16_prev_adc_measurement_middle)));
    if (ui16_prev_adc_measurement_middle == NULL) {
      FATAL("[config_lf_auto_on_update] malloc failed for middle");
    } else {
      *ui16_prev_adc_measurement_middle = (uint16_t)~0;
    }
  }
  if (ui16_prev_adc_measurement_right == NULL) {
    ui16_prev_adc_measurement_right = malloc(sizeof(typeof(ui16_prev_adc_measurement_right)));
    if (ui16_prev_adc_measurement_right == NULL) {
      FATAL("[config_lf_auto_on_update] malloc failed for right");
    } else {
      *ui16_prev_adc_measurement_right = (uint16_t)~0;
    }
  }

  // update sample if changed
  if ((*ui16_prev_adc_measurement_left == (uint16_t)~0) ||
      (*ui16_prev_adc_measurement_middle == (uint16_t)~0) ||
      (*ui16_prev_adc_measurement_right == (uint16_t)~0) ||
      (ABS(ui16_adc_measurement_left - *ui16_prev_adc_measurement_left) >
       SETTING_LF_CONFIG_AUTO_DIFFERENCE_TRESHOLD) ||
      (ABS(ui16_adc_measurement_middle - *ui16_prev_adc_measurement_middle) >
       SETTING_LF_CONFIG_AUTO_DIFFERENCE_TRESHOLD) ||
      (ABS(ui16_adc_measurement_right - *ui16_prev_adc_measurement_right) >
       SETTING_LF_CONFIG_AUTO_DIFFERENCE_TRESHOLD)) {
    // Then
    *ui16_prev_adc_measurement_left = ui16_adc_measurement_left;
    *ui16_prev_adc_measurement_middle = ui16_adc_measurement_middle;
    *ui16_prev_adc_measurement_right = ui16_adc_measurement_right;
    *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
    SHIFT_push(0);
    SHIFT_push(0);
    SHIFT_push(0);
    return;
  }

  static uint32_t ui32_last_push_time;
  uint32_t ui32_time_elapsed = (CLOCK_get_milliseconds() - ui32_last_push_time);

  if ((ui32_time_elapsed > 1000)) {
    ui32_last_push_time = CLOCK_get_milliseconds();
    SHIFT_push(1);
  }
}

void config_lf_take_sample_on_entry(t_state *inst __attribute__((unused))) {
  *inst->ui32p_state_entry_time_ms = CLOCK_get_milliseconds();
}

void config_lf_take_sample_on_update(t_state *inst __attribute__((unused))) {
  static uint8_t b_push_toggle;
  static uint32_t ui32_toggle_time;

  uint32_t ui32_time_elapsed = (CLOCK_get_milliseconds() - ui32_toggle_time);

  if ((ui32_time_elapsed > LF_TAKE_SAMPLE_BLINK_PERIOD_MS)) {
    b_push_toggle = !b_push_toggle;
    ui32_toggle_time = CLOCK_get_milliseconds();
  }

  SHIFT_push(b_push_toggle);
  SHIFT_push(b_push_toggle);
  SHIFT_push(b_push_toggle);
}

// LF AUTO END

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
  UI(MSG_RESTING);
  return;
}

void reset_on_update(t_state *inst __attribute__((unused))) { return; }

void lf_set_treshold_prompt_on_entry(t_state *inst __attribute__((unused))) {
  UI(MSG_ENTER_VALUE_LF_TRESHOLD);
}

// end RESETT state
