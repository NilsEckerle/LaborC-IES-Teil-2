#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include "tools/linked_list.h"
#include "state_machine/state.h"

typedef struct state_machine {
	t_linked_list_node *tp_head_states;

	char *cp_current_state_name;
	t_state *tp_current_state;

	char *cp_error_state_name;
	t_state *tp_error_state;

	uint8_t bool_is_new_state;

	int8_t (*add_state)(struct state_machine *inst, t_state *new_state);
	int8_t (*add_error_state)(struct state_machine *inst, t_state *new_state);
	int8_t (*set_start_state)(struct state_machine *inst, char *start_state_name);
	int8_t (*set_current_state)(struct state_machine *inst, char *new_state_name);
	void (*run)(struct state_machine *inst);

} t_state_machine;

int8_t add_state(t_state_machine *inst, t_state *new_state);

int8_t add_error_state(t_state_machine *inst, t_state *new_state);

int8_t set_start_state(t_state_machine *inst, char *start_state_name);

int8_t set_current_state(t_state_machine *inst, char *new_state_name);

void run(t_state_machine *inst);

int8_t STATE_MACHINE_constructor(
		t_state_machine *inst);

#endif // !STATE_MACHINE_H
