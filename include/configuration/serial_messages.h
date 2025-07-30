/**
 * @file serial_messages.h
 * @brief Serial communication message definitions
 * @author Nils Eckerle
 * @date 2025-07-26
 */

#ifndef SERIAL_MESSAGES_H
#define SERIAL_MESSAGES_H

#include "tools/math_helper_functions.h"

/**
 * @brief String literal with Start and help ui
 * @note This Message is a requirement by the product owner
 *
 * Usage requirement:
 * - in waiting for start command state
 * - Print in 1 Hz frequency
 */
#define MSG_WAITING_UI                                                         \
  "Hey you. I am in state WAITING. "                                           \
  "Send me an 'S' so that I can start or "                                     \
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
#define MSG_SEARCHING                                                          \
  "Not on the starting field, I am in state SEARCHING. "                       \
  "Place me there please...\n\n"

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
#define MSG_FMT_DRIVING_ROUND                                                  \
  "Round and round I go during state DRIVING, currently round %u\n\n"

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
#define MSG_ROUND_1_COMPLETE                                                   \
  "YEAH, still in state DRIVING, done round 1, going for round 2/3\n\n"

/**
 * @brief String literal after completing round 2
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed after finishing round 2
 * - Printed once
 */
#define MSG_ROUND_2_COMPLETE                                                   \
  "YEAH YEAH, still in state DRIVING, done round 2, going for round 3/3\n\n"

/**
 * @brief String literal after completing all rounds
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed after finishing round 3
 */
#define MSG_ROUND_3_COMPLETE                                                   \
  "Done. Thank you! I am going into state RESTING now and I will "             \
  "reset myself in 5 seconds. Take care of you and me!\n\n"

/**
 * @brief String literal for entering resting state
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed once when entering resting state
 */
#define MSG_RESTING                                                            \
  "I will be in state RESTING from now on! zzzZZZzzzZZZzzz\n\n"

/**
 * @brief String literal for pausing state
 * @note This message is a requirement by the product owner
 *
 * Usage requirement:
 * - Displayed in pausing state
 * - Printed at 1 Hz frequency
 */
#define MSG_PAUSING                                                            \
  "I am in state PAUSING .... zzzZZZzzzZZZzzz .... wake me up with P "         \
  "again.\n\n"

#define PAUSE_MSG_PERIOD_MS CALCULATE_PERIOD_MS(1)

#define MSG_WAITING_HELP                                                       \
  "========== ROBOTER HELP ==========\n"                                       \
  "FROM HERE:\n"                                                               \
  "'S'       - to start driving\n"                                             \
  "'?'       - to get this help\n"                                             \
  "'C'       - to get into config\n"                                           \
  "CONFIG:\n"                                                                  \
  "'S'       - to get back to waiting\n"                                       \
  "'R'       - to configure rounds to drive\n"                                 \
  "'L'       - to configure line follower sensor tresholds\n"                  \
  "'P'       - to configure engine power scaling (only slows engine down)\n"   \
  "DRIVING\n"                                                                  \
  "'P'       - toggle driving pause\n"                                         \
  "==================================\n\n"

#endif // !SERIAL_MESSAGES_H
