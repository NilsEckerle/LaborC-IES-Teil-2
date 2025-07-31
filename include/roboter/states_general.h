/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef STATES_GENERAL_H
#define STATES_GENERAL_H

#include "state_machine/state.h"

void error_on_entry(t_state *inst __attribute__((unused)));

void error_on_update(t_state *inst __attribute__((unused)));

void nothing_on_entry(t_state *inst __attribute__((unused)));

void nothing_on_update(t_state *inst __attribute__((unused)));

#endif  // STATES_GENERAL_H
