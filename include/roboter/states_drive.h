/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef STATES_DRIVE_H
#define STATES_DRIVE_H

#include "state_machine/state.h"

void drive_logic_super_state_on_entry(t_state *inst __attribute__((unused)));

void drive_logic_super_state_on_update(t_state *inst __attribute__((unused)));

void drive_through_start_on_entry(t_state *inst __attribute__((unused)));

void drive_through_start_on_entry_on_update(t_state *inst __attribute__((unused)));

void forward_on_entry(t_state *inst __attribute__((unused)));

void forward_on_update(t_state *inst __attribute__((unused)));

void backwards_on_entry(t_state *inst __attribute__((unused)));

void backwards_on_update(t_state *inst __attribute__((unused)));

void left_on_entry(t_state *inst __attribute__((unused)));

void left_on_update(t_state *inst __attribute__((unused)));

void hard_left_on_entry(t_state *inst __attribute__((unused)));

void hard_left_on_update(t_state *inst __attribute__((unused)));

void right_on_entry(t_state *inst __attribute__((unused)));

void right_on_update(t_state *inst __attribute__((unused)));

void hard_right_on_entry(t_state *inst __attribute__((unused)));

void hard_right_on_update(t_state *inst __attribute__((unused)));

void check_is_start_field_on_entry(t_state *inst __attribute__((unused)));

void check_is_start_field_on_update(t_state *inst __attribute__((unused)));

void stop_on_entry(t_state *inst __attribute__((unused)));

void stop_on_update(t_state *inst __attribute__((unused)));

void pause_on_entry(t_state *inst __attribute__((unused)));

void pause_on_update(t_state *inst __attribute__((unused)));

#endif  // STATES_DRIVE_H
