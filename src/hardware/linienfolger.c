#include "hardware/adc.h"
#include "roboter/roboter.h"
#include "tools/logger.h"
#include <stdint.h>
#include "hardware/linienfolger.h"
#include "tools/bit_functions.h"

void LF_init() {
	// set DDR as input
  UNSET_BIT(LF_RIGHT_DDR, LF_RIGHT_BIT);
  UNSET_BIT(LF_MIDDLE_DDR, LF_MIDDLE_BIT);
  UNSET_BIT(LF_LEFT_DDR, LF_LEFT_BIT);

	// set input as pull-up (so i need a HIGH to get a 1)
  SET_BIT(LF_RIGHT_PORT, LF_RIGHT_BIT);
  SET_BIT(LF_MIDDLE_PORT, LF_MIDDLE_BIT);
  SET_BIT(LF_LEFT_PORT, LF_LEFT_BIT);

  return;
}

int8_t LF_get_state(LF_index lf_index) {
	t_roboter *robi = ROBOTER_get_instance();
	uint16_t value = 0;
  switch (lf_index) {
  case LF_LEFT:
    value = ADC_get_avg(ADC_LF_LEFT, ADC_AVG_WINDOW);
		break;
  case LF_MIDDLE:
    value = ADC_get_avg(ADC_LF_MIDDLE, ADC_AVG_WINDOW);
		break;
  case LF_RIGHT:
    value = ADC_get_avg(ADC_LF_RIGHT, ADC_AVG_WINDOW);
		break;
  default:
    ERROR("Invalid line follower sensor index: %u\n", lf_index);
		return -1;
  }

	return (uint8_t)((value >= robi->ui8_LF_left_threshold) ? 1 : 0);
}

LF_detection_state LF_bitstring_to_state(uint8_t ui_lf_detection_bitstring) {
	if (ui_lf_detection_bitstring > 7) {
		return (LF_detection_state)LF_UNDEFINED;
	}

	// cast bitstring pattern to LF_detection_state
	return (LF_detection_state)ui_lf_detection_bitstring;
}

LF_detection_state LF_get_states() {
	uint8_t lf_state_bitstring = 0;
	if (LF_get_state(0)) {
		SET_BIT(lf_state_bitstring, 2);
	}
	if (LF_get_state(1)) {
		SET_BIT(lf_state_bitstring, 1);
	}
	if (LF_get_state(2)) {
		SET_BIT(lf_state_bitstring, 0);
	}

  return LF_bitstring_to_state(lf_state_bitstring);
}

