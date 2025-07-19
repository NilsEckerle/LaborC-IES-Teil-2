#ifndef CONDITIONS_CLOCK_H
#define CONDITIONS_CLOCK_H

#include "state_machine/state.h"

#define START_FIELD_DELAY_MS 150

uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)), void *vp_dto);

#endif // CONDITIONS_CLOCK_H
