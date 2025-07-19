#include "state_machine/state.h"
#include "state_machine/state_machine.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>
// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/dynamic_array.h"
#include "tools/logger.h"

static void do_nothing(t_state *inst, void *dto) {}

int8_t STATE_add_edge_with_execute(
    t_state *inst,
    uint8_t (*condition)(t_state *tp_current_state, void *vp_dto),
    void (*fp_execute_on_transition)(t_state *tp_current_state, void *vp_dto),
    char *next_state_name) {
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
  edge->condition = condition;
  edge->fp_execute_on_transition = fp_execute_on_transition;
  edge->vp_dto = NULL;
  edge->state_name = malloc(strlen(next_state_name) + 1);
  strcpy(edge->state_name, next_state_name);

  // add the new edge
  if (DYN_ARR_add(inst->tdynarr_edges, edge)) {
    WARNING("[add_edge] Edge to '%s' failed to add to dyn_arr!\n",
            edge->state_name);
    return 3;
  }

  INFO("[add_edge] added edge '%s -> %s'\n", inst->unique_name,
       edge->state_name);
  return 0;
}

int8_t STATE_add_edge(t_state *inst,
                      uint8_t (*condition)(t_state *inst, void *vp_dto),
                      char *next_state_name) {
  return STATE_add_edge_with_execute(inst, condition, do_nothing,
                                     next_state_name);
}

void STATE_set_parent(t_state *inst, t_state *tp_new_parent) {
  inst->tp_parent = tp_new_parent;
  return;
}

void STATE_check_edges(t_state *inst, t_state_machine *state_machine) {
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
  if (NULL == state_machine) {
    WARNING("[check_edges] state_machine is NULL!\n");
    return;
  }

  // iterate over all edges
  for (int i = 0; i < inst->tdynarr_edges->ui8_size; i++) {
    t_edge *edge = DYN_ARR_get_as_ptr(inst->tdynarr_edges, i, t_edge *);
    TRACE("checking edge 'adress %p : to %s - adress %p'.\n", edge,
          edge->state_name, edge->state_name);
    TRACE("checking condition '%s->%s'.\n", inst->unique_name,
          edge->state_name);

    // parrent conditions
    if (inst->tp_parent != 0) {
      STATE_check_edges(inst->tp_parent, state_machine);
    }

    // self conditions
    if (edge->condition(inst, edge->vp_dto)) {
      TRACE("AAAAARRRRRGGGGHHHHH!\n");
      int8_t rc = set_current_state(state_machine, edge->state_name);
      if (rc != 0) { // state doesnt exist
        FATAL("check_edges State %s does not exist.\n", edge->state_name);
        set_current_state(state_machine,
                          state_machine->tp_error_state->unique_name);
        edge->fp_execute_on_transition(inst, edge->vp_dto);
      }
      return;
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
      DYN_ARR_destructor(&inst->tdynarr_edges); // field
    }

    free(inst); // struct
  }
}

t_state *_STATE_constructor_P(const char *progmem_unique_state_name,
                           void (*on_entry)(struct state *inst),
                           void (*on_update)(struct state *inst)) {
  t_state *inst = malloc(sizeof(t_state));
  if (NULL == inst) {
    WARNING("[STATE_constructor] malloc failed for inst!\n");
    return NULL;
  }

  // guards
  if (progmem_unique_state_name == NULL) {
    return NULL;
  }
  if (on_entry == NULL) {
    return NULL;
  }
  if (on_update == NULL) {
    return NULL;
  }

  // set name
  inst->unique_name = malloc(strlen_P(progmem_unique_state_name) + 1);
  if (inst->unique_name == NULL) {
    WARNING("[STATE_constructor] malloc failed for unique_name: %s!\n",
            progmem_unique_state_name);
    return NULL;
  }
  strcpy_P(inst->unique_name, progmem_unique_state_name);

  inst->ui32p_state_entry_time_ms = malloc(sizeof(uint32_t));
  if (inst->ui32p_state_entry_time_ms == NULL) {
    WARNING("[STATE_constructor] malloc failed for state_entry_time!\n");
    return NULL;
  }

  inst->tdynarr_edges = DYN_ARRAY_constructor();
  inst->tp_parent = NULL;

  // bind functions
  inst->add_edge = STATE_add_edge;
  inst->on_entry = on_entry;
  inst->on_update = on_update;
  inst->check_edges = STATE_check_edges;
  inst->destructor = STATE_destructor;

  INFO("State %s constructor successfull.\n", unique_state_name);

  return inst;
}
