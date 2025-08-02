/**
 * @file conditions_LF.h
 * @brief Line follower condition functions for state machine transitions
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef CONDITIONS_LF_H
#define CONDITIONS_LF_H

#include "state_machine/state.h"

/**
 * @brief Checks if robot should transition from forward movement to left turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if left turn condition is met, 0 otherwise
 * @note Based on line follower sensor readings indicating line deviation to left
 */
uint8_t condition_forward_to_left(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from forward movement to right turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if right turn condition is met, 0 otherwise
 * @note Based on line follower sensor readings indicating line deviation to right
 */
uint8_t condition_forward_to_right(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if all line follower sensors detect a line (LMR state)
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if LF_LMR state detected, 0 otherwise
 * @note Indicates wide line, intersection, or start/finish area
 */
uint8_t condition_LF_LMR(t_state *inst __attribute__((unused)),
                         void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if sensors are NOT in LMR state
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if NOT in LF_LMR state, 0 otherwise
 */
uint8_t condition_LF_NOT_LMR(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if no line follower sensors detect a line
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if LF_NONE state detected, 0 otherwise
 * @note May indicate line loss or off-track condition
 */
uint8_t condition_LF_NONE(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from forward to backward movement
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if backward movement condition is met, 0 otherwise
 */
uint8_t condition_forward_to_backwards(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from backward to forward movement
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if forward movement condition is met, 0 otherwise
 */
uint8_t condition_backwards_to_forward(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should start moving forward from stationary state
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if forward movement should begin, 0 otherwise
 */
uint8_t condition_nothing_to_forward(t_state *inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should return to forward movement from left turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if forward movement condition is met, 0 otherwise
 */
uint8_t condition_left_to_forward(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from left turn to hard left turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if hard left turn condition is met, 0 otherwise
 * @note Used when gentle left turn is insufficient to follow line
 */
uint8_t condition_left_to_hard_left(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from hard left to gentle left turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if gentle left turn condition is met, 0 otherwise
 */
uint8_t condition_hard_left_to_left(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should return to forward movement from right turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if forward movement condition is met, 0 otherwise
 */
uint8_t condition_right_to_forward(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from right turn to hard right turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if hard right turn condition is met, 0 otherwise
 * @note Used when gentle right turn is insufficient to follow line
 */
uint8_t condition_right_to_hard_right(t_state *inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should transition from hard right to gentle right turn
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if gentle right turn condition is met, 0 otherwise
 */
uint8_t condition_hard_right_to_right(t_state *inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if robot should begin line following after start condition
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if start-to-forward transition should occur, 0 otherwise
 * @note Typically used after start delay or initialization phase
 */
uint8_t condition_check_for_start_to_forward(t_state *inst __attribute__((unused)),
                                             void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_LF_H
