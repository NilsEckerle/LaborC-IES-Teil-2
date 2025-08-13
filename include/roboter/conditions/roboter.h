/**
 * @file conditions_roboter.h
 * @brief Robot-specific condition functions for state machine transitions
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef CONDITIONS_ROBOTER_H
#define CONDITIONS_ROBOTER_H

#include "state_machine/state.h"

/**
 * @brief Checks if robot has remaining rounds to complete
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if current_round < max_rounds, 0 otherwise
 * @note Uses ROBOTER_get_instance()->i8_current_round and i8_max_rounds
 */
uint8_t condition_has_rounds(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot has completed all rounds
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if current_round >= max_rounds, 0 otherwise
 * @note Uses ROBOTER_get_instance()->i8_current_round and i8_max_rounds
 */
uint8_t condition_has_no_rounds(t_state *inst __attribute__((unused)),
                                void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_ROBOTER_H
