#include "tools/iesusart.h"
#include <stdint.h>
#define LOG_LEVEL LOG_LEVEL_INFO_TRACE
#include "tools/logger.h"
#include "state_machine/state_machine.h"
#include "state_machine/state.h"

void print_hello_on_entry(t_state *inst) {
	INFO_SPAM("hello_print state was entered.\n");
}

void print_hello_on_update(t_state *inst) {
	INFO_SPAM("hello_print update.\n");
}

void print_good_by_on_entry(t_state *inst) {
	INFO_SPAM("good_by_print state was entered.\n");
}

void print_good_by_on_update(t_state *inst) {
	INFO_SPAM("good_by_print update.\n");
}

void print_error_on_entry(t_state *inst) {
	INFO_SPAM("good_error state was entered.\n");
}

void print_error_on_update(t_state *inst) {
	INFO_SPAM("good_error update.\n");
}

uint8_t condition_hello_to_good_by() {

	return 0;
}

uint8_t condition_hello_to_error() {

	return 0;
}

uint8_t condition_good_by_to_error() {

	return 0;
}

int main() {
	USART_init(UBRR_SETTING);
	USART_print("USART_init");

	// init state machine
	t_state_machine state_machine;
	int8_t rc = STATE_MACHINE_constructor(&state_machine);
	if (0 != rc) {
		FATAL("Failed to construct state_machine.\n");
		return 1;
	}

	// init states
	t_state state_print_hello;
	t_state state_print_good_by;
	t_state state_print_error;

	rc = STATE_constructor(&state_print_hello, "print_hello", print_hello_on_entry, print_hello_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&state_print_good_by, "print_good_by", print_good_by_on_entry, print_good_by_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	rc = STATE_constructor(&state_print_error, "print_error", print_error_on_entry, print_error_on_update);
	if (0 != rc) {
		FATAL("Failed to construct state.\n");
		return 1;
	}

	// configure states
	state_print_hello.add_edge(&state_print_hello, condition_hello_to_good_by, state_print_hello.unique_name);
	state_print_hello.add_edge(&state_print_hello, condition_hello_to_error, state_print_error.unique_name);
	state_print_hello.add_edge(&state_print_good_by, condition_good_by_to_error, state_print_error.unique_name);

	// configure state machine
	state_machine.add_error_state(&state_machine, &state_print_error);
	state_machine.add_state(&state_machine, &state_print_hello);
	state_machine.add_state(&state_machine, &state_print_good_by);

	state_machine.set_start_state(&state_machine, state_print_hello.unique_name);

	state_machine.run(&state_machine);

	return 0;
}
