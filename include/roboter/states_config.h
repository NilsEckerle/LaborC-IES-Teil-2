#ifndef STATES_CONFIG_H
#define STATES_CONFIG_H

#include "state_machine/state.h"

void init_robi_on_entry(t_state *inst __attribute__((unused)));

void init_robi_on_update(t_state*inst __attribute__((unused)));

void config_on_entry(t_state *inst __attribute__((unused)));

void config_on_update(t_state*inst __attribute__((unused)));

void config_rounds_on_entry(t_state *inst __attribute__((unused)));

void config_rounds_on_update(t_state*inst __attribute__((unused)));

void config_lf_static_on_entry(t_state *inst __attribute__((unused)));

void config_lf_static_on_update(t_state *inst __attribute__((unused)));

void waiting_on_entry(t_state *inst __attribute__((unused)));

void waiting_on_update(t_state*inst __attribute__((unused)));

#endif // STATES_CONFIG_H
