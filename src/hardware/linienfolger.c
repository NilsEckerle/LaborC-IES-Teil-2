#include "tools/logger.h"
#include <stdint.h>
#include "hardware/linienfolger.h"
#include "tools/bit_functions.h"

void LF_init() {
	// set DDR as input
  UNSET_BIT(LF_0_DDR, LF_0_BIT);
  UNSET_BIT(LF_1_DDR, LF_1_BIT);
  UNSET_BIT(LF_2_DDR, LF_2_BIT);

	// set input as pull-up (so i need a HIGH to get a 1)
  SET_BIT(LF_0_PORT, LF_0_BIT);
  SET_BIT(LF_1_PORT, LF_1_BIT);
  SET_BIT(LF_2_PORT, LF_2_BIT);

  return;
}

int8_t LF_get_state(uint8_t ui_lf_index) {
  switch (ui_lf_index) {
  case 0:
    return (LF_0_PIN & (1 << LF_0_BIT)) ? 1 : 0;
  case 1:
    return (LF_1_PIN & (1 << LF_1_BIT)) ? 1 : 0;
  case 2:
    return (LF_2_PIN & (1 << LF_2_BIT)) ? 1 : 0;
  default:
    ERROR("Invalid line follower sensor index: %u\n", ui_lf_index);
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
		SET_BIT(lf_state_bitstring, 0);
	}
	if (LF_get_state(1)) {
		SET_BIT(lf_state_bitstring, 1);
	}
	if (LF_get_state(2)) {
		SET_BIT(lf_state_bitstring, 2);
	}

  return LF_bitstring_to_state(lf_state_bitstring);
}

