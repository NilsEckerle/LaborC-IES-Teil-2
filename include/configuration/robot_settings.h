/**
 * @file robot_settings.h
 * @brief Central point for changing robot settings
 * @author Nils Eckerle
 * @date 2025-07-28
 */

#ifndef ROBOT_SETTINGS_H
#define ROBOT_SETTINGS_H

#include "tools/math_helper_functions.h"

// Roboter

/** @brief default rounds */
#define SETTING_DEFAULT_ROUNDS 3

// Line Follower

/** @brief Count of samples taken to calculate LF average */
#define SETTING_LF_AVG_SAMPLES 10

/** @brief Default threshold for the left line follower
 * @note range 0-1024
 */
#define SETTING_LF_LEFT_DEFAULT_THRESHOLD 512

/** @brief Default threshold for the middle line follower
 * @note range 0-1024
 */
#define SETTING_LF_MIDDLE_DEFAULT_THRESHOLD 512

/** @brief Default threshold for the right line follower
 * @note range 0-1024
 */
#define SETTING_LF_RIGHT_DEFAULT_THRESHOLD 512

// Driving

/** @brief Delay before start field is detected to prevent detection in corners
 */
#define SETTING_START_FIELD_DELAY_MS 170

// Engine

#define SETTING_ENGINE_PWM_POWER_MAX (255)

#define SETTING_ENGINE_PWM_POWER_FORWARD                                       \
  ((uint8_t)(SETTING_ENGINE_PWM_POWER_MAX * 7 / 8))

#define SETTING_ENGINE_PWM_POWER_BACKWARD                                      \
  ((uint8_t)(SETTING_ENGINE_PWM_POWER_MAX * 7 / 8))

#define SETTING_ENGINE_PWM_POWER_TURN_FORWARD                                  \
  ((uint8_t)(SETTING_ENGINE_PWM_POWER_MAX * 7 / 8))

#define SETTING_ENGINE_PWM_POWER_TURN_FORWARD_LOW                              \
  ((uint8_t)(SETTING_ENGINE_PWM_POWER_MAX * 1 / 16))

#define SETTING_ENGINE_PWM_POWER_TURN_BACKWARD                                 \
  ((uint8_t)(SETTING_ENGINE_PWM_POWER_MAX * 3 / 4))

// WAITING

#define WAITING_BLINK_PERIOD_MS CALCULATE_PERIOD_MS(5)

// PAUSE
#define PAUSE_SHIFT_PERIOD_MS CALCULATE_PERIOD_MS(10)

#endif // !ROBOT_SETTINGS_H
