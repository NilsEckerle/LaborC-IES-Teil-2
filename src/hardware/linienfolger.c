#include "tools/logger.h"
#include <stdint.h>
#include "hardware/linienfolger.h"

void LF_init() {
	// set DDR as input
  LF_0_DDR &= ~(1 << LF_0_BIT);
  LF_1_DDR &= ~(1 << LF_1_BIT);
  LF_2_DDR &= ~(1 << LF_2_BIT);

	// set input as pull-up (so i need a HIGH to get a 1)
  LF_0_PORT |= (1 << LF_0_BIT);
  LF_1_PORT |= (1 << LF_1_BIT);
  LF_2_PORT |= (1 << LF_2_BIT);

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
	// get new sensor readings
  TRACE("Reading all line follower sensor states\n");
  int8_t i_lf0_state = LF_get_state(0);
  int8_t i_lf1_state = LF_get_state(1);
  int8_t i_lf2_state = LF_get_state(2);

  TRACE("LF0 state: %i\n", i_lf0_state);
  TRACE("LF1 state: %i\n", i_lf1_state);
  TRACE("LF2 state: %i\n", i_lf2_state);

  // Validate all sensor readings
  if (i_lf0_state < 0 || i_lf1_state < 0 || i_lf2_state < 0) {
    ERROR("Reading line sensor states failed - sensor errors detected\n");
    return (LF_detection_state)LF_UNDEFINED;
  }

  // Store valid results in output array
	uint8_t lf_state_bitstring = 0;
  lf_state_bitstring |= (i_lf0_state << 0);
  lf_state_bitstring |= (i_lf1_state << 1);
  lf_state_bitstring |= (i_lf2_state << 2);

  return LF_bitstring_to_state(lf_state_bitstring);
}

