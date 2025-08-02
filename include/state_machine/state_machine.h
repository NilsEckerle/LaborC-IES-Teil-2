/**
 * @file state_machine.h
 * @brief Hierarchical finite state machine implementation
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state_machine/state.h"
#include "tools/dynamic_array.h"
#include <stdint.h>

/**
 * @brief Hierarchical finite state machine with error handling
 *
 * This structure implements a complete finite state machine that manages
 * multiple states, tracks the current active state and provides error state handling.
 * The state machine supports hierarchical states through the underlying state system.
 *
 * @note The state machine maintains a registry of all states and provides
 *       automatic error handling by switching to a designated error state
 *       when invalid transitions occur or states become unreachable.
 */
typedef struct state_machine {
  t_dyn_arr *arrp_states; /**< Dynamic array containing all registered states */

  t_state *tp_current_state; /**< Pointer to the currently active state */
  t_state *tp_error_state;   /**< Pointer to the designated error/fallback state */

  uint8_t bool_is_new_state; /**< Flag indicating if the current state was just entered */
} state_machine_t;

/**
 * @brief Registers a new state with the state machine
 *
 * Adds a state to the state machine's internal registry. The state must be
 * properly initialized before being added. Duplicate states (same memory address)
 * are detected and logged as warnings but still added to maintain compatibility.
 *
 * @param tp_state_machine State machine instance to add the state to
 * @param new_state State to register (must be non-NULL and properly initialized)
 * @return 0 on success, positive value on error:
 *         - 1: Invalid parameter (NULL state machine or state)
 *         - 3: Failed to add state to internal dynamic array
 *
 * @note States must be added to the state machine before they can be used
 *       as transition targets. The state machine does not take ownership
 *       of the state memory - states must be freed separately.
 */
int8_t STATE_MACHINE_add_state(state_machine_t *tp_state_machine, t_state *new_state);

/**
 * @brief Registers a state as the designated error state
 *
 * Sets up a special error state that the state machine will automatically
 * switch to when invalid transitions occur or when transitioning to non-existent
 * states. The error state is also added to the general state registry.
 *
 * @param tp_state_machine State machine instance to configure
 * @param new_state State to use as the error state (must be non-NULL)
 * @return 0 on success, positive value on error:
 *         - 1: Invalid parameter (NULL state machine or state)
 *         - 2: Failed to add error state to the state registry
 *
 * @note Only one error state can be active at a time. Setting a new error
 *       state will replace any previously configured error state. The error
 *       state should implement appropriate error handling or recovery logic.
 */
int8_t STATE_MACHINE_add_error_state(state_machine_t *tp_state_machine, t_state *new_state);

/**
 * @brief Sets the initial state for the state machine
 *
 * Configures which state the state machine should begin execution in.
 * This is typically called during state machine initialization before
 * starting the main execution loop.
 *
 * @param tp_state_machine State machine instance to configure
 * @param start_state State to begin execution in (must be registered)
 * @return 0 on success, 1 on error (invalid parameters)
 *
 * @note This function is equivalent to STATE_MACHINE_set_current_state().
 *       The start state should be registered with the state machine before
 *       calling this function, though this is not enforced.
 */
int8_t STATE_MACHINE_set_start_state(state_machine_t *tp_state_machine, t_state *start_state);

/**
 * @brief Transitions the state machine to a new state
 *
 * Immediately changes the current active state and marks it as a new state
 * to trigger the on_entry() function during the next update cycle. This
 * function can be called from within state logic or externally to force
 * state transitions.
 *
 * @param tp_state_machine State machine instance to update
 * @param new_state Target state to transition to (must be non-NULL)
 * @return 0 on success, 1 on error (invalid parameters)
 *
 * @note This function does not validate that the target state is registered
 *       with the state machine. The new state flag ensures that on_entry()
 *       is called exactly once when entering the new state.
 */
int8_t STATE_MACHINE_set_current_state(state_machine_t *tp_state_machine, t_state *new_state);

/**
 * @brief Executes one update cycle of the state machine
 *
 * Performs a complete state machine update including entry logic for new states,
 * parent state updates (if hierarchical), current state updates, and edge
 * condition evaluation for potential transitions. This is the core execution
 * function that should be called regularly.
 *
 * @param tp_state_machine State machine instance to update
 *
 * @note The update sequence is:
 *       1. Execute on_entry() for newly entered states
 *       2. Execute parent state on_update() if present
 *       3. Execute current state on_update()
 *       4. Check all transition conditions and execute transitions if triggered
 *
 * @warning This function assumes the current state is valid and non-NULL.
 *          Undefined behavior may occur if called on an uninitialized state machine.
 */
void STATE_MACHINE_update(state_machine_t *tp_state_machine);

/**
 * @brief Runs the state machine in an infinite loop
 *
 * Continuously executes STATE_MACHINE_update() in an infinite loop, creating
 * a blocking state machine execution. This is suitable for embedded systems
 * or applications where the state machine is the primary control flow.
 *
 * @param inst State machine instance to run
 *
 * @warning This function never returns and will block the calling thread
 *          indefinitely. Ensure proper initialization before calling.
 *          Use STATE_MACHINE_update() directly for non-blocking execution.
 */
void STATE_MACHINE_run(state_machine_t *inst);

/**
 * @brief Destroys a state machine and frees all associated memory
 *
 * Safely destroys the state machine by freeing the internal state registry
 * and the state machine structure itself. This function is safe to call
 * with NULL pointers.
 *
 * @param inst State machine instance to destroy (can be NULL)
 *
 * @note This function only frees the state machine structure and its internal
 *       dynamic array. Individual states must be freed separately using
 *       STATE_destructor(). The function handles partial initialization
 *       gracefully by checking for NULL pointers before freeing.
 */
void STATE_MACHINE_destructor(state_machine_t *inst);

/**
 * @brief Creates and initializes a new state machine instance
 *
 * Allocates memory for a new state machine and initializes its internal
 * dynamic array for state storage. The returned state machine is ready
 * for state registration and configuration.
 *
 * @return Pointer to newly created state machine on success, NULL on failure
 *
 * @note Memory allocation failures at any point will result in NULL return.
 *       The returned state machine must be destroyed using STATE_MACHINE_destructor()
 *       to prevent memory leaks. The state machine is initialized with:
 *       - Empty state registry
 *       - No current state (NULL)
 *       - No error state (NULL)
 *       - New state flag cleared
 */
state_machine_t *STATE_MACHINE_constructor();

#endif  // !STATE_MACHINE_H
