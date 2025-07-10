#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include "state_machine/state.h"
#include "tools/dynamic_array.h"

typedef struct state_machine {
	t_dyn_arr *arrp_states;

	t_state *tp_current_state;
	t_state *tp_error_state;

	uint8_t bool_is_new_state;
} t_state_machine;

int8_t add_state(t_state_machine *inst, t_state *new_state);

int8_t add_error_state(t_state_machine *inst, t_state *new_state);

int8_t set_start_state(t_state_machine *inst, char *start_state_name);

int8_t set_current_state(t_state_machine *inst, char *new_state_name);

void update(t_state_machine *inst);

void run(t_state_machine *inst);

void STATE_MACHINE_destructor(t_state_machine *inst);

t_state_machine *STATE_MACHINE_constructor();

#endif // !STATE_MACHINE_H
