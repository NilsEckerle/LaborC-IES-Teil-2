// #define LOG_LEVEL LOG_LEVEL_INFO
#include "roboter/states_general.h"
#include "roboter/states_config.h"
#include "roboter/states_drive.h"
#include "roboter/conditions_general.h"
#include "roboter/conditions_LF.h"
#include "roboter/conditions_clock.h"

#include "tools/iesusart.h"
#include "tools/logger.h"

#include "state_machine/state.h"
#include "state_machine/state_machine.h"

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

t_state_machine *configure_state_machine() {

  // init state machine
  t_state_machine *state_machine = STATE_MACHINE_constructor();
  if (NULL == state_machine) {
    FATAL("Failed to construct state_machine.\n");
    return NULL;
  }

  // init states
  t_state *t_state_init_robi =
      STATE_constructor("init_robi", init_robi_on_entry, init_robi_on_update);
  t_state *t_state_check_for_start =
      STATE_constructor("check_for_start", check_is_start_field_on_entry, check_is_start_field_on_update);
  t_state *t_state_drive_throught =
      STATE_constructor("drive_through", drive_through_start_on_entry, drive_through_start_on_entry_on_update);
  t_state *t_state_forward =
      STATE_constructor("forward", forward_on_entry, forward_on_update);
  t_state *t_state_backwards =
      STATE_constructor("backwards", backwards_on_entry, backwards_on_update);
  t_state *t_state_left =
      STATE_constructor("left", left_on_entry, left_on_update);
  t_state *t_state_hard_left =
      STATE_constructor("hard_left", hard_left_on_entry, hard_left_on_update);
  t_state *t_state_right =
      STATE_constructor("right", right_on_entry, right_on_update);
  t_state *t_state_hard_right = STATE_constructor(
      "hard_right", hard_right_on_entry, hard_right_on_update);
  t_state *t_state_stop =
      STATE_constructor("stop", stop_on_entry, stop_on_update);
  t_state *t_state_error =
      STATE_constructor("error", error_on_entry, error_on_update);
  t_state *t_state_drive_logic_super_state =
      STATE_constructor("drive_logic_super_state", drive_logic_super_state_on_entry, drive_logic_super_state_on_update);

  if (NULL == t_state_init_robi || NULL == t_state_forward ||
      NULL == t_state_backwards || NULL == t_state_left ||
      NULL == t_state_hard_left || NULL == t_state_right ||
      NULL == t_state_hard_right || NULL == t_state_stop ||
      NULL == t_state_error) {
    FATAL("Failed to construct state!\n");
    return NULL;
  }

  INFO("[main] all states constructed\n");

  // configure states
	// drive_logic_super_state
	add_edge(t_state_drive_logic_super_state, condition_LF_LMR, t_state_check_for_start->unique_name);

	// init
  add_edge(t_state_init_robi, condition_allways, t_state_drive_throught->unique_name);

	// drive_through_start
	add_edge(t_state_drive_throught, condition_LF_NOT_LMR, t_state_forward->unique_name);

	// check for start field
	add_edge(t_state_check_for_start, condition_start_field_delay, t_state_stop->unique_name);
	add_edge(t_state_check_for_start, condition_check_for_start_to_forward, t_state_forward->unique_name);

	// forward
  add_edge(t_state_forward, condition_forward_to_left,
           t_state_left->unique_name);
  add_edge(t_state_forward, condition_forward_to_right,
           t_state_right->unique_name);
  add_edge(t_state_forward, condition_forward_to_backwards,
           t_state_backwards->unique_name);

	//backward
  add_edge(t_state_backwards, condition_backwards_to_forward,
           t_state_forward->unique_name);

	// left
  add_edge(t_state_left, condition_left_to_hard_left,
           t_state_hard_left->unique_name);
  add_edge(t_state_left, condition_left_to_forward,
           t_state_forward->unique_name);

  add_edge(t_state_hard_left, condition_hard_left_to_left,
           t_state_left->unique_name);

	// right
  add_edge(t_state_right, condition_right_to_hard_right,
           t_state_hard_right->unique_name);
  add_edge(t_state_right, condition_right_to_forward,
           t_state_forward->unique_name);

  add_edge(t_state_hard_right, condition_hard_right_to_right,
           t_state_right->unique_name);

	// stop
  // add_edge(t_state_stop, condition_allways, t_state_config->unique_name);

  INFO("[main] all edges added\n");

	// configure state herarchy
	STATE_set_parent(t_state_forward, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_backwards, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_left, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_hard_left, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_right, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_hard_right, t_state_drive_logic_super_state);

  INFO("[main] all state herarchy set\n");

  // configure state machine
  add_error_state(state_machine, t_state_error);
  add_state(state_machine, t_state_drive_throught);
  add_state(state_machine, t_state_forward);
  add_state(state_machine, t_state_backwards);
  add_state(state_machine, t_state_left);
  add_state(state_machine, t_state_hard_left);
  add_state(state_machine, t_state_right);
  add_state(state_machine, t_state_hard_right);
  add_state(state_machine, t_state_init_robi);
  add_state(state_machine, t_state_check_for_start);
  add_state(state_machine, t_state_stop);

  INFO("[main] all states added\n");

  int8_t rc = set_start_state(state_machine, t_state_init_robi->unique_name);
  if (0 != rc) { // set_start_state failed
    set_current_state(state_machine,
                      state_machine->tp_error_state->unique_name);
  }

	return state_machine;
}

int main() {
  USART_init(UBRR_SETTING);
  INFO("USART_init\n");

	t_state_machine *state_machine = configure_state_machine();
	if (NULL == state_machine) {
		return 1;
	}

  INFO("[main] now starting state machine\n");
  run(state_machine);

  return 0;
}
