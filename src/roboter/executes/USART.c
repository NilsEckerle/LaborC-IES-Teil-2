#include "roboter/executes/USART.h"
#include "configuration/serial_messages.h"
#include "tools/logger.h"

void execute_print_waiting_help(t_state *tp_current_state __attribute__((unused)),
                                void *vp_dto __attribute__((unused))) {
  PRINT_WAITING_HELP();
}

void execute_print_fresh_start(t_state *tp_current_state __attribute__((unused)),
                               void *vp_dto __attribute__((unused))) {
  UI(MSG_FRESH_START);
}
