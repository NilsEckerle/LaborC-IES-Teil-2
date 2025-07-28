#ifndef EXECUTE_CONFIG_H
#define EXECUTE_CONFIG_H

#include "state_machine/state.h"

void execute_set_robi_lf_l_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto);
void execute_set_robi_lf_m_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto);
void execute_set_robi_lf_r_threshold(t_state *tp_current_state __attribute__((unused)), void *vp_dto);

void execute_set_robi_rounds(t_state *tp_current_state __attribute__((unused)), void *vp_dto);

#endif // EXECUTE_CONFIG_H
