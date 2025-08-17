/**
 * @file general.h
 * @brief General utility state functions for robot state machine
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef STATES_GENERAL_H
#define STATES_GENERAL_H

#include "state_machine/state.h"

/**
 * @brief Error state entry function
 * @param inst State instance (unused)
 * @note Sets LF_NONE on shift register, stops all motors, sets full PWM power for emergency stop
 */
void error_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Error state update function
 * @param inst State instance (unused)
 * @note Continuously prints FATAL error messages to indicate system error condition
 */
void error_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief No-operation state entry function
 * @param inst State instance (unused)
 * @note Empty function for states that require no entry actions
 */
void nothing_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief No-operation state update function
 * @param inst State instance (unused)
 * @note Empty function for states that require no update actions
 */
void nothing_on_update(t_state *inst __attribute__((unused)));

#endif  // STATES_GENERAL_H
