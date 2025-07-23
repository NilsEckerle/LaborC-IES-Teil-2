#ifndef CONDITIONS_USART_H
#define CONDITIONS_USART_H

#include "state_machine/state.h"

uint8_t condition_USART_helper_clear_invalid_input(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

uint8_t condition_USART_r(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));
uint8_t condition_USART_m(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));
uint8_t condition_USART_l(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

uint8_t condition_USART_s(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

uint8_t condition_USART_c(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

uint8_t condition_USART_lfconfigstatic(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));


uint8_t condition_USART_isdigit(t_state *inst __attribute__((unused)), void *vp_dto __attribute__((unused)));

#endif // CONDITIONS_USART_H
