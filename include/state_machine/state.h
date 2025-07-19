#ifndef STATE_H
#define STATE_H

#include "tools/dynamic_array.h"
#include <stdint.h>

typedef struct state_machine t_state_machine;

typedef struct state {
  t_dyn_arr *tdynarr_edges;
  uint32_t *ui32p_state_entry_time_ms;
  struct state *tp_parent;

  void (*on_entry)(struct state *tp_state);
  void (*on_update)(struct state *tp_state);

} t_state;

typedef struct edge {
  void *vp_dto;
  uint8_t (*condition)(t_state *tp_state, void *vp_dto);
  void (*fp_execute_on_transition)(t_state *tp_current_state, void *vp_dto);
  t_state *state;

} t_edge;

int8_t STATE_add_edge_with_execute(
    t_state *tp_state,
    uint8_t (*condition)(t_state *tp_current_state, void *vp_dto),
    void (*fp_execute_on_transition)(t_state *tp_current_state, void *vp_dto),
    t_state *next_state_name);

int8_t STATE_add_edge(t_state *tp_state,
                      uint8_t (*condition)(t_state *tp_state, void *vp_dto),
                      t_state *next_state);

void STATE_set_parent(t_state *tp_state, t_state *tp_new_parent);

void STATE_check_edges(t_state *tp_state, t_state_machine *state_machine);

void STATE_destructor(t_state *tp_state);

t_state *STATE_constructor(void (*on_entry)(struct state *tp_state),
                           void (*on_update)(struct state *tp_state));

#endif // !STATE_H
