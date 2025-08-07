#include "state_machine/state.h"
#include "roboter/roboter.h"
#include "state_machine/state_machine.h"
#include <stdint.h>
#include <stdlib.h>
// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/dynamic_array.h"
#include "tools/iesusart.h"
#include "tools/logger.h"

// default "trash" function for STATE_add_edge with no execution
static void do_nothing(t_state *tp_state __attribute__((unused)),
                       void *dto __attribute__((unused))) {}

int8_t STATE_add_edge_with_execute(t_state *tp_state,
                                   uint8_t (*condition)(t_state *tp_current_state, void *vp_dto),
                                   void (*execute)(t_state *tp_current_state,
                                                                    void *vp_dto),
                                   t_state *next_state) {
  // guards
  if (tp_state == NULL) {  // invalid parameter
    WARNING("add_edge: tp_state is NULL.\n");
    return 1;
  }
  if (condition == NULL) {  // invalid parameter
    WARNING("add_edge: condition is NULL.\n");
    return 1;
  }
  if (next_state == NULL) {  // invalid parameter
    WARNING("add_edge: next_state is NULL.\n");
    return 1;
  }
  TRACE("add_edge: parameter valid\n");

  // create new edge
  t_edge *edge = malloc(sizeof(t_edge));
  if (NULL == edge) {
    WARNING("[add_edge] malloc failed for edge!\n");
    return 2;
  }
  edge->condition = condition;
  edge->execute = execute;
  edge->vp_dto = NULL;
  edge->state = next_state;

  // add the new edge
  if (DYN_ARR_add(tp_state->tdynarr_edges, edge)) {
    WARNING("[add_edge] Failed to add edge to dyn_arr!\n", edge->state);
    return 3;
  }

  INFO("[add_edge] added edge\n");

  return 0;
}

int8_t STATE_add_edge(t_state *tp_state, uint8_t (*condition)(t_state *tp_state, void *vp_dto),
                      t_state *next_state) {
  return STATE_add_edge_with_execute(tp_state, condition, do_nothing, next_state);
}

void STATE_set_parent(t_state *tp_state, t_state *tp_new_parent) {
  tp_state->tp_parent = tp_new_parent;
  return;
}

void STATE_check_edges(t_state *tp_state, t_state_machine *state_machine) {
  // guards
  if (NULL == tp_state) {
    WARNING("[check_edges] tp_state is NULL!\n");
    return;
  }
  if (NULL == tp_state->tdynarr_edges) {
    WARNING("[check_edges] tp_state edge array is NULL!\n");
    return;
  }
  if (0 == tp_state->tdynarr_edges->ui8_size) {
    TRACE("[check_edges] State has no edges to check.\n");
    return;
  }
  if (NULL == state_machine) {
    WARNING("[check_edges] state_machine is NULL!\n");
    return;
  }

  // iterate over all edges
  for (int i = 0; i < tp_state->tdynarr_edges->ui8_size; i++) {
    t_edge *edge = DYN_ARR_get_as_ptr(tp_state->tdynarr_edges, i, t_edge *);
    TRACE("checking edge 'adress %p : to adress %p'.\n", edge, edge->state);

    // check parrent conditions
    if (tp_state->tp_parent != 0) {
      STATE_check_edges(tp_state->tp_parent, state_machine); // !! rekursion!
    }

    // check self conditions
    if (edge->condition(tp_state, edge->vp_dto)) {
      edge->execute(tp_state, ROBOTER_get_instance()->vp_dto);
      TRACE("conditon is true!\n");

      int8_t rc = STATE_MACHINE_set_current_state(state_machine, edge->state);
      if (rc != 0) {  // state doesnt exist
        FATAL("check_edges State %s does not exist.\n", edge->state);
        STATE_MACHINE_set_current_state(state_machine, state_machine->tp_error_state);
      }
      return;
    }
  }
  // clear USART buffer when false input was send. 
  // i do it here so i dont have to add this as a condition to every state i use USART conditions.
  USART_consume_on_second_call_string();
  return;
}

void STATE_destructor(t_state *tp_state) {
  if (NULL != tp_state) {
    if (NULL != tp_state->tdynarr_edges) {
      DYN_ARR_destructor(&tp_state->tdynarr_edges);  // free field
    }

    if (NULL != tp_state->ui32p_state_entry_time_ms) {
      free(tp_state->ui32p_state_entry_time_ms); // free field
      tp_state->ui32p_state_entry_time_ms = NULL;
    }

    free(tp_state);  // free struct
  }

  return;
}

t_state *STATE_constructor(void (*on_entry)(struct state *tp_state),
                           void (*on_update)(struct state *tp_state)) {
  // guards
  if (on_entry == NULL) {
    return NULL;
  }
  if (on_update == NULL) {
    return NULL;
  }

  // create new state
  t_state *tp_state = malloc(sizeof(t_state));
  if (NULL == tp_state) {
    WARNING("[STATE_constructor] malloc failed for tp_state!\n");
    return NULL;
  }

  tp_state->ui32p_state_entry_time_ms = malloc(sizeof(uint32_t));
  if (tp_state->ui32p_state_entry_time_ms == NULL) {
    WARNING("[STATE_constructor] malloc failed for state_entry_time!\n");
    return NULL;
  }

  tp_state->tdynarr_edges = DYN_ARRAY_constructor();
  tp_state->tp_parent = NULL;

  // bind functions
  tp_state->on_entry = on_entry;
  tp_state->on_update = on_update;

  INFO("State adress: %p constructor successfull.\n", tp_state);

  return tp_state;
}
