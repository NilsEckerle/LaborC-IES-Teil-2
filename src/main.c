#define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"
#include <stdint.h>
#include <util/delay.h>
#include "state_machine/state_machine.h"
#include "state_machine/state.h"
#include "tools/iesusart.h"
#include <avr/io.h>
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"
#include "hardware/engine.h"

void init_robi_on_entry(t_state *inst __attribute__((unused))) {
	INFO("init\n");
	SHIFT_init();
	LF_init();
	ENGINE_init();
	return;
}

void init_robi_on_update(t_state*inst __attribute__((unused))) {
	INFO("init update\n");
	return;
}

void nothing_on_entry(t_state*inst __attribute__((unused))) {
	INFO("nothing\n");
	return;
}

void nothing_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void forward_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_M);
	INFO("forward\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/4));		// set to 3/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/4));	// set to 3/4 speed
																											// 1111 1111 = 255
																											// 0011 1111 = 63  = 255/4
																											// 1100 0000 = 192 = ~63
																											
  // Left motors forward
	ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
	// Right motors forward
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
	return;
}

void forward_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void backwards_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_M);
	INFO("forward\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/4));		// set to 3/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/4));	// set to 3/4 speed
																											// 1111 1111 = 255
																											// 0011 1111 = 63  = 255/4
																											// 1100 0000 = 192 = ~63
																											
  // Left motors forward
	ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);
	// Right motors forward
	ENGINE_HB_IN3_PORT |= (1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
	_delay_ms(200);
	return;
}

void backwards_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void left_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_LM);
	INFO("left\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, (255/4));		// set to 1/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/6));	// set to 5/6 speed

	// Left motors forward
	ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
	// Right motors forward
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
	return;
}

void left_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void hard_left_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_L);
	INFO("hard left\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255/8));		// set to 1/8 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/8));	// set to 7/8 speed

	// Left motors backwards
	ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT |= (1 << ENGINE_HB_IN2_BIT);

	// Right motors forward
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
}

void hard_left_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void right_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_MR);
	INFO("right\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/6));		// set to 5/6 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255/4));		// set to 1/4 speed

	// Left motors forward
	ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
	// Right motors stop
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
	return;
}

void right_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void hard_right_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_R);
	INFO("hard right\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255/8));		// set to 1/8 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/8));		// set to 7/8 speed

	// Left motors forward
	ENGINE_HB_IN1_PORT |= (1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
	// Right motors forward
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT |= (1 << ENGINE_HB_IN4_BIT);
	return;
}

void hard_right_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void stop_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_LMR);
	INFO("stop\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, 255);	// set to full speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, 255); // set to full speed

	// Stop left motors
	ENGINE_HB_IN1_PORT &= ~(1 << ENGINE_HB_IN1_BIT);
	ENGINE_HB_IN2_PORT &= ~(1 << ENGINE_HB_IN2_BIT);
	// Stop right motors
	ENGINE_HB_IN3_PORT &= ~(1 << ENGINE_HB_IN3_BIT);
	ENGINE_HB_IN4_PORT &= ~(1 << ENGINE_HB_IN4_BIT);
	return;
}

void stop_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void error_on_entry(t_state*inst __attribute__((unused))) {
	stop_on_entry(inst);
	return;
}

void error_on_update(t_state*inst __attribute__((unused))) {
	USART_print("ERROR STATE!\n");
	return;
}

/********************
* CONDITIONS
********************/

uint8_t condition_allways() {
	return 1;
}

uint8_t condition_forward_to_left() {
	if (LF_get_states() == (LF_detection_state)LF_LM) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_hard_left() {
	if (LF_get_state(0) == (LF_detection_state)LF_L) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_right() {
	if (LF_get_states() == (LF_detection_state)LF_MR) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_hard_right() {
	if (LF_get_state(0) == (LF_detection_state)LF_R) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_stop() {
	if (LF_get_states() == (LF_detection_state)LF_LMR) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_backwards() {
	if (LF_get_states() == (LF_detection_state)LF_NONE) {
		return 1;
	}
	return 0;
}

uint8_t condition_nothing_to_forward() {
	if (LF_get_states() == (LF_detection_state)LF_NONE) {
		return 1;
	}
	return 0;
}

uint8_t condition_left_to_forward() {
	if (LF_get_state(0) == 0) {
		return 1;
	}
	return 0;
}

uint8_t condition_left_to_hard_left() {
	if (LF_get_state(1) == 0) {
		return 1;
	}
	return 0;
}

uint8_t condition_hard_left_to_left() {
	if (LF_get_state(1) == 1) {
		return 1;
	}
	return 0;
}

uint8_t condition_right_to_forward() {
	if (LF_get_state(2) == 0) {
		return 1;
	}
	return 0;
}

uint8_t condition_right_to_hard_right() {
	if (LF_get_state(1) == 0) {
		return 1;
	}
	return 0;
}

uint8_t condition_hard_right_to_right() {
	if (LF_get_state(1) == 1) {
		return 1;
	}
	return 0;
}


int main() {
	USART_init(UBRR_SETTING);
	USART_print("USART_init\n");

	LF_init();

	// init state machine
	t_state_machine state_machine;
	int8_t rc = STATE_MACHINE_constructor(&state_machine);
	if (0 != rc) {
		FATAL("Failed to construct state_machine.\n");
		return 1;
	}

	// init states
	t_state t_state_init_robi;
	t_state t_state_forward;
	t_state t_state_backwards;
	t_state t_state_left;
	t_state t_state_hard_left;
	t_state t_state_right;
	t_state t_state_hard_right;
	t_state t_state_stop;
	t_state t_state_error;

	rc = STATE_constructor(&t_state_init_robi, "init_robi", init_robi_on_entry, init_robi_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_forward, "forward", forward_on_entry, forward_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_left, "left", left_on_entry, left_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_hard_left, "hard_left", hard_left_on_entry, hard_left_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_right, "right", right_on_entry, right_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_hard_right, "hard_right", hard_right_on_entry, hard_right_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_stop, "stop", stop_on_entry, stop_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_backwards, "backwards", backwards_on_entry, backwards_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&t_state_error, "error", stop_on_entry, error_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	INFO("[main] all states constructed\n");

	// configure states
	t_state_init_robi.add_edge(&t_state_init_robi, condition_allways, t_state_forward.unique_name);

	t_state_forward.add_edge(&t_state_forward, condition_forward_to_left, t_state_left.unique_name);
	t_state_forward.add_edge(&t_state_forward, condition_forward_to_right, t_state_right.unique_name);
	t_state_forward.add_edge(&t_state_forward, condition_forward_to_stop, t_state_stop.unique_name);
	t_state_forward.add_edge(&t_state_forward, condition_forward_to_backwards, t_state_backwards.unique_name);

	t_state_backwards.add_edge(&t_state_backwards, condition_allways, t_state_forward.unique_name);

	t_state_left.add_edge(&t_state_left, condition_left_to_forward, t_state_forward.unique_name);
	t_state_left.add_edge(&t_state_left, condition_left_to_hard_left, t_state_hard_left.unique_name);

	t_state_hard_left.add_edge(&t_state_hard_left, condition_hard_left_to_left, t_state_left.unique_name);

	t_state_right.add_edge(&t_state_right, condition_right_to_hard_right, t_state_hard_right.unique_name);

	t_state_hard_right.add_edge(&t_state_hard_right, condition_hard_right_to_right, t_state_right.unique_name);

	t_state_stop.add_edge(&t_state_stop, condition_allways, t_state_forward.unique_name);

	INFO("[main] all edges added\n");

	// configure state machine
	state_machine.add_error_state(&state_machine, &t_state_error);
	state_machine.add_state(&state_machine, &t_state_forward);
	state_machine.add_state(&state_machine, &t_state_backwards);
	state_machine.add_state(&state_machine, &t_state_left);
	state_machine.add_state(&state_machine, &t_state_hard_left);
	state_machine.add_state(&state_machine, &t_state_right);
	state_machine.add_state(&state_machine, &t_state_hard_right);
	state_machine.add_state(&state_machine, &t_state_init_robi);
	state_machine.add_state(&state_machine, &t_state_stop);

	INFO("[main] all states added\n");

	rc = state_machine.set_start_state(&state_machine, t_state_init_robi.unique_name);
	if (0 != rc) { // set_start_state failed
		state_machine.set_current_state(&state_machine, state_machine.tp_error_state->unique_name);
	}

	INFO("[main] now running state machine\n");

	state_machine.run(&state_machine);

	return 0;
}
