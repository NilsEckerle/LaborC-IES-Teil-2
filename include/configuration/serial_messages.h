/**
 * @file serial_messages.h
 * @brief Serial communication message definitions
 * @author Nils Eckerle
 * @date 2025-07-26
 */

#ifndef SERIAL_MESSAGES_H
#define SERIAL_MESSAGES_H

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
  "Send me an ’S’ so that I can start or "                                     \
  "send me a ’?’ so that I can give you help.\n\n"

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

#endif // !SERIAL_MESSAGES_H
