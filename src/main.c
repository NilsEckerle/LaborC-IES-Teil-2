// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "state_machine/state.h"
#include "state_machine/state_machine.h"

#include "roboter/states_general.h"
#include "roboter/states_config.h"
#include "roboter/states_drive.h"

#include "roboter/conditions_general.h"
#include "roboter/conditions_LF.h"
#include "roboter/conditions_clock.h"
#include "roboter/conditions_roboter.h"
#include "roboter/conditions_USART.h"

#include "roboter/execute_config.h"

#include "tools/iesusart.h"
#include "tools/logger.h"

#include <avr/io.h>
#include <stdint.h>

t_state_machine *configure_state_machine() {

	// init state machine
	t_state_machine *state_machine = STATE_MACHINE_constructor();
	if (NULL == state_machine) {
		FATAL("Failed to construct state_machine.\n");
		return NULL;
	}

	// init states
	// config
	t_state *t_state_config = STATE_constructor(
			"config", 
			config_on_entry, 
			config_on_update);
	t_state *t_state_config_rounds = STATE_constructor(
			"config_rounds", 
			config_rounds_on_entry, 
			config_rounds_on_update);
	t_state *t_state_config_lf_static = STATE_constructor(
			"config_lf_static", 
			config_lf_static_on_entry,
			config_lf_static_on_update);
	t_state *t_state_wait_start = STATE_constructor(
			"wait_start", 
			wait_start_on_entry, 
			wait_start_on_update);

	// driving
	t_state *t_state_init_robi = STATE_constructor(
			"init_robi", 
			init_robi_on_entry, 
			init_robi_on_update);
	t_state *t_state_check_for_start = STATE_constructor(
			"check_for_start", 
			check_is_start_field_on_entry, 
			check_is_start_field_on_update);
	t_state *t_state_drive_throught = STATE_constructor(
			"drive_through", 
			drive_through_start_on_entry, 
			drive_through_start_on_entry_on_update);
	t_state *t_state_forward = STATE_constructor(
			"forward", 
			forward_on_entry, 
			forward_on_update);
	t_state *t_state_backwards = STATE_constructor(
			"backwards", 
			backwards_on_entry, 
			backwards_on_update);
	t_state *t_state_left = STATE_constructor(
			"left", 
			left_on_entry, 
			left_on_update);
	t_state *t_state_hard_left = STATE_constructor(
			"hard_left", 
			hard_left_on_entry, 
			hard_left_on_update);
	t_state *t_state_right = STATE_constructor(
			"right", 
			right_on_entry, 
			right_on_update);
	t_state *t_state_hard_right = STATE_constructor( 
			"hard_right", 
			hard_right_on_entry, 
			hard_right_on_update);
	t_state *t_state_stop = STATE_constructor(
			"stop", 
			stop_on_entry, 
			stop_on_update);
	t_state *t_state_error = STATE_constructor(
			"error", 
			error_on_entry, 
			error_on_update);
	t_state *t_state_drive_logic_super_state = STATE_constructor(
			"drive_logic_super_state", 
			drive_logic_super_state_on_entry, 
			drive_logic_super_state_on_update);

	if (NULL == t_state_init_robi || NULL == t_state_forward ||
			NULL == t_state_backwards || NULL == t_state_left ||
			NULL == t_state_hard_left || NULL == t_state_right ||
			NULL == t_state_hard_right || NULL == t_state_stop ||
			NULL == t_state_error) {
		FATAL("Failed to construct state!\n");
		return NULL;
	}

	INFO("[configure_state_machine] all states constructed\n");

	// configure states
	// init
	STATE_add_edge( t_state_init_robi, condition_allways, t_state_config->unique_name);

	// config
	STATE_add_edge(t_state_config, condition_USART_r, t_state_config_rounds->unique_name);
	STATE_add_edge(t_state_config, condition_USART_s, t_state_wait_start->unique_name);
	STATE_add_edge(t_state_config, condition_USART_lfconfigstatic, t_state_config_lf_static->unique_name);
	STATE_add_edge(t_state_config, condition_USART_helper_clear_invalid_input, t_state_error->unique_name);
	// config rounds
	STATE_add_edge(t_state_config_rounds, condition_USART_c, t_state_config->unique_name);
	STATE_add_edge(t_state_config_rounds, condition_USART_isdigit, t_state_config_rounds->unique_name);
	STATE_add_edge(t_state_config_rounds, condition_USART_helper_clear_invalid_input, t_state_error->unique_name);
	// config lf static
	STATE_add_edge(t_state_config_lf_static, condition_USART_c, t_state_config->unique_name);
	STATE_add_edge_with_execute(t_state_config_lf_static, condition_USART_lnum, execute_set_robi_lf_l_threshold, t_state_config_rounds->unique_name);
	STATE_add_edge_with_execute(t_state_config_lf_static, condition_USART_mnum, execute_set_robi_lf_m_threshold, t_state_config_rounds->unique_name);
	STATE_add_edge_with_execute(t_state_config_lf_static, condition_USART_rnum, execute_set_robi_lf_r_threshold, t_state_config_rounds->unique_name);
	STATE_add_edge(t_state_wait_start, condition_USART_helper_clear_invalid_input, t_state_error->unique_name);
	// wait start
	STATE_add_edge(t_state_wait_start, condition_USART_s, t_state_drive_throught->unique_name);
	STATE_add_edge(t_state_wait_start, condition_USART_c, t_state_config->unique_name);
	STATE_add_edge(t_state_wait_start, condition_USART_helper_clear_invalid_input, t_state_error->unique_name);

	// drive_logic_super_state
	STATE_add_edge( t_state_drive_logic_super_state, condition_LF_LMR, t_state_check_for_start->unique_name);

	// drive_through_start
	STATE_add_edge( t_state_drive_throught, condition_LF_NOT_LMR, t_state_forward->unique_name);

	// check for start field
	STATE_add_edge( t_state_check_for_start, condition_start_field_delay, t_state_stop->unique_name);
	STATE_add_edge( t_state_check_for_start, condition_check_for_start_to_forward, t_state_forward->unique_name);

	// forward
	STATE_add_edge( t_state_forward, condition_forward_to_left, t_state_left->unique_name);
	STATE_add_edge( t_state_forward, condition_forward_to_right, t_state_right->unique_name);
	STATE_add_edge( t_state_forward, condition_forward_to_backwards, t_state_backwards->unique_name);

	//backward
	STATE_add_edge( t_state_backwards, condition_backwards_to_forward, t_state_forward->unique_name);

	// left
	STATE_add_edge( t_state_left, condition_left_to_hard_left, t_state_hard_left->unique_name);
	STATE_add_edge( t_state_left, condition_left_to_forward, t_state_forward->unique_name);

	STATE_add_edge( t_state_hard_left, condition_hard_left_to_left, t_state_left->unique_name);

	// right
	STATE_add_edge( t_state_right, condition_right_to_hard_right, t_state_hard_right->unique_name);
	STATE_add_edge( t_state_right, condition_right_to_forward, t_state_forward->unique_name);

	STATE_add_edge( t_state_hard_right, condition_hard_right_to_right, t_state_right->unique_name);

	// stop
	STATE_add_edge(t_state_stop, condition_has_rounds, t_state_drive_throught->unique_name);
	STATE_add_edge(t_state_stop, condition_has_no_rounds, t_state_config->unique_name);

	INFO("[configure_state_machine] all edges added\n");

	// configure state hierarchy
	STATE_set_parent(t_state_forward, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_backwards, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_left, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_hard_left, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_right, t_state_drive_logic_super_state);
	STATE_set_parent(t_state_hard_right, t_state_drive_logic_super_state);

	INFO("[configure_state_machine] all state herarchy set\n");

	// configure state machine
	add_error_state(state_machine, t_state_error);

	add_state(state_machine, t_state_config);
	add_state(state_machine, t_state_config_rounds);
	add_state(state_machine, t_state_config_lf_static);
	add_state(state_machine, t_state_wait_start);

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

	INFO("[configure_state_machine] all states added\n");

	int8_t rc = set_start_state(state_machine, t_state_init_robi->unique_name);
	if (0 != rc) { // set_start_state failed
		set_current_state(state_machine, state_machine->tp_error_state->unique_name);
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
