#include "state_machine/state.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "state_machine/state_machine.h"
// #define LOG_LEVEL LOG_LEVEL_TRACE
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
	TRACE("add_edge: parameter valid\n");

	t_edge *edge = malloc(sizeof(t_edge));
	if (NULL == edge) {
		WARNING("[add_edge] malloc failed for edge to: %s!\n", next_state_name);
		return 2;
	}
	edge->condition  = condition;
	edge->state_name = malloc(strlen(next_state_name) + 1);
	strcpy(edge->state_name, next_state_name);

	// add the new edge
	if (inst->tdynarr_edges->fp_add(inst->tdynarr_edges, edge)) {
		WARNING("[add_edge] Edge to '%s' failed to add to dyn_arr!\n", edge->state_name);
		return 3;
	}

	INFO("[add_edge] added edge '%s -> %s'\n", inst->unique_name, edge->state_name);

	return 0;
}

void check_edges(t_state *inst, t_state_machine *state_machine) {
	if (NULL == inst) { 
		WARNING("[check_edges] inst is NULL!\n");
		return; 
	}
	if (NULL == inst->tdynarr_edges) { 
		WARNING("[check_edges] inst edge array is NULL!\n");
		return; 
	}

	if (0 == inst->tdynarr_edges->ui8_size) { 
		TRACE("[check_edges] State has no edges to check.\n");
		return; 
	}

	for (int i = 0; i < inst->tdynarr_edges->ui8_size; i++) {
    t_edge *edge = DYN_ARR_get_as_ptr(inst->tdynarr_edges, i, t_edge *);
		TRACE("checking edge 'adress %p : to %s - adress %p'.\n", edge, edge->state_name, edge->state_name);
		TRACE("checking condition '%s->%s'.\n", inst->unique_name, edge->state_name);

		if (edge->condition()) {
			TRACE("AAAAARRRRRGGGGHHHHH!\n");
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

void STATE_destructor(t_state *inst) {
	if (NULL != inst) {
		if (NULL != inst->unique_name) {
			free(inst->unique_name); // field
		}

		if (NULL != inst->tdynarr_edges) {
			inst->tdynarr_edges->fp_destructor(inst->tdynarr_edges); // field
		}

		free(inst); // struct
	}
}

t_state *STATE_constructor(
		const char *unique_state_name,
		void (*on_entry)(struct state *inst), 
		void (*on_update)(struct state *inst)
		) {
	t_state *inst = malloc(sizeof(t_state));
	if (NULL == inst) {
		WARNING("[STATE_constructor] malloc failed for inst!\n");
		return NULL;
	}

	// guards
	if (unique_state_name == NULL) { return NULL; }
	if (on_entry == NULL) { return NULL; }
	if (on_update == NULL) { return NULL; }

	// set name
	inst->unique_name = malloc(strlen(unique_state_name) + 1);
	if (inst->unique_name == NULL) {
		WARNING("[STATE_constructor] malloc failed for unique_name: %s!\n", unique_state_name);
		return NULL;
	}
	strcpy(inst->unique_name, unique_state_name);

	inst->tdynarr_edges = DYN_ARRAY_constructor();

	// bind functions
	inst->add_edge = add_edge;
	inst->on_entry = on_entry;
	inst->on_update = on_update;
	inst->check_edges = check_edges;
	inst->destructor = STATE_destructor;

	INFO("State %s constructor successfull.\n", unique_state_name);

	return inst;
}
