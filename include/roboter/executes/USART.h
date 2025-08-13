/**
 * @file execute_USART.h
 * @brief USART execute functions for state machine actions
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef EXECUTE_USART_H
#define EXECUTE_USART_H

#include "state_machine/state.h"

/**
 * @brief Prints waiting help message via USART
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @note Calls PRINT_WAITING_HELP() macro to display help information
 */
void execute_print_waiting_help(t_state *tp_current_state __attribute__((unused)), void *vp_dto);

/**
 * @brief Prints fresh start message via USART
 * @param tp_current_state Current state instance (unused)
 * @param vp_dto Data transfer object (unused)
 * @note Calls UI(MSG_FRESH_START) to display startup message
 */
void execute_print_fresh_start(t_state *tp_current_state __attribute__((unused)), void *vp_dto);

#endif  // EXECUTE_USART_H
