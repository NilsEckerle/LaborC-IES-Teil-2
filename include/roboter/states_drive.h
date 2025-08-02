/**
 * @file states_drive.h
 * @brief Driving state functions for robot movement and line following
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef STATES_DRIVE_H
#define STATES_DRIVE_H

#include "state_machine/state.h"

/**
 * @brief Drive logic super state entry function
 * @param inst State instance (unused)
 * @note Parent state for all driving behaviors
 */
void drive_logic_super_state_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Drive logic super state update function
 * @param inst State instance (unused)
 * @note Periodically prints current round information during driving
 */
void drive_logic_super_state_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Drive through start field state entry function
 * @param inst State instance (unused)
 * @note Increments round counter, prints round info, sets LF_LMR on shift register, drives forward
 */
void drive_through_start_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Drive through start field state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void drive_through_start_on_entry_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Forward movement state entry function
 * @param inst State instance (unused)
 * @note Sets LF_M on shift register, configures both motors for forward movement
 */
void forward_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Forward movement state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void forward_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Backward movement state entry function
 * @param inst State instance for timing
 * @note Sets LF_M on shift register, records entry time for delayed motor activation
 */
void backwards_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Backward movement state update function
 * @param inst State instance for timing calculations
 * @note Activates backward motor movement after BACKWAD_DELAY_TRESHOLD delay
 */
void backwards_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Left turn state entry function
 * @param inst State instance (unused)
 * @note Sets LF_LM on shift register, left motor backward/right motor forward for gentle left turn
 */
void left_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Left turn state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void left_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Hard left turn state entry function
 * @param inst State instance (unused)
 * @note Sets LF_L on shift register, aggressive left turn with full backward/forward motor speeds
 */
void hard_left_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Hard left turn state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void hard_left_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Right turn state entry function
 * @param inst State instance (unused)
 * @note Sets LF_MR on shift register, left motor forward/right motor backward for gentle right turn
 */
void right_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Right turn state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void right_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Hard right turn state entry function
 * @param inst State instance (unused)
 * @note Sets LF_R on shift register, aggressive right turn with full forward/backward motor speeds
 */
void hard_right_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Hard right turn state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void hard_right_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Check start field state entry function
 * @param inst State instance for timing
 * @note Records entry time, sets LF_M on shift register, drives forward to check for start field
 */
void check_is_start_field_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Check start field state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void check_is_start_field_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Stop state entry function
 * @param inst State instance (unused)
 * @note Sets LF_LMR on shift register, stops all motors, prints round completion messages
 */
void stop_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Stop state update function
 * @param inst State instance (unused)
 * @note Currently no update behavior implemented
 */
void stop_on_update(t_state *inst __attribute__((unused)));

/**
 * @brief Pause state entry function
 * @param inst State instance (unused)
 * @note Stops all motors and clears shift register
 */
void pause_on_entry(t_state *inst __attribute__((unused)));

/**
 * @brief Pause state update function
 * @param inst State instance (unused)
 * @note Periodically prints pause message and creates blinking LED pattern on shift register
 */
void pause_on_update(t_state *inst __attribute__((unused)));

#endif  // STATES_DRIVE_H
