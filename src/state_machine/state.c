#include "state_machine/state.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "state_machine/state_machine.h"
// #define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/dynamic_array.h"
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
	INFO("add_edge: parameter valid\n");

	// create edge
	// t_edge *edge = malloc(sizeof(t_edge));
	// if (edge == NULL) { // malloc failed
	// 	FATAL("[add_edge] edge malloc failed!\n");
	// 	return 2; 
	// }
	//
	// // bind condition
	// edge->condition = condition;
	//
	// // set state_name
	// edge->state_name = malloc(strlen(next_state_name)+1);
	// if (edge->state_name == NULL) { 
	// 	WARNING("add_edge: edge->state_name malloc failed!\n");
	// 	return 2; 
	// } // malloc failed
	// strcpy(edge->state_name, next_state_name);

	t_edge edge = {};
	edge.condition  = condition;
	edge.state_name = next_state_name;

	// add the new edge
	DYN_ARR_add(inst->tdynarr_edges, edge);

	INFO("[add_edge] added edge '%s -> %s'\n", inst->unique_name, edge->state_name);

	return 0;
}

void check_edges(t_state *inst, t_state_machine *state_machine) {
	if (NULL == inst) { return; }
	if (NULL == inst->tdynarr_edges) { return; }
	if (0 == inst->tdynarr_edges->ui8_size) { return; }

	for (int i = 0; i < inst->tdynarr_edges->ui8_size; i++) {
    t_edge *edge = DYN_ARR_get_as_type(inst->tdynarr_edges, i, t_edge*);
		if (edge->condition()) {
			int8_t rc = state_machine->set_current_state(state_machine, edge->state_name);
			if (rc != 0) { // state doesnt exist
				FATAL("check_edges State %s does not exist.\n", edge->state_name);
				state_machine->set_current_state(state_machine, state_machine->tp_error_state->unique_name);
				break;
			}
		}
	}
	return;
}

int8_t STATE_constructor(
		t_state *inst,
		const char *unique_state_name,
		void (*on_entry)(struct state *inst), 
		void (*on_update)(struct state *inst))
{

	// guards
	if (unique_state_name == NULL) { return 1; }
	if (on_entry == NULL) { return 1; }
	if (on_update == NULL) { return 1; }

	// set name
	inst->unique_name = malloc(strlen(unique_state_name) + 1);
	if (inst->unique_name == NULL) {
		WARNING("[STATE_constructor] malloc failed for unique_name: %s!\n", unique_state_name);
	}
	strcpy(inst->unique_name, unique_state_name);

	*inst->tdynarr_edges = DYN_ARRAY_constructor();

	// bind functions
	inst->add_edge = &add_edge;
	inst->on_entry = on_entry;
	inst->on_update = on_update;
	inst->check_edges = &check_edges;

	INFO("State %s constructor successfull.\n", unique_state_name);

	return 0;
}
