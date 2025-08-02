/**
 * @file states_config.h
 * @brief Configuration and setup state functions for robot state machine
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef STATES_CONFIG_H
#define STATES_CONFIG_H

#include "state_machine/state.h"

/**
 * @brief Robot initialization state entry function
 * @param inst State instance (unused)
 * @note Initializes all hardware components: shift register, ADC, line follower, engine, clock
 */
void init_robi_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Robot initialization state update function
 * @param inst State instance (unused)
 * @note Currently only logs spam messages during initialization
 */
void init_robi_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Configuration menu state entry function
 * @param inst State instance (unused)
 * @note Prints configuration help menu via PRINT_CONFIG_HELP()
 */
void config_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Configuration menu state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void config_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Rounds configuration state entry function
 * @param inst State instance (unused)
 * @note Prompts user to enter number of rounds via MSG_ROUNDS_ASK_NUMBER
 */
void config_rounds_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Rounds configuration state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void config_rounds_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Line follower static configuration state entry function
 * @param inst State instance for tracking entry time
 * @note Records state entry time for timing-based updates
 */
void config_lf_static_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Line follower static configuration state update function
 * @param inst State instance for timing calculations
 * @note Shows LF state on shift register and prints threshold values every 1000ms
 */
void config_lf_static_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Waiting state entry function
 * @param inst State instance (unused)
 * @note Prints waiting UI, resets current round to 0, initializes timing variables
 */
void waiting_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Waiting state update function
 * @param inst State instance (unused)
 * @note Periodically prints waiting message and blinks LEDs using shift register
 */
void waiting_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Searching state entry function
 * @param inst State instance (unused)
 * @note Prints searching message and initializes timing for periodic updates
 */
void searching_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Searching state update function
 * @param inst State instance (unused)
 * @note Periodically prints search message and updates shift register with LF state changes
 */
void searching_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Reset state entry function
 * @param inst State instance for recording entry time
 * @note Records entry time and prints resting message via MSG_RESTING
 */
void reset_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Reset state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void reset_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Line follower threshold prompt state entry function
 * @param inst State instance (unused)
 * @note Prompts user to enter threshold value via MSG_ENTER_VALUE_LF_TRESHOLD
 */
void lf_set_treshold_prompt_on_entry(t_state *inst __attribute__((unused)));

#endif // STATES_CONFIG_H
