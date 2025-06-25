#include "state_machine/state.h"
#include <stdint.h>
#include <string.h>
#include "state_machine/state_machine.h"
#include "tools/linked_list.h"
#include "tools/logger.h"

int8_t add_edge(t_state *inst, uint8_t (*condition)(), char *next_state_name) {
	// guards
	if (inst == NULL) { // invalid parameter
		WARNING("add_edge: inst is NULL.\n");
		return 1; 
	} 
	if (condition == NULL) { // invalid parameter
		WARNING("add_edge: condition is NULL.\n");
		return 1; 
	} 
	if (next_state_name == NULL) { // invalid parameter
		WARNING("add_edge: next_state_name is NULL.\n");
		return 1; 
	}
	WARNING("add_edge: parameter valid\n");

	// create edge
	t_edge *edge = malloc(sizeof(t_edge));
	if (edge == NULL) { // malloc failed
		WARNING("add_edge: edge malloc failed!\n");
		return 2; 
	}

	// bind condition
	edge->condition = condition;

	// set state_name
	edge->state_name = malloc(strlen(next_state_name)+1);
	if (edge == NULL) { 
		WARNING("add_edge: edge->state_name malloc failed!\n");
		return 2; 
	} // malloc failed
	strcpy(edge->state_name, next_state_name);

	// add the new edge to the linked list
	int8_t rc = linked_list_prepend_node(&inst->head_edges, edge);
	if (rc != 0) { 
		WARNING("add_edge: linked list prepend failed!\n");
		return 3; 
	} // prepend failed

	return 0;
}

void check_edges(t_state *inst, t_state_machine *state_machine) {

	t_linked_list_node *current = inst->head_edges;
	while (current != NULL) {

    t_edge *edge = (t_edge*)current->vp_data;
		if (edge->condition()) {

			int8_t rc = state_machine->set_current_state(state_machine, edge->state_name);
			if (rc != 0) { // state doesnt exist
				FATAL("check_edges State %s does not exist.\n", edge->state_name);
				state_machine->set_current_state(state_machine, state_machine->cp_error_state_name);
				break;
			}
		}
		current = current->stp_ll_next;
	}
	return;
}

int8_t STATE_constructor(
		t_state *inst,
		char *unique_state_name,
		void (*on_entry)(struct state *inst), 
		void (*on_update)(struct state *inst))
{

	// guards
	if (unique_state_name == NULL) { return 1; }
	if (on_entry == NULL) { return 1; }
	if (on_update == NULL) { return 1; }

	// set name
	inst->unique_name = malloc(strlen(unique_state_name) +1);
	strcpy(inst->unique_name, unique_state_name);

	// bind functions
	inst->add_edge = &add_edge;
	inst->on_entry = on_entry;
	inst->on_update = on_update;
	inst->check_edges = &check_edges;

	INFO("State %s constructor successfull.\n", unique_state_name);

	return 0;
}
