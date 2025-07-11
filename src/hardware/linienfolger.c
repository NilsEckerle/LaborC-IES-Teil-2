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
  switch (lf_index) {
  case LF_LEFT:
    return (LF_LEFT_PIN & (1 << LF_LEFT_BIT)) ? 1 : 0;
  case LF_MIDDLE:
    return (LF_MIDDLE_PIN & (1 << LF_MIDDLE_BIT)) ? 1 : 0;
  case LF_RIGHT:
    return (LF_RIGHT_PIN & (1 << LF_RIGHT_BIT)) ? 1 : 0;
  default:
    ERROR("Invalid line follower sensor index: %u\n", lf_index);
    break;
  }

  return -1; // return error
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

