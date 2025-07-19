#include "roboter/states_config.h"
#include "roboter/roboter.h"

#include "hardware/clock.h"
#include "hardware/engine.h"
#include "hardware/adc.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"

// #define LOG_LEVEL LOG_LEVEL_INFO_SPAM
#include "tools/logger.h"
#include <ctype.h>
#include <stdlib.h>

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
  ROBOTER_get_instance();
  UI("Send char to select: [s]tart, set [r]ounds\n");
  return;
}

void config_on_update(t_state *inst __attribute__((unused))) { return; }

void config_rounds_on_entry(t_state *inst __attribute__((unused))) {
  INFO("config_rounds");
  UI("Send number to set rounds to it. Then [c]onfirm you input.\n");
  return;
}

void config_rounds_on_update(t_state *inst __attribute__((unused))) {
  return;
}

void config_lf_static_on_entry(t_state *inst __attribute__((unused))) {
  INFO("config_lf_static_rounds");
  UI("l {num} - set threshold of left lf\n \
			m {num} - set threshold of middle lf\n \
			r {num} - set threshold of right lf\n");
  return;
}

void config_lf_static_on_update(t_state *inst __attribute__((unused))) {
	// show LF state in shift register to see what you configure
	SHIFT_push_state(LF_get_states());
	UI("Left  : '%d' - Middle: '%d' - Right : '%d'\n", 
			ADC_get_avg(ADC_LF_LEFT, 10),
			ADC_get_avg(ADC_LF_MIDDLE, 10),
			ADC_get_avg(ADC_LF_RIGHT, 10)
			);
	return;
}

void wait_start_on_entry(t_state *inst __attribute__((unused))) {
  INFO("wait_start");
  UI("Send char to select: [s]tart, [c]onfig\n");
  return;
}

void wait_start_on_update(t_state *inst __attribute__((unused))) { return; }
