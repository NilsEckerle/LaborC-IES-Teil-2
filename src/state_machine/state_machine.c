#include "state_machine/state_machine.h"
#include <stdint.h>
#include <string.h>
#include "state_machine/state.h"
#include "tools/linked_list.h"
#include "tools/logger.h"

struct DTO_linked_list_itterate_find {
	char *state_name_to_find;
	t_state **found_state_return_register;
};

static void *find_state_by_name_callback(struct DTO_linked_list_itterate_find *dto_found, t_state *state_to_check) {
	if (*dto_found->found_state_return_register != NULL) { // already found state
		return NULL;
	}

	if (strcmp(dto_found->state_name_to_find, state_to_check->unique_name)) {
		*dto_found->found_state_return_register = state_to_check;
	}

	return NULL;
}

static t_state *find_state_by_name(t_linked_list_node *head, char *state_name_to_find) {
	struct DTO_linked_list_itterate_find *dto = {0};
	dto->state_name_to_find = state_name_to_find;

	linked_list_itterate_function(head, find_state_by_name_callback, dto);

	return *dto->found_state_return_register;
}


int8_t add_state(t_state_machine *inst, t_state *new_state) {
	// guards
	if (NULL == inst) {
		WARNING("add_state: inst is NULL");
		return 1;
	}
	if (NULL == new_state) {
		WARNING("add_state: new_state is NULL");
		return 1;
	}

	// check duplicate
	t_state *state = find_state_by_name(inst->tp_head_states, new_state->unique_name);
	if (NULL != state) {
		WARNING("add_state: state %s already exist.\n", new_state->unique_name);
		return 2;
	}

	// add the state
	linked_list_prepend_node(&inst->tp_head_states, new_state);
}

int8_t add_error_state(t_state_machine *inst, t_state *new_state) {
	int8_t rc = inst->add_state(inst, new_state);
	if (0 != rc) {
		WARNING("add_error_state: adding state didn't work");
		return 1;
	}

	inst->tp_error_state = new_state;
	inst->cp_error_state_name = new_state->unique_name;
	return 0;
}

int8_t set_start_state(t_state_machine *inst, char *start_state_name) {
	struct DTO_linked_list_itterate_find *dto = {0};
	dto->state_name_to_find = start_state_name;

	t_state *state = find_state_by_name(inst->tp_head_states, start_state_name);
	if (NULL == state) {
		WARNING("set_start_state: state %s not found.\n", start_state_name);
		return 1;
	}

	inst->tp_current_state = state;
	inst->cp_current_state_name = state->unique_name;
	return 0;
}



int8_t set_current_state(t_state_machine *inst, char *new_state_name) {
	if (inst->cp_current_state_name != NULL) {
		free(inst->cp_current_state_name);
	}

	inst->cp_current_state_name = malloc(strlen(new_state_name));
	if (inst->cp_current_state_name == NULL) {
		WARNING("set_current_state: malloc failed!\n");
		return 1;
	}

	t_state *p_new_current_state = NULL;
	linked_list_itterate_function(inst->tp_head_states, find_state_by_name_callback, p_new_current_state);

	return 0;
}

void run(t_state_machine *inst) {
	t_state *tp_state = inst->tp_current_state;
	while (1) {
		tp_state->on_update(tp_state);
		tp_state->check_edges(tp_state, inst);
	}
}

int8_t STATE_MACHINE_constructor(
		t_state_machine *inst) {
	if (inst == NULL) { // invalid parameter
		return 1;
	}

	inst->add_state = &add_state;
	inst->add_error_state = &add_error_state;
	inst->set_start_state = &set_start_state;
	inst->set_current_state = &set_current_state;
	inst->run = &run;

	return 0;
}

