#ifndef ROBOTER_STATES_H
#define ROBOTER_STATES_H

#include "state_machine/state.h"

void init_robi_on_entry(t_state *inst __attribute__((unused)));

void init_robi_on_update(t_state*inst __attribute__((unused)));

void nothing_on_entry(t_state*inst __attribute__((unused)));

void nothing_on_update(t_state*inst __attribute__((unused)));

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

void stop_on_entry(t_state*inst __attribute__((unused)));

void stop_on_update(t_state*inst __attribute__((unused)));

void error_on_entry(t_state*inst __attribute__((unused)));

void error_on_update(t_state*inst __attribute__((unused)));

/********************
* CONDITIONS
********************/

uint8_t condition_allways();

uint8_t condition_forward_to_left();

uint8_t condition_forward_to_right();

uint8_t condition_forward_to_stop();

uint8_t condition_forward_to_backwards();

uint8_t condition_backwards_to_forward();

uint8_t condition_nothing_to_forward();

uint8_t condition_left_to_forward();

uint8_t condition_left_to_hard_left();

uint8_t condition_hard_left_to_left();

uint8_t condition_right_to_forward();

uint8_t condition_right_to_hard_right();

uint8_t condition_hard_right_to_right();


#endif // ROBOTER_STATES_H
