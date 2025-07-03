#define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"
#include "tools/iesusart.h"

#include "state_machine/state_machine.h"
#include "state_machine/state.h"
#include "roboter/roboter_states.h" // roboter state definitions

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int main() {
	USART_init(UBRR_SETTING);
	USART_print("USART_init\n");

	// init state machine
	t_state_machine *state_machine = STATE_MACHINE_constructor();
	if (NULL == state_machine) {
		FATAL("Failed to construct state_machine.\n");
		return 1;
	}

	// init states
	// This is Excel !!!!
	t_state *t_state_init_robi  =	STATE_constructor("init_robi",  init_robi_on_entry, 	init_robi_on_update);
	t_state *t_state_forward    =	STATE_constructor("forward",    forward_on_entry, 		forward_on_update);
	t_state *t_state_backwards  =	STATE_constructor("backwards",  backwards_on_entry, 	backwards_on_update);	
	t_state *t_state_left       =	STATE_constructor("left",       left_on_entry, 				left_on_update);
	t_state *t_state_hard_left  =	STATE_constructor("hard_left",  hard_left_on_entry, 	hard_left_on_update);
	t_state *t_state_right      =	STATE_constructor("right",      right_on_entry, 			right_on_update);
	t_state *t_state_hard_right = STATE_constructor("hard_right", hard_right_on_entry, 	hard_right_on_update);
	t_state *t_state_stop       = STATE_constructor("stop",       stop_on_entry, 				stop_on_update);
	t_state *t_state_error      =	STATE_constructor("error",      stop_on_entry, 				error_on_update);

	if (NULL == t_state_init_robi
			|| NULL == t_state_forward
			|| NULL == t_state_backwards
			|| NULL == t_state_left
			|| NULL == t_state_hard_left
			|| NULL == t_state_right
			|| NULL == t_state_hard_right
			|| NULL == t_state_stop
			|| NULL == t_state_error
			) {
		FATAL("Failed to construct state!\n");
		return 1;
	}

	INFO("[main] all states constructed\n");

	// configure states
	t_state_init_robi->add_edge(t_state_init_robi, condition_allways, t_state_forward->unique_name);

	t_state_forward->add_edge(t_state_forward, condition_forward_to_left, t_state_left->unique_name);
	t_state_forward->add_edge(t_state_forward, condition_forward_to_right, t_state_right->unique_name);
	t_state_forward->add_edge(t_state_forward, condition_forward_to_stop, t_state_stop->unique_name);
	t_state_forward->add_edge(t_state_forward, condition_forward_to_backwards, t_state_backwards->unique_name);

	t_state_backwards->add_edge(t_state_backwards, condition_backwards_to_forward, t_state_forward->unique_name);

	t_state_left->add_edge(t_state_left, condition_left_to_hard_left, t_state_hard_left->unique_name);
	t_state_left->add_edge(t_state_left, condition_left_to_forward, t_state_forward->unique_name);

	t_state_hard_left->add_edge(t_state_hard_left, condition_hard_left_to_left, t_state_left->unique_name);

	t_state_right->add_edge(t_state_right, condition_right_to_hard_right, t_state_hard_right->unique_name);
	t_state_right->add_edge(t_state_right, condition_right_to_forward, t_state_forward->unique_name);

	t_state_hard_right->add_edge(t_state_hard_right, condition_hard_right_to_right, t_state_right->unique_name);

	t_state_stop->add_edge(t_state_stop, condition_allways, t_state_forward->unique_name);

	INFO("[main] all edges added\n");

	// configure state machine
	state_machine->add_error_state(state_machine, t_state_error);
	state_machine->add_state(state_machine, t_state_forward);
	state_machine->add_state(state_machine, t_state_backwards);
	state_machine->add_state(state_machine, t_state_left);
	state_machine->add_state(state_machine, t_state_hard_left);
	state_machine->add_state(state_machine, t_state_right);
	state_machine->add_state(state_machine, t_state_hard_right);
	state_machine->add_state(state_machine, t_state_init_robi);
	state_machine->add_state(state_machine, t_state_stop);

	INFO("[main] all states added\n");

	int8_t rc = state_machine->set_start_state(state_machine, t_state_init_robi->unique_name);
	if (0 != rc) { // set_start_state failed
		state_machine->set_current_state(state_machine, state_machine->tp_error_state->unique_name);
	}

	INFO("[main] now running state machine\n");

	state_machine->run(state_machine);

	return 0;
}
