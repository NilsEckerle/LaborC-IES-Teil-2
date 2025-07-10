// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "state_machine/state_machine.h"
#include "state_machine/state.h"
#include "tools/dynamic_array.h"

static t_state *find_state_by_name(t_state_machine *tp_state_machine, const char *cp_state_name_to_find) {
	for (int i = 0; i < tp_state_machine->arrp_states->ui8_size; i++) {
		t_state *state = DYN_ARR_get_as_ptr(tp_state_machine->arrp_states, i, t_state *);
		TRACE("[find_state_by_name] comparing '%s' with '%s'\n", state->unique_name, cp_state_name_to_find);
		TRACE("%p\n", state);
		if (0 == strcmp(state->unique_name, cp_state_name_to_find)) { // match found
			return state;
		}
	}
	return NULL; // no match found
}


int8_t add_state(t_state_machine *inst, t_state *new_state) {
	// guards
	if (NULL == inst || NULL == new_state) {
		WARNING("[add_state] invalid parameter inst or new_state\n");
		return 1;
	}

	// check duplicate
	t_state *state = find_state_by_name(inst, new_state->unique_name);
	if (NULL != state) {
		WARNING("[add_state] State '%s' already exist.\n", new_state->unique_name);
		return 2;
	}

	// add state
	if (DYN_ARR_add(inst->arrp_states, new_state)) {
		WARNING("[add_state] State '%s' failed to add to dyn_arr!\n", new_state->unique_name);
		return 3;
	}

	INFO("[add_state] Added state '%s'\n", new_state->unique_name);

	return 0;
}

int8_t add_error_state(t_state_machine *inst, t_state *new_state) {
	if (NULL == inst || NULL == new_state) {
		WARNING("[add_error_state] invalid parameter inst of new_state\n");
		return 1;
	}

	int8_t rc = inst->add_state(inst, new_state);
	if (0 != rc) {
		WARNING("[add_error_state] adding state '%s' didn't work\n", new_state->unique_name);
		return 2;
	}

	inst->tp_error_state = new_state;
	return 0;
}

int8_t set_start_state(t_state_machine *inst, char *start_state_name) {
	return set_current_state(inst, start_state_name);
}

int8_t set_current_state(t_state_machine *inst, char *new_state_name) {
	inst->bool_is_new_state = 1;
	inst->tp_current_state = find_state_by_name(inst, new_state_name);
	if (NULL == inst->tp_current_state) {
		inst->tp_current_state = inst->tp_error_state;
		FATAL("[set_current_state] State '%s' not found.\n", new_state_name);
		return 1;
	}

	return 0;
}

void update(t_state_machine *inst) {
		t_state *tp_state = inst->tp_current_state;

		INFO_SPAM("[run] running state %p, %s\n", tp_state, tp_state->unique_name);

		if (inst->bool_is_new_state) {
			inst->bool_is_new_state = 0;
			inst->tp_current_state->on_entry(inst->tp_current_state);
		}
		tp_state->on_update(tp_state);
		tp_state->check_edges(tp_state, inst);
		_delay_us(1);
}

void run(t_state_machine *inst) {
	while (1) {
		inst->update(inst);
	}
}

void STATE_MACHINE_destructor(t_state_machine *inst) {
	if (NULL != inst) {
		if (NULL != inst->arrp_states) {
			DYN_ARR_destructor(&inst->arrp_states); //field
		}

		free(inst); // struct
	}
}

t_state_machine *STATE_MACHINE_constructor() {
	t_state_machine *inst = malloc(sizeof(t_state_machine));
	if (NULL == inst) {
		WARNING("[STATE_MACHINE_constructor] malloc failed for instance!\n");
		return NULL;
	}

	inst->arrp_states = DYN_ARRAY_constructor();
	if (NULL == inst->arrp_states) {
		WARNING("[STATE_MACHINE_constructor] constructor failed for dyn_arr!\n");
		return NULL;
	}

	inst->add_state = add_state;
	inst->add_error_state = add_error_state;
	inst->set_start_state = set_start_state;
	inst->set_current_state = set_current_state;
	inst->update = update;
	inst->run = run;
	inst->destructor = STATE_MACHINE_destructor;

	return inst;
}

