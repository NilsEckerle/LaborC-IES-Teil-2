#ifndef ROBOTER_STATES_H
#define ROBOTER_STATES_H

#include "state_machine/state.h"

#define START_FIELD_THRESHHOLD_MS 150

void init_robi_on_entry(t_state *inst __attribute__((unused)));

void init_robi_on_update(t_state*inst __attribute__((unused)));

void drive_logic_super_state_on_entry(t_state*inst __attribute__((unused)));

void drive_logic_super_state_on_update(t_state*inst __attribute__((unused)));

void nothing_on_entry(t_state*inst __attribute__((unused)));

void nothing_on_update(t_state*inst __attribute__((unused)));

void drive_through_start_on_entry(t_state*inst __attribute__((unused)));

void drive_through_start_on_entry_on_update(t_state*inst __attribute__((unused)));

void forward_on_entry(t_state*inst __attribute__((unused)));

void forward_on_update(t_state*inst __attribute__((unused)));

void backwards_on_entry(t_state*inst __attribute__((unused)));

void backwards_on_update(t_state*inst __attribute__((unused)));

void left_on_entry(t_state*inst __attribute__((unused)));

void left_on_update(t_state*inst __attribute__((unused)));

void hard_left_on_entry(t_state*inst __attribute__((unused)));

void hard_left_on_update(t_state*inst __attribute__((unused)));

void right_on_entry(t_state*inst __attribute__((unused)));

void right_on_update(t_state*inst __attribute__((unused)));

void hard_right_on_entry(t_state*inst __attribute__((unused)));

void hard_right_on_update(t_state*inst __attribute__((unused)));

void check_is_start_field_on_entry(t_state*inst __attribute__((unused)));

void check_is_start_field_on_update(t_state*inst __attribute__((unused)));

void stop_on_entry(t_state*inst __attribute__((unused)));

void stop_on_update(t_state*inst __attribute__((unused)));

void error_on_entry(t_state*inst __attribute__((unused)));

void error_on_update(t_state*inst __attribute__((unused)));

/********************
* CONDITIONS
********************/

uint8_t condition_allways(t_state *inst __attribute__((unused)));

uint8_t condition_forward_to_left(t_state *inst __attribute__((unused)));

uint8_t condition_forward_to_right(t_state *inst __attribute__((unused)));

uint8_t condition_LF_LMR(t_state *inst __attribute__((unused)));

uint8_t condition_LF_NOT_LMR(t_state *inst __attribute__((unused)));

uint8_t condition_forward_to_backwards(t_state *inst __attribute__((unused)));

uint8_t condition_backwards_to_forward(t_state *inst __attribute__((unused)));

uint8_t condition_nothing_to_forward(t_state *inst __attribute__((unused)));

uint8_t condition_left_to_forward(t_state *inst __attribute__((unused)));

uint8_t condition_left_to_hard_left(t_state *inst __attribute__((unused)));

uint8_t condition_hard_left_to_left(t_state *inst __attribute__((unused)));

uint8_t condition_right_to_forward(t_state *inst __attribute__((unused)));

uint8_t condition_right_to_hard_right(t_state *inst __attribute__((unused)));

uint8_t condition_hard_right_to_right(t_state *inst __attribute__((unused)));

uint8_t condition_check_for_start_to_forward(t_state *inst __attribute__((unused)));

uint8_t condition_check_for_start_to_stop(t_state *inst __attribute__((unused)));



#endif // ROBOTER_STATES_H
