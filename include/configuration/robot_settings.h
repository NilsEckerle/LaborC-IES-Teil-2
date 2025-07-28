/**
 * @file robot_settings.h
 * @brief Central point for changing robot settings
 * @author Nils Eckerle
 * @date 2025-07-28
 */

#ifndef ROBOT_SETTINGS_H
#define ROBOT_SETTINGS_H

// Roboter

/** default rounds */
#define SETTING_DEFAULT_ROUNDS 3

// Line Follower

/** Count of samples taken to calculate LF average */
#define SETTING_LF_AVG_SAMPLES 10

/** Default threshold for the left line follower
 * @note range 0-1024
 */
#define SETTING_LF_LEFT_DEFAULT_THRESHOLD 512

/** Default threshold for the middle line follower
 * @note range 0-1024
 */
#define SETTING_LF_MIDDLE_DEFAULT_THRESHOLD 512

/** Default threshold for the right line follower
 * @note range 0-1024
 */
#define SETTING_LF_RIGHT_DEFAULT_THRESHOLD 512

// Driving

#define SETTING_START_FIELD_DELAY_MS 170

#endif // !ROBOT_SETTINGS_H
