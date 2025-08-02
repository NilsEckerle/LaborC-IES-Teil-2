// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "state_machine/state_machine.h"
#include "state_machine/state.h"
#include "tools/dynamic_array.h"
#include "tools/logger.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

int8_t STATE_MACHINE_add_state(state_machine_t *inst, t_state *new_state) {
  // guards
  if (NULL == inst || NULL == new_state) {
    WARNING("[add_state] invalid parameter inst or new_state\n");
    return 1;
  }

  // check duplicate
  for (int i = 0; i < inst->arrp_states->ui8_size; i++) {
    if (inst->arrp_states->vpp_data_array[i] == new_state) {  // is duplicate
      WARNING("[add_state] state adress %p is already added", new_state);
    }
  }

  // add state
  if (DYN_ARR_add(inst->arrp_states, new_state)) {
    WARNING("[add_state] State adress: '%p' failed to add to dyn_arr!\n", new_state);
    return 3;
  }

  INFO("[add_state] Added state adress: '%p'\n", new_state);

  return 0;
}

int8_t STATE_MACHINE_add_error_state(state_machine_t *inst, t_state *new_state) {
  if (NULL == inst || NULL == new_state) {
    WARNING("[add_error_state] invalid parameter inst of new_state\n");
    return 1;
  }

  int8_t rc = STATE_MACHINE_add_state(inst, new_state);
  if (0 != rc) {
    WARNING("[add_error_state] adding state adress: '%p' didn't work\n", new_state);
    return 2;
  }

  inst->tp_error_state = new_state;
  return 0;
}

int8_t STATE_MACHINE_set_start_state(state_machine_t *inst, t_state *start_state) {
  return STATE_MACHINE_set_current_state(inst, start_state);
}

int8_t STATE_MACHINE_set_current_state(state_machine_t *inst, t_state *tp_new_state) {
  if (inst == NULL || tp_new_state == NULL) {
    return 1;
  }
  inst->bool_is_new_state = 1;
  inst->tp_current_state = tp_new_state;

  TRACE("[set_current_state] to adress: %p\n", tp_new_state);

  return 0;
}

void STATE_MACHINE_update(state_machine_t *tp_state_machine) {
  t_state *tp_state = tp_state_machine->tp_current_state;

  INFO_SPAM("[run] running state adress: %p\n", tp_state);

  if (tp_state_machine->bool_is_new_state) {
    tp_state_machine->bool_is_new_state = 0;
    tp_state_machine->tp_current_state->on_entry(tp_state_machine->tp_current_state);
  }
  if (tp_state->tp_parent != NULL) {
    tp_state->tp_parent->on_update(tp_state->tp_parent);
  }
  tp_state->on_update(tp_state);
  STATE_check_edges(tp_state, tp_state_machine);
}

void STATE_MACHINE_run(state_machine_t *inst) {
  while (1) {
    STATE_MACHINE_update(inst);
  }
}

void STATE_MACHINE_destructor(state_machine_t *inst) {
  if (NULL != inst) {
    if (NULL != inst->arrp_states) {
      DYN_ARR_destructor(&inst->arrp_states);  // field
    }

    free(inst);  // struct
  }
}

state_machine_t *STATE_MACHINE_constructor() {
  state_machine_t *inst = malloc(sizeof(state_machine_t));
  if (NULL == inst) {
    WARNING("[STATE_MACHINE_constructor] malloc failed for instance!\n");
    return NULL;
  }

  inst->arrp_states = DYN_ARRAY_constructor();
  if (NULL == inst->arrp_states) {
    WARNING("[STATE_MACHINE_constructor] constructor failed for dyn_arr!\n");
    return NULL;
  }

  return inst;
}
