/**
 * @file serial_messages.h
 * @brief Serial communication message definitions
 * @author Nils Eckerle
 * @date 2025-07-26
 */

#ifndef SERIAL_MESSAGES_H
#define SERIAL_MESSAGES_H

#include "tools/logger.h"
#include "tools/math_helper_functions.h"

/**
 * @brief String literal with Start and help ui
 * @note This Message is a requirement by the product owner
 *
 * Usage requirement:
 * - in waiting for start command state
 * - Print in 1 Hz frequency
 */
#define MSG_WAITING_UI                     \
  "Hey you. I am in state WAITING. "       \
  "Send me an 'S' so that I can start or " \
  "send me a '?' so that I can give you help.\n\n"

#define WAITING_MSG_PERIOD_MS CALCULATE_PERIOD_MS(1)

/**
 * @brief String literal to place the robi on the start field
 * @note This Message is a requirement by the product owner
 *
 * Usage requirement:
 * - in searching for start field state
 * - Print in 1 Hz frequency
 */
#define MSG_SEARCHING                                    \
  "Not on the starting field, I am in state SEARCHING. " \
  "Place me there please...\n\n"

/** @brief Searching message period
 * @note the period is the time between two ocouring events.
 * 1s/1 = 1s == 1000ms/1 = 1000ms
 */
#define SEARCHING_MSG_PERIOD_MS CALCULATE_PERIOD_MS(1)

/**
 * @brief String format literal for driving state with round counter
 * @note Use this as the format in printf like functions.
 *       This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed in driving state
 * - Printed at 1 Hz frequency
 * - Round counting starts from 1
 * - %u placeholder should be replaced with current round number
 */
#define MSG_FMT_DRIVING_ROUND "Round and round I go during state DRIVING, currently round %u\n\n"

/** @brief Driving message period
 * @note the period is the time between two ocouring events.
 * 1s/1 = 1s == 1000ms/1 = 1000ms
 */
#define DRIVING_MSG_PERIOD_MS CALCULATE_PERIOD_MS(1)

/**
 * @brief String literal for fresh start message
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed when starting to drive (only once after 'S' command)
 */
#define MSG_FRESH_START "A fresh clean start! :-)\n\n"

/**
 * @brief String literal after completing round 1
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed after finishing round 1
 * - Printed once
 */
#define MSG_ROUND_1_COMPLETE "YEAH, still in state DRIVING, done round 1, going for round 2/3\n\n"

/**
 * @brief String literal after completing round 2
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed after finishing round 2
 * - Printed once
 */
#define MSG_ROUND_2_COMPLETE \
  "YEAH YEAH, still in state DRIVING, done round 2, going for round 3/3\n\n"

/**
 * @brief String literal after completing all rounds
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed after finishing round 3
 */
#define MSG_ROUND_3_COMPLETE                                       \
  "Done. Thank you! I am going into state RESTING now and I will " \
  "reset myself in 5 seconds. Take care of you and me!\n\n"

/**
 * @brief String literal for entering resting state
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed once when entering resting state
 */
#define MSG_RESTING "I will be in state RESTING from now on! zzzZZZzzzZZZzzz\n\n"

/**
 * @brief String literal for pausing state
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed in pausing state
 * - Printed at 1 Hz frequency
 */
#define MSG_PAUSING                                                    \
  "I am in state PAUSING .... zzzZZZzzzZZZzzz .... wake me up with P " \
  "again.\n\n"

/** @brief Pause message period
 * @note the period is the time between two ocouring events.
 * 1s/1 = 1s == 1000ms/1 = 1000ms
 */
#define PAUSE_MSG_PERIOD_MS CALCULATE_PERIOD_MS(1)

/** @brief Prints the Waiting help
 * @note this had to split up into multiple strings and prints,
 * so that the print buffer could be smaller, to save memory.
 */
#define PRINT_WAITING_HELP()                                               \
  do {                                                                     \
    UI("========== ROBOTER HELP ==========\n");                            \
    UI("FROM HERE:\n");                                                    \
    UI("'S'       - to start driving\n");                                  \
    UI("'ST'      - to start driving and timing each round\n");            \
    UI("'?'       - to get this help\n");                                  \
    UI("'C'       - to get into config\n");                                \
    UI("CONFIG:\n");                                                       \
    UI("'W'       - to get back to waiting\n");                            \
    UI("'R'       - to configure rounds to drive\n");                      \
    UI("'L'       - to configure line follower sensor tresholds\n");       \
    UI("'P'       - to configure engine power scaling (only slows engine " \
       "down)\n");                                                         \
    UI("DRIVING:\n");                                                      \
    UI("'P'       - toggle driving pause\n");                              \
    UI("==================================\n\n");                          \
  } while (0)

/** @brief Prints the config help
 * @note this had to split up into multiple strings and prints,
 * so that the print buffer could be smaller, to save memory.
 */
#define PRINT_CONFIG_HELP()                               \
  do {                                                    \
    UI("Send char to select:\n");                         \
    UI("'W' - got back to waiting state\n");              \
    UI("'R' - got to round config\n");                    \
    UI("'L' - go to line follower threshold config\n\n"); \
  } while (0)

/** @brief Prints the line follower config help
 * @note this had to split up into multiple strings and prints,
 * so that the print buffer could be smaller, to save memory.
 */
#define PRINT_CONFIG_LINE_FOLLOWER_THRESHOLDS()    \
  do {                                             \
    UI("COMMANDS:\n");                             \
    UI("'W' - got back to waiting state\n");       \
    UI("'C' - confirm and go back to config\n");   \
    UI("'L' - set threshold of left sensor\n");    \
    UI("'M' - set threshold of middle sensor\n");  \
    UI("'R' - set threshold of right sensor\n\n"); \
  } while (0)

/** 
 * @brief Format string to print newly configured round count
 * @note Use with printf-like functions to display the new round configuration
 * @param %u Unsigned integer representing the new number of rounds
 */
#define MSG_FMT_ROUNDS_SET_TO "Rounds set to %u laps.\n\n"

/**
 * @brief Prompt message asking user to enter number of rounds
 * @note Displayed when configuring the number of rounds to drive
 */
#define MSG_ROUNDS_ASK_NUMBER "How many rounds to drive? (Enter a number.)\n\n"

/**
 * @brief Prompt message asking user to enter line follower sensor threshold value
 * @note Displayed when configuring line follower sensor thresholds.
 *       Valid range is 0-1023 (10-bit ADC resolution)
 */
#define MSG_ENTER_VALUE_LF_TRESHOLD "Enter number as line sensor treshold. (0-1023)\n\n"

/**
 * @brief Format string to confirm newly set line follower threshold
 * @note Use with printf-like functions to display the new threshold value
 * @param %u Unsigned integer representing the new threshold value (0-1023)
 */
#define MSG_FMT_LF_TRESHOLD_SET_TO "Line sensor treshold set to %u of max 1023.\n\n"

#define MSG_FMT_ROUND_TIMING "Round %d took %u.%u seconds.\n\n"

#endif  // !SERIAL_MESSAGES_H
