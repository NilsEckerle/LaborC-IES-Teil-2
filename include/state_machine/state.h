/**
 * @file state.h
 * @brief State management system for hierarchical state machines
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef STATE_H
#define STATE_H

#include "tools/dynamic_array.h"
#include <stdint.h>

typedef struct state_machine t_state_machine;

/**
 * @brief Individual state in a hierarchical state machine
 *
 * This structure represents a single state that can contain multiple edges
 * (transitions) to other states, support hierarchical relationships with
 * parent states, and execute custom entry and update logic.
 *
 * @note States support hierarchical organization where child states can
 *       inherit behavior from parent states. Parent state conditions are
 *       checked before child state conditions during edge evaluation.
 */
typedef struct state {
  t_dyn_arr *tdynarr_edges;            /**< Dynamic array of t_edge pointers for state transitions */
  uint32_t *ui32p_state_entry_time_ms; /**< Timestamp when state was entered (milliseconds) */
  struct state *tp_parent;             /**< Pointer to parent state for hierarchical organization */

  /**
   * @brief Function called once when entering this state
   * @param tp_state Pointer to the state being entered
   */
  void (*on_entry)(struct state *tp_state);

  /**
   * @brief Function called every update cycle while in this state
   * @param tp_state Pointer to the current state
   */
  void (*on_update)(struct state *tp_state);

} t_state;

/**
 * @brief Transition edge between states with condition and execution logic
 *
 * This structure defines a conditional transition from one state to another.
 * Each edge contains a condition function that determines when the transition
 * should occur and an optional execution function that runs during the transition.
 */
typedef struct edge {
  void *vp_dto;                       /**< Data transfer object passed to condition and execution functions */
  
  /**
   * @brief Condition function that determines if transition should occur
   * @param tp_state Pointer to the current state
   * @param vp_dto Data transfer object with transition data
   * @return 1 if transition should occur, 0 otherwise
   */
  uint8_t (*condition)(t_state *tp_state, void *vp_dto);
  
  /**
   * @brief Optional function executed when transition occurs
   * @param tp_current_state Pointer to the state being exited
   * @param vp_dto Data transfer object with transition data
   */
  void (*fp_execute_on_transition)(t_state *tp_current_state, void *vp_dto);
  
  t_state *state;                     /**< Pointer to the target state for this transition */

} t_edge;

/**
 * @brief Adds a transition edge with custom execution logic to a state
 * 
 * Creates a new transition from the specified state to the target state with
 * a condition function and an execution function that runs during the transition.
 * 
 * @param tp_state Source state to add the transition to
 * @param condition Function that determines when transition should occur
 * @param fp_execute_on_transition Function to execute during transition (can be NULL)
 * @param next_state_name Target state for the transition
 * @return 0 on success, positive value on error:
 *         - 1: Invalid parameter (NULL pointer)
 *         - 2: Memory allocation failure for edge
 *         - 3: Failed to add edge to dynamic array
 * 
 * @note The condition function is called every update cycle to check if
 *       the transition should occur. The execution function runs once
 *       when the transition is triggered.
 */
int8_t STATE_add_edge_with_execute(
    t_state *tp_state,
    uint8_t (*condition)(t_state *tp_current_state, void *vp_dto),
    void (*fp_execute_on_transition)(t_state *tp_current_state, void *vp_dto),
    t_state *next_state_name);

/**
 * @brief Adds a simple transition edge to a state
 * 
 * Creates a new transition from the specified state to the target state with
 * only a condition function. No execution logic runs during the transition.
 * 
 * @param tp_state Source state to add the transition to
 * @param condition Function that determines when transition should occur
 * @param next_state Target state for the transition
 * @return 0 on success, positive value on error (same as STATE_add_edge_with_execute)
 * 
 * @note This is a convenience function that calls STATE_add_edge_with_execute
 *       with a no-operation execution function.
 */
int8_t STATE_add_edge(t_state *tp_state,
                      uint8_t (*condition)(t_state *tp_state, void *vp_dto),
                      t_state *next_state);

/**
 * @brief Sets the parent state for hierarchical organization
 * 
 * Establishes a parent-child relationship between states, allowing the child
 * state to inherit behavior from the parent. Parent state conditions are
 * evaluated before child state conditions.
 * 
 * @param tp_state Child state to set parent for
 * @param tp_new_parent Parent state (can be NULL to remove parent)
 * 
 * @note Parent states enable hierarchical state machines where common
 *       behavior can be shared across multiple child states.
 */
void STATE_set_parent(t_state *tp_state, t_state *tp_new_parent);

/**
 * @brief Evaluates all transition conditions for a state and its parents
 * 
 * Checks all transition conditions for the current state and any parent states
 * in the hierarchy. If any condition is met, executes the transition and
 * updates the state machine's current state.
 * 
 * @param tp_state State to check transitions for
 * @param state_machine State machine to update if transition occurs
 * 
 * @note Parent state conditions are checked before child state conditions.
 *       The first matching condition triggers its transition and stops further
 *       evaluation. If a transition leads to a non-existent state, the state
 *       machine switches to its error state.
 */
void STATE_check_edges(t_state *tp_state, t_state_machine *state_machine);

/**
 * @brief Destroys a state and frees all associated memory
 * 
 * Recursively frees the state's edge array and the state structure itself.
 * Safe to call with NULL pointer.
 * 
 * @param tp_state Pointer to the state to destroy (can be NULL)
 * 
 * @note This function only frees the state structure and its edges array.
 *       The individual edge structures and target states are not freed.
 */
void STATE_destructor(t_state *tp_state);

/**
 * @brief Creates and initializes a new state
 * 
 * Allocates memory for a new state structure and initializes it with the
 * provided entry and update functions. Sets up the edges array and
 * entry timestamp.
 * 
 * @param on_entry Function to call when entering the state (required, not NULL)
 * @param on_update Function to call every update cycle (required, not NULL)
 * @return Pointer to newly created state on success, NULL on failure
 * 
 * @note Both function parameters are required and cannot be NULL. The returned
 *       state must be freed using STATE_destructor() to prevent memory leaks.
 *       The entry timestamp is allocated but not initialized until the state
 *       is first entered.
 */
t_state *STATE_constructor(void (*on_entry)(struct state *tp_state),
                           void (*on_update)(struct state *tp_state));

#endif // !STATE_H
