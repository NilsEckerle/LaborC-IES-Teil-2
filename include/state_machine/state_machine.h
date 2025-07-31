/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-31
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state_machine/state.h"
#include "tools/dynamic_array.h"
#include <stdint.h>

typedef struct state_machine {
  t_dyn_arr *arrp_states;

  t_state *tp_current_state;
  t_state *tp_error_state;

  uint8_t bool_is_new_state;
} t_state_machine;

int8_t STATE_MACHINE_add_state(t_state_machine *tp_state_machine, t_state *new_state);

int8_t STATE_MACHINE_add_error_state(t_state_machine *tp_state_machine, t_state *new_state);

int8_t STATE_MACHINE_set_start_state(t_state_machine *tp_state_machine, t_state *start_state);

int8_t STATE_MACHINE_set_current_state(t_state_machine *tp_state_machine, t_state *new_state);

void STATE_MACHINE_update(t_state_machine *tp_state_machine);

void STATE_MACHINE_run(t_state_machine *tp_state_machine);

void STATE_MACHINE_destructor(t_state_machine *tp_state_machine);

t_state_machine *STATE_MACHINE_constructor();

#endif  // !STATE_MACHINE_H
