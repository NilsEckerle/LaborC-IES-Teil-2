/**
 * @file conditions_USART.h
 * @brief USART command condition functions for state machine transitions
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef CONDITIONS_USART_H
#define CONDITIONS_USART_H

#include "state_machine/state.h"

/**
 * @brief Helper function to clear invalid USART input from buffer
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return Always returns 0 (condition never true, used for side effects)
 * @note Consumes USART string on second call to clear invalid input
 */
uint8_t condition_USART_helper_clear_invalid_input(t_state *inst __attribute__((unused)),
                                                   void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "R" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "R" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_R(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "M" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "M" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_M(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "L" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "L" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_L(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "S" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "S" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_S(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "C" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "C" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_C(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "A" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "A" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_A(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "W" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "W" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_W(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "P" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "P" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_P(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "?" or "help" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if help command received, 0 otherwise
 * @note Accepts both "?" and "help" strings, consumes string on match
 */
uint8_t condition_USART_questionmark(t_state *inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received "lfconfigstatic" command
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if "lfconfigstatic" command received, 0 otherwise
 * @note Consumes the string from USART buffer on match
 */
uint8_t condition_USART_lfconfigstatic(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if USART received a numeric string (digits only)
 * @param inst State instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @return 1 if numeric string received, 0 otherwise
 * @note Converts string to integer and stores in robot DTO, consumes string
 */
uint8_t condition_USART_isdigit(t_state *inst __attribute__((unused)),
                                void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_USART_H
