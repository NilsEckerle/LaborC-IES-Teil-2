#ifndef CONDITIONS_USART_H
#define CONDITIONS_USART_H

#include "state_machine/state.h"

uint8_t condition_USART_helper_clear_invalid_input(t_state *inst __attribute__((unused)),
                                                   void *vp_dto __attribute__((unused)));

uint8_t condition_USART_R(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_M(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_L(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_S(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_C(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_W(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_P(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused)));

uint8_t condition_USART_questionmark(t_state *inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

uint8_t condition_USART_lfconfigstatic(t_state *inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused)));

uint8_t condition_USART_isdigit(t_state *inst __attribute__((unused)),
                                void *vp_dto __attribute__((unused)));

#endif // CONDITIONS_USART_H
