/**
 * @file logger.h
 * @brief USART-based logging system with configurable log levels
 * 
 * Supports TRACE, INFO, WARNING, ERROR, FATAL levels with compile-time filtering.
 * Initialize USART before use: LOGGER_init_USART() or manual setup.
 * 
 * @example INFO("Value: %d", count); ERROR("Failed with code %d", err);
 */

#ifndef LOGGER_H
#define LOGGER_H

/********************
 * START LOG LOGIC
 ********************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "iesusart.h"

/** @brief Log levels for compile-time filtering */
#define LOG_LEVEL_TRACE 0      ///< Most verbose - execution flow
#define LOG_LEVEL_INFO 1       ///< General information
#define LOG_LEVEL_WARNING 2    ///< Non-critical warnings
#define LOG_LEVEL_ERROR 3      ///< Recoverable errors
#define LOG_LEVEL_FATAL 4      ///< Critical failures
#define LOG_LEVEL_DISABLE 100  ///< Disable all logging

#define LOG_LEVEL LOG_LEVEL_TRACE
// #define LOG_LEVEL LOG_LEVEL_DISABLE

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_ERROR
#endif /* ifndef LOG_LEVEL */

/** @brief Initialize USART for logging */
void LOGGER_init_USART();

static char LOG_buffer[256];  ///< Internal buffer for log messages

/**
 * @brief Internal formatted logging function
 * @param prefix Log level prefix (e.g., "[INFO] ")
 * @param format Printf-style format string
 * @param ... Format arguments
 */
static void LOG_printf(const char *prefix, const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  // Format the message with prefix
  snprintf(LOG_buffer, sizeof(LOG_buffer), "%s", prefix);
  vsnprintf(LOG_buffer + strlen(LOG_buffer),
            sizeof(LOG_buffer) - strlen(LOG_buffer), format, args);
  va_end(args);
  
  // Send to USART
  USART_print(LOG_buffer);
}

/**
 * @defgroup LogMacros Logging Macros
 * Use these macros for logging. Only compiled if LOG_LEVEL allows.
 * @{
 */

/** @brief Trace level - detailed execution flow */
#if LOG_LEVEL <= 0
#define TRACE(format, ...) LOG_printf("[TRACE] ", format, ##__VA_ARGS__)
#else
#define TRACE(format, ...) ((void)0)
#endif

/** @brief Info level - general information */
#if LOG_LEVEL <= 1
#define INFO(format, ...) LOG_printf("[INFO] ", format, ##__VA_ARGS__)
#else
#define INFO(format, ...) ((void)0)
#endif

/** @brief Warning level - unusual but non-critical situations */
#if LOG_LEVEL <= 2
#define WARNING(format, ...) LOG_printf("[WARN] ", format, ##__VA_ARGS__)
#else
#define WARNING(format, ...) ((void)0)
#endif

/** @brief Error level - recoverable errors */
#if LOG_LEVEL <= 3
#define ERROR(format, ...) LOG_printf("[ERROR] ", format, ##__VA_ARGS__)
#else
#define ERROR(format, ...) ((void)0)
#endif

/** @brief Fatal level - critical errors requiring program termination */
#if LOG_LEVEL <= 4
#define FATAL(format, ...) LOG_printf("[FATAL] ", format, ##__VA_ARGS__)
#else
#define FATAL(format, ...) ((void)0)
#endif

/** @} */ // End of LogMacros group

/********************
 * END LOG LOGIC
 ********************/

#endif // END LOGGER_H
