/**
 * @file execute_config.h
 * @brief Configuration execute functions for robot parameter setting
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef EXECUTE_CONFIG_H
#define EXECUTE_CONFIG_H

#include "state_machine/state.h"

/**
 * @brief Sets the left line follower sensor threshold value
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Pointer to uint16_t value containing new threshold
 * @note Updates ROBOTER_get_instance()->ui16_LF_left_threshold and prints confirmation
 */
void execute_set_robi_lf_l_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

/**
 * @brief Sets the middle line follower sensor threshold value
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Pointer to uint16_t value containing new threshold
 * @note Updates ROBOTER_get_instance()->ui16_LF_middle_threshold and prints confirmation
 */
void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

/**
 * @brief Sets the right line follower sensor threshold value
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Pointer to uint16_t value containing new threshold
 * @note Updates ROBOTER_get_instance()->ui16_LF_right_threshold and prints confirmation
 */
void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

/**
 * @brief Sets the maximum number of rounds for the robot to complete
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Pointer to uint8_t value containing new round count
 * @note Updates ROBOTER_get_instance()->i8_max_rounds and prints confirmation
 */
void execute_set_robi_rounds(t_state *tp_current_state __attribute__((unused)), void *vp_dto __attribute__((unused)));

#endif  // EXECUTE_CONFIG_H
