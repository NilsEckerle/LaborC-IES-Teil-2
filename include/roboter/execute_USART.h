#ifndef EXECUTE_USART_H
#define EXECUTE_USART_H

#include "state_machine/state.h"

void execute_print_waiting_help(t_state *tp_current_state
                                __attribute__((unused)),
                                void *vp_dto);

void execute_print_fresh_start(t_state *tp_current_state
                               __attribute__((unused)),
                               void *vp_dto);

#endif // EXECUTE_USART_H
