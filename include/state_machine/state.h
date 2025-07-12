#ifndef STATE_H
#define STATE_H

#include "tools/dynamic_array.h"
#include <stdint.h>

typedef struct state_machine t_state_machine;

typedef struct state {
  char *unique_name;
  t_dyn_arr *tdynarr_edges;
  uint32_t *ui32p_state_entry_time_ms;
  struct state *tp_parent;

  int8_t (*add_edge)(struct state *inst,
                     uint8_t (*condition)(struct state *inst),
                     char *next_state_name);
  void (*on_entry)(struct state *inst);
  void (*on_update)(struct state *inst);
  void (*check_edges)(struct state *inst, t_state_machine *state_machine);
  void (*destructor)(struct state *inst);

} t_state;

typedef struct edge {
  uint8_t (*condition)(t_state *inst);
  char *state_name;
} t_edge;

int8_t STATE_add_edge(t_state *inst, uint8_t (*condition)(t_state *inst),
                      char *next_state_name);

int8_t STATE_add_edge_with_execute(t_state *inst,
                                   uint8_t (*condition)(t_edge *inst),
                                   void (*fp_execute_on_transition)(),
                                   char *next_state_name);

void STATE_set_parent(t_state *inst, t_state *tp_new_parent);

void STATE_check_edges(t_state *inst, t_state_machine *state_machine);

void STATE_destructor(t_state *inst);

t_state *STATE_constructor(const char *unique_state_name,
                           void (*on_entry)(struct state *inst),
                           void (*on_update)(struct state *inst));

#endif // !STATE_H
