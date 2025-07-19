#ifndef CONDITIONS_ROBOTER_H
#define CONDITIONS_ROBOTER_H

#include "state_machine/state.h"

uint8_t condition_has_rounds(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

uint8_t condition_has_no_rounds(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

#endif // CONDITIONS_ROBOTER_H
