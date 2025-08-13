// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "state_machine/state.h"
#include "state_machine/state_machine.h"

#include "roboter/states_config.h"
#include "roboter/states_drive.h"
#include "roboter/states_general.h"

#include "roboter/conditions_LF.h"
#include "roboter/conditions_USART.h"
#include "roboter/conditions_clock.h"
#include "roboter/conditions_general.h"
#include "roboter/conditions_roboter.h"

#include "roboter/execute_USART.h"
#include "roboter/execute_config.h"

#include "tools/iesusart.h"
#include "tools/logger.h"

#include <avr/io.h>
#include <stdint.h>

t_state_machine *configure_state_machine() {

  // init state machine
  t_state_machine *state_machine = STATE_MACHINE_constructor();
  if (NULL == state_machine) {
    FATAL("Failed to construct state_machine.\n");
    return NULL;
  }

  // init states
  // config
  t_state *t_state_config_super_state = STATE_constructor(nothing_on_entry, nothing_on_update);
  t_state *t_state_init_robi = STATE_constructor(init_robi_on_entry, init_robi_on_update);

  t_state *t_state_config = STATE_constructor(config_on_entry, config_on_update);
  t_state *t_state_config_rounds = STATE_constructor(config_rounds_on_entry,
                                                     config_rounds_on_update);
  t_state *t_state_config_lf_static = STATE_constructor(config_lf_static_on_entry,
                                                        config_lf_static_on_update);
  t_state *t_state_config_lf_static_left = STATE_constructor(lf_set_treshold_prompt_on_entry,
                                                             nothing_on_entry);
  t_state *t_state_config_lf_static_middle = STATE_constructor(lf_set_treshold_prompt_on_entry,
                                                               nothing_on_entry);
  t_state *t_state_config_lf_static_right = STATE_constructor(lf_set_treshold_prompt_on_entry,
                                                              nothing_on_entry);
  t_state *t_state_waiting = STATE_constructor(waiting_on_entry, waiting_on_update);
  t_state *t_state_searching = STATE_constructor(searching_on_entry, searching_on_update);
  t_state *t_state_resetting = STATE_constructor(reset_on_entry, reset_on_update);

  // driving
  t_state *t_state_check_for_start = STATE_constructor(check_is_start_field_on_entry,
                                                       check_is_start_field_on_update);
  t_state *t_state_drive_throught = STATE_constructor(drive_through_start_on_entry,
                                                      drive_through_start_on_entry_on_update);
  t_state *t_state_forward = STATE_constructor(forward_on_entry, forward_on_update);
  t_state *t_state_backwards = STATE_constructor(backwards_on_entry, backwards_on_update);
  t_state *t_state_left = STATE_constructor(left_on_entry, left_on_update);
  t_state *t_state_hard_left = STATE_constructor(hard_left_on_entry, hard_left_on_update);
  t_state *t_state_right = STATE_constructor(right_on_entry, right_on_update);
  t_state *t_state_hard_right = STATE_constructor(hard_right_on_entry, hard_right_on_update);
  t_state *t_state_stop = STATE_constructor(stop_on_entry, stop_on_update);
  t_state *t_state_error = STATE_constructor(error_on_entry, error_on_update);
  t_state *t_state_drive_logic_super_state = STATE_constructor(drive_logic_super_state_on_entry,
                                                               drive_logic_super_state_on_update);

  t_state *t_state_pause = STATE_constructor(pause_on_entry, pause_on_update);

  INFO("[configure_state_machine] all states constructed\n");

  // configure states
  // init
  STATE_add_edge(t_state_init_robi, 
      condition_allways, 
      t_state_waiting);

  STATE_add_edge(t_state_config_super_state, condition_USART_W, t_state_waiting);
  // config
  STATE_add_edge(t_state_config, condition_USART_R, t_state_config_rounds);
  STATE_add_edge(t_state_config, condition_USART_L, t_state_config_lf_static);
  // config rounds
  STATE_add_edge(t_state_config_rounds, condition_USART_C, t_state_config);
  STATE_add_edge_with_execute(t_state_config_rounds, condition_USART_isdigit,
                              execute_set_robi_rounds, t_state_config);
  // config lf static
  STATE_add_edge(t_state_config_lf_static, condition_USART_C, t_state_config);
  STATE_add_edge(t_state_config_lf_static, condition_USART_L, t_state_config_lf_static_left);
  STATE_add_edge(t_state_config_lf_static, condition_USART_M, t_state_config_lf_static_middle);
  STATE_add_edge(t_state_config_lf_static, condition_USART_R, t_state_config_lf_static_right);
  // config lf static left
  STATE_add_edge(t_state_config_lf_static_left, condition_USART_C, t_state_config_lf_static);
  STATE_add_edge_with_execute(t_state_config_lf_static_left, condition_USART_isdigit,
                              execute_set_robi_lf_l_threshold, t_state_config_lf_static);
  // config lf static middle
  STATE_add_edge(t_state_config_lf_static_middle, condition_USART_C, t_state_config_lf_static);
  STATE_add_edge_with_execute(t_state_config_lf_static_middle, condition_USART_isdigit,
                              execute_set_robi_lf_m_threshold, t_state_config_lf_static);
  // config lf static right
  STATE_add_edge(t_state_config_lf_static_right, condition_USART_C, t_state_config_lf_static);
  STATE_add_edge_with_execute(t_state_config_lf_static_right, condition_USART_isdigit,
                              execute_set_robi_lf_r_threshold, t_state_config_lf_static);
  // waiting
  STATE_add_edge(t_state_waiting, condition_LF_NEITHER_L_M_R, t_state_searching);
  STATE_add_edge_with_execute(t_state_waiting, condition_USART_S, execute_print_fresh_start,
                              t_state_drive_throught);
  STATE_add_edge_with_execute(t_state_waiting, condition_USART_questionmark,
                              execute_print_waiting_help, t_state_waiting);
  STATE_add_edge(t_state_waiting, condition_USART_C, t_state_config);
  // searching
  STATE_add_edge(t_state_searching, condition_LF_L_M_R, t_state_waiting);

  // drive_logic_super_state
  STATE_add_edge(t_state_drive_logic_super_state, condition_LF_L_M_R, t_state_check_for_start);
  STATE_add_edge(t_state_drive_logic_super_state, condition_USART_P, t_state_pause);

  // pause
  STATE_add_edge(t_state_pause, condition_USART_P, t_state_forward);

  // drive_through_start
  STATE_add_edge(t_state_drive_throught, condition_LF_NEITHER_L_M_R, t_state_forward);

  // check for start field
  STATE_add_edge(t_state_check_for_start, condition_start_field_delay, t_state_stop);
  STATE_add_edge(t_state_check_for_start, condition_LF_nL_nM_nR, t_state_forward);

  // forward
  STATE_add_edge(t_state_forward, condition_LF_L_X_nR, t_state_left);
  STATE_add_edge(t_state_forward, condition_LF_nL_X_R, t_state_right);
  STATE_add_edge(t_state_forward, condition_LF_nL_nM_nR, t_state_backwards);

  // backward
  STATE_add_edge(t_state_backwards, condition_LF_L_nM_X, t_state_hard_left);
  STATE_add_edge(t_state_backwards, condition_LF_X_nM_R, t_state_hard_right);
  STATE_add_edge(t_state_backwards, condition_LF_L_M_nR, t_state_hard_left);
  STATE_add_edge(t_state_backwards, condition_LF_nL_M_R, t_state_hard_right);

  // left
  STATE_add_edge(t_state_left, condition_LF_X_nM_X, t_state_hard_left);
  STATE_add_edge(t_state_left, condition_LF_nL_nM_nR, t_state_hard_left);
  STATE_add_edge(t_state_left, condition_LF_nL_M_nR, t_state_forward);

  STATE_add_edge(t_state_hard_left, condition_LF_nL_nM_nR, t_state_hard_left);
  STATE_add_edge(t_state_hard_left, condition_LF_X_M_X, t_state_left);

  // right
  STATE_add_edge(t_state_right, condition_LF_X_nM_X, t_state_hard_right);
  STATE_add_edge(t_state_right, condition_LF_nL_nM_nR, t_state_hard_right);
  STATE_add_edge(t_state_right, condition_LF_nL_M_nR, t_state_forward);

  STATE_add_edge(t_state_hard_right, condition_LF_nL_nM_nR, t_state_hard_right);
  STATE_add_edge(t_state_hard_right, condition_LF_X_M_X, t_state_right);

  // stop
  STATE_add_edge(t_state_stop, condition_has_rounds, t_state_drive_throught);
  STATE_add_edge(t_state_stop, condition_has_no_rounds, t_state_resetting);

  // resetting
  STATE_add_edge(t_state_resetting, condition_5_seconds_after_entry, t_state_waiting);

  INFO("[configure_state_machine] all edges added\n");

  // configure state hierarchy
  STATE_set_parent(t_state_forward, t_state_drive_logic_super_state);
  STATE_set_parent(t_state_backwards, t_state_drive_logic_super_state);
  STATE_set_parent(t_state_left, t_state_drive_logic_super_state);
  STATE_set_parent(t_state_hard_left, t_state_drive_logic_super_state);
  STATE_set_parent(t_state_right, t_state_drive_logic_super_state);
  STATE_set_parent(t_state_hard_right, t_state_drive_logic_super_state);

  STATE_set_parent(t_state_config, t_state_config_super_state);
  STATE_set_parent(t_state_config_lf_static, t_state_config_super_state);
  STATE_set_parent(t_state_config_lf_static_left, t_state_config_super_state);
  STATE_set_parent(t_state_config_lf_static_middle, t_state_config_super_state);
  STATE_set_parent(t_state_config_lf_static_right, t_state_config_super_state);
  STATE_set_parent(t_state_config_rounds, t_state_config_super_state);

  INFO("[configure_state_machine] all state herarchy set\n");

  // configure state machine
  STATE_MACHINE_add_error_state(state_machine, t_state_error);

  STATE_MACHINE_add_state(state_machine, t_state_config);
  STATE_MACHINE_add_state(state_machine, t_state_config_super_state);
  STATE_MACHINE_add_state(state_machine, t_state_config_rounds);
  STATE_MACHINE_add_state(state_machine, t_state_config_lf_static);
  STATE_MACHINE_add_state(state_machine, t_state_config_lf_static_left);
  STATE_MACHINE_add_state(state_machine, t_state_config_lf_static_middle);
  STATE_MACHINE_add_state(state_machine, t_state_config_lf_static_right);
  STATE_MACHINE_add_state(state_machine, t_state_waiting);
  STATE_MACHINE_add_state(state_machine, t_state_searching);
  STATE_MACHINE_add_state(state_machine, t_state_resetting);

  STATE_MACHINE_add_state(state_machine, t_state_drive_throught);
  STATE_MACHINE_add_state(state_machine, t_state_forward);
  STATE_MACHINE_add_state(state_machine, t_state_backwards);
  STATE_MACHINE_add_state(state_machine, t_state_left);
  STATE_MACHINE_add_state(state_machine, t_state_hard_left);
  STATE_MACHINE_add_state(state_machine, t_state_right);
  STATE_MACHINE_add_state(state_machine, t_state_hard_right);
  STATE_MACHINE_add_state(state_machine, t_state_init_robi);
  STATE_MACHINE_add_state(state_machine, t_state_check_for_start);
  STATE_MACHINE_add_state(state_machine, t_state_stop);

  STATE_MACHINE_add_state(state_machine, t_state_drive_logic_super_state);
  STATE_MACHINE_add_state(state_machine, t_state_pause);

  INFO("[configure_state_machine] all states added\n");

  int8_t rc = STATE_MACHINE_set_start_state(state_machine, t_state_init_robi);
  if (0 != rc) {  // set_start_state failed
    STATE_MACHINE_set_current_state(state_machine, state_machine->tp_error_state);
  }

  return state_machine;
}

int main() {
  USART_init(UBRR_SETTING);
  INFO("USART_init\n");

  t_state_machine *state_machine = configure_state_machine();
  if (NULL == state_machine) {
    return 1;
  }

  INFO("[main] now starting state machine\n");
  STATE_MACHINE_run(state_machine);

  return 0;
}
