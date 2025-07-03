#ifndef STATE_H
#define STATE_H

#include <stdint.h>
#include "tools/dynamic_array.h"

typedef struct state_machine t_state_machine;

typedef struct edge {
	uint8_t (*condition)();
	char *state_name;
} t_edge;

typedef struct state {
	char *unique_name;
	t_dyn_arr *tdynarr_edges;

	int8_t (*add_edge)(struct state *inst, uint8_t (*condition)(), char *next_state_name);
	void (*on_entry)(struct state *inst);
	void (*on_update)(struct state *inst);
	void (*check_edges)(struct state *inst, t_state_machine *state_machine);
	void (*destructor)(struct state *inst);

} t_state;

int8_t add_edge(t_state *inst, uint8_t (*condition)(), char *next_state_name);

void check_edges(t_state *inst, t_state_machine *state_machine);

void STATE_destructor(t_state *inst);

t_state *STATE_constructor(
		const char *unique_state_name,
		void (*on_entry)(struct state *inst), 
		void (*on_update)(struct state *inst));

#endif // !STATE_H
