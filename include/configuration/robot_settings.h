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
#define SETTING_LF_LEFT_DEFAULT_THRESHOLD 200

/** @brief Default threshold for the middle line follower
 * @note range 0-1024
 */
#define SETTING_LF_MIDDLE_DEFAULT_THRESHOLD 800

/** @brief Default threshold for the right line follower
 * @note range 0-1024
 */
#define SETTING_LF_RIGHT_DEFAULT_THRESHOLD 200

// Driving

/** @brief Delay before start field is detected to prevent detection in corners */
#define SETTING_START_FIELD_DELAY_MS 170

/** @brief Delay before backward is detected to prevent deadlock in corners */
#define SETTINGBACKWAD_DELAY_TRESHOLD 0

// Engine


/** @brief Defines max PWM power 
 * @note rangeing 0 - 255
 */
#define SETTING_ENGINE_PWM_POWER_MAX (255)

/** @brief Defines forward power
 * @note Used for Speed while driving forward
 */
#define SETTING_ENGINE_PWM_POWER_FORWARD CALCULATE_PERCENT(SETTING_ENGINE_PWM_POWER_MAX, 90)

/** @brief Defines backward power
 * @note Used for Speed while driving backward
 */
#define SETTING_ENGINE_PWM_POWER_BACKWARD CALCULATE_PERCENT(SETTING_ENGINE_PWM_POWER_MAX, 60)

/** @brief Defines turn speed of the outside wheel */
#define SETTING_ENGINE_PWM_POWER_TURN_FORWARD CALCULATE_PERCENT(SETTING_ENGINE_PWM_POWER_MAX, 100)

/** @brief Defines turn speed of the inside wheel in soft turns */
#define SETTING_ENGINE_PWM_POWER_TURN_FORWARD_LOW \
  CALCULATE_PERCENT(SETTING_ENGINE_PWM_POWER_MAX, 0)

/** @brief Defines turn speed of the inside wheel in hard turns 
 * @note this wheel is ment to turn backward
 */
#define SETTING_ENGINE_PWM_POWER_TURN_BACKWARD CALCULATE_PERCENT(SETTING_ENGINE_PWM_POWER_MAX, 100)

// WAITING

/** @brief Blinking LEDS period
 * @note the period is the time between two ocouring events.
 * 1s/5 = 0.2s == 1000ms/5 = 200ms
 */
#define SETTING_WAITING_BLINK_PERIOD_MS CALCULATE_PERIOD_MS(5)

// PAUSE

/** @brief Pause running light period
 * @note the period is the time between two ocouring events.
 * 1s/10 = 0.1s == 1000ms/10 = 100ms
 */
#define SETTING_PAUSE_SHIFT_PERIOD_MS CALCULATE_PERIOD_MS(10)

#endif  // !ROBOT_SETTINGS_H
