/**
 * @file conditions_general.h
 * @brief General utility condition functions for state machine transitions
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef CONDITIONS_GENERAL_H
#define CONDITIONS_GENERAL_H

#include "state_machine/state.h"

/**
 * @brief Condition function that always returns true
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return Always returns 1 (condition is always true)
 * @note Used for unconditional state transitions or as a fallback condition
 */
uint8_t condition_allways(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_GENERAL_H
