/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef CONDITIONS_CLOCK_H
#define CONDITIONS_CLOCK_H

#include "state_machine/state.h"

uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused)));

uint8_t condition_5_seconds_after_entry(t_state *inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_CLOCK_H
