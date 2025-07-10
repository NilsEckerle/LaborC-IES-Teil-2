#include "roboter/roboter_states.h"
#include "hardware/engine.h"
#include "hardware/linienfolger.h"
#include "hardware/shiftregister.h"
#include "tools/bit_functions.h"
#define LOG_LEVEL LOG_LEVEL_INFO
#include "tools/logger.h"

void init_robi_on_entry(t_state *inst __attribute__((unused))) {
	INFO("init\n");
	SHIFT_init();
	LF_init();
	ENGINE_init();
	return;
}

void init_robi_on_update(t_state*inst __attribute__((unused))) {
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
	SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Right motors forward
	UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
	return;
}

void forward_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void backwards_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_M);
	INFO("backwards\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/4));		// set to 3/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/4));	// set to 3/4 speed
																											// 1111 1111 = 255
																											// 0011 1111 = 63  = 255/4
																											// 1100 0000 = 192 = ~63
																											
  // Left motors forward
	UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Right motors forward
	SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
	return;
}

void backwards_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void left_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_LM);
	INFO("left\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, (255/16));		// set to 1/16 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/8));	// set to 7/8 speed

	// Left motors backwards
	UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Right motors forward
	UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
	return;
}

void left_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void hard_left_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_L);
	INFO("hard left\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/4));		// set to 3/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/8));	// set to 7/8 speed

	// Left motors backwards
	UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	SET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);

	// Right motors forward
	UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
}

void hard_left_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void right_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_MR);
	INFO("right\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/8));		// set to 7/8 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, (255/16));		// set to 1/16 speed

	// Left motors forward
	SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Right motors backwards
	SET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
	return;
}

void right_on_update(t_state*inst __attribute__((unused))) {
	return;
}

void hard_right_on_entry(t_state*inst __attribute__((unused))) {
	SHIFT_push_state((LF_detection_state)LF_R);
	INFO("hard right\n");

	ENGINE_set_duty_cicle(ENGINE_PWM_LEFT, ~(255/4));		// set to 3/4 speed
	ENGINE_set_duty_cicle(ENGINE_PWM_RIGHT, ~(255/8));		// set to 7/8 speed

	// Left motors forward
	SET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Right motors forward
	UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	SET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
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
	UNSET_BIT(ENGINE_HB_IN1_PORT, ENGINE_HB_IN1_BIT);
	UNSET_BIT(ENGINE_HB_IN2_PORT, ENGINE_HB_IN2_BIT);
	// Stop right motors
	UNSET_BIT(ENGINE_HB_IN3_PORT, ENGINE_HB_IN3_BIT);
	UNSET_BIT(ENGINE_HB_IN4_PORT, ENGINE_HB_IN4_BIT);
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
	if (LF_get_state(0) == 0 && LF_get_state(2) == 1) {
		return 1;
	}
	return 0;
}

uint8_t condition_forward_to_right() {
	if (LF_get_state(0) == 1 && LF_get_state(2) == 0) {
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

uint8_t condition_backwards_to_forward() {
	if (LF_get_states() != LF_NONE) {
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

