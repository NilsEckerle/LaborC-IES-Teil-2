#ifndef GENERATE_STATE_MACHINE_DIAGRAM
#include "roboter/execute_USART.h"
#include "configuration/serial_messages.h"
#include "roboter/roboter.h"
#include "tools/logger.h"

void execute_print_waiting_help(t_state *tp_current_state __attribute__((unused)),
                                void *vp_dto __attribute__((unused))) {
  PRINT_WAITING_HELP();
}

void execute_print_fresh_start(t_state *tp_current_state __attribute__((unused)),
                               void *vp_dto __attribute__((unused))) {
  UI(MSG_FRESH_START);
}
#else
#include "roboter/execute_USART.h"

void execute_print_waiting_help(t_state *tp_current_state __attribute__((unused)),
                                void *vp_dto __attribute__((unused))) {
}

void execute_print_fresh_start(t_state *tp_current_state __attribute__((unused)),
                               void *vp_dto __attribute__((unused))) {
}
#endif // !GENERATE_STATE_MACHINE_DIAGRAM
