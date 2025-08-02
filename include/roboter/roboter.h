/**
 * @file roboter.h
 * @brief Robot instance management and configuration structure
 * @author Nils Eckerle
 * @date 2025-08-02
 */

#ifndef ROBOTER_H
#define ROBOTER_H

#include "tools/logger.h"
#include <stdint.h>

/**
 * @brief Robot configuration and state structure
 *
 * Contains all configurable parameters and runtime state for the robot,
 * including line follower sensor thresholds, round counting, and data transfer.
 */
typedef struct roboter {
  uint16_t ui16_LF_left_threshold;   /**< Left line follower sensor threshold value */
  uint16_t ui16_LF_middle_threshold; /**< Middle line follower sensor threshold value */
  uint16_t ui16_LF_right_threshold;  /**< Right line follower sensor threshold value */
  uint8_t ui8_LF_ADC_avg_samples;    /**< Number of ADC samples for line follower averaging */

  int8_t i8_current_round; /**< Current round/lap counter */
  int8_t i8_max_rounds;    /**< Maximum rounds to complete */
  void *vp_dto;            /**< Generic data transfer object pointer */
} t_roboter;

/**
 * @brief Initializes robot structure with default configuration values
 * @param robi Pointer to robot structure to initialize
 * @note Sets thresholds from SETTING_LF_*_DEFAULT_THRESHOLD constants
 * @note Sets rounds from SETTING_DEFAULT_ROUNDS and resets current round to 0
 */
void ROBOTER_init(t_roboter *robi);

/**
 * @brief Gets singleton instance of robot structure
 * @return Pointer to global robot instance
 * @note Automatically initializes instance on first call
 * @note Thread-safe singleton pattern implementation
 */
t_roboter *ROBOTER_get_instance();

/**
 * @brief Macro to safely set data transfer object with automatic memory management
 * @param t_data Data to store in robot's DTO (any type)
 *
 * This macro:
 * - Frees existing DTO memory if present
 * - Allocates new memory based on data type
 * - Copies the data value to allocated memory
 * - Updates robot's vp_dto pointer
 *
 * @note Uses typeof() for type-safe memory allocation
 * @note Automatically handles memory cleanup of previous DTO
 *
 * @warning Requires stdlib.h for malloc/free functions
 * @see ROBOTER_get_instance() for accessing robot instance
 */
#define ROBOTER_set_dto(t_data)                       \
  do {                                                \
    t_roboter *tp_robi = ROBOTER_get_instance();      \
    if (tp_robi->vp_dto != NULL) {                    \
      free(tp_robi->vp_dto);                          \
      tp_robi->vp_dto = NULL;                         \
    }                                                 \
    tp_robi->vp_dto = malloc(sizeof(typeof(t_data))); \
    *(typeof(t_data) *)tp_robi->vp_dto = t_data;      \
  } while (0)

#endif  // !ROBOTER_H
