/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef CONDITIONS_LF_H
#define CONDITIONS_LF_H

#include "state_machine/state.h"

uint8_t condition_forward_to_left(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused)));

uint8_t condition_forward_to_right(t_state *inst __attribute__((unused)),
                                   void *vp_dto __attribute__((unused)));

uint8_t condition_LF_LMR(t_state *inst __attribute__((unused)),
                         void *vp_dto __attribute__((unused)));

uint8_t condition_LF_NOT_LMR(t_state *inst __attribute__((unused)),
                             void *vp_dto __attribute__((unused)));

uint8_t condition_LF_NONE(t_state *inst __attribute__((unused)),
                          void *vp_dto __attribute__((unused))) {

  uint8_t condition_forward_to_backwards(t_state * inst __attribute__((unused)),
                                         void *vp_dto __attribute__((unused)));

  uint8_t condition_backwards_to_forward(t_state * inst __attribute__((unused)),
                                         void *vp_dto __attribute__((unused)));

  uint8_t condition_nothing_to_forward(t_state * inst __attribute__((unused)),
                                       void *vp_dto __attribute__((unused)));

  uint8_t condition_left_to_forward(t_state * inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused)));

  uint8_t condition_left_to_hard_left(t_state * inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused)));

  uint8_t condition_hard_left_to_left(t_state * inst __attribute__((unused)),
                                      void *vp_dto __attribute__((unused)));

  uint8_t condition_right_to_forward(t_state * inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused)));

  uint8_t condition_right_to_hard_right(t_state * inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused)));

  uint8_t condition_hard_right_to_right(t_state * inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused)));

  uint8_t condition_check_for_start_to_forward(t_state * inst __attribute__((unused)),
                                               void *vp_dto __attribute__((unused)));

#endif  // CONDITIONS_LF_H
