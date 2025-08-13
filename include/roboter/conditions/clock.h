/**
 * @file conditions_clock.h
 * @brief Time-based condition functions for state machine transitions
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef CONDITIONS_CLOCK_H
#define CONDITIONS_CLOCK_H

#include "state_machine/state.h"

/**
 * @brief Checks if the configured start field delay has elapsed since state entry
 *
 * @param inst Pointer to the current state instance containing entry time information
 * @param vp_dto Pointer to data transfer object, not used by this condition
 * 
 * @return 1 if the start field delay time has elapsed (condition is true)
 * @return 0 if the start field delay time has not yet elapsed (condition is false)
 * 
 * @note This function relies on:
 *       - CLOCK_get_milliseconds() for current system time
 *       - inst->ui32p_state_entry_time_ms for state entry timestamp
 *       - SETTING_START_FIELD_DELAY_MS configuration constant
 * 
 * @see SETTING_START_FIELD_DELAY_MS in robot_settings.h for delay configuration
 * @see CLOCK_get_milliseconds() in hardware/clock.h for system time access
 */
uint8_t condition_start_field_delay(t_state *inst __attribute__((unused)),
                                    void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if 5 seconds have elapsed since state entry
 * 
 * @param inst Pointer to the current state instance containing entry time information
 * @param vp_dto Pointer to data transfer object, not used by this condition
 * 
 * @return 1 if more than 5 seconds have elapsed since state entry (condition is true)
 * @return 0 if 5 seconds or less have elapsed since state entry (condition is false)
 * 
 * @note This function relies on:
 *       - CLOCK_get_milliseconds() for current system time
 *       - inst->ui32p_state_entry_time_ms for state entry timestamp
 *       - Fixed 5000ms (5 second) timeout value
 * 
 * @see CLOCK_get_milliseconds() in hardware/clock.h for system time access
 * @see condition_start_field_delay() for configurable delay alternative
 */
uint8_t condition_5_seconds_after_entry(t_state *inst __attribute__((unused)),
                                        void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if 4 seconds have elapsed since state entry
 * 
 * @param inst Pointer to the current state instance containing entry time information
 * @param vp_dto Pointer to data transfer object, not used by this condition
 * 
 * @return 1 if more than 4 seconds have elapsed since state entry (condition is true)
 * @return 0 if 4 seconds or less have elapsed since state entry (condition is false)
 * 
 * @note This function relies on:
 *       - CLOCK_get_milliseconds() for current system time
 *       - inst->ui32p_state_entry_time_ms for state entry timestamp
 *       - Fixed 4000ms (4 second) timeout value
 * 
 * @see CLOCK_get_milliseconds() in hardware/clock.h for system time access
 * @see condition_start_field_delay() for configurable delay alternative
 */
uint8_t condition_CLOCK_4_seconds(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused)));

/**
 * @brief Checks if 1 seconds have elapsed since state entry
 * 
 * @param inst Pointer to the current state instance containing entry time information
 * @param vp_dto Pointer to data transfer object, not used by this condition
 * 
 * @return 1 if more than 1 seconds have elapsed since state entry (condition is true)
 * @return 0 if 1 seconds or less have elapsed since state entry (condition is false)
 * 
 * @note This function relies on:
 *       - CLOCK_get_milliseconds() for current system time
 *       - inst->ui32p_state_entry_time_ms for state entry timestamp
 *       - Fixed 1000ms (1 second) timeout value
 * 
 * @see CLOCK_get_milliseconds() in hardware/clock.h for system time access
 * @see condition_start_field_delay() for configurable delay alternative
 */
uint8_t condition_CLOCK_1_seconds(t_state *inst __attribute__((unused)),
                                  void *vp_dto __attribute__((unused)));
#endif  // CONDITIONS_CLOCK_H
