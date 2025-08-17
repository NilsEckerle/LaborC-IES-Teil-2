/**
 * @file logger.h
 * @brief Hierarchical logging system for embedded systems
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <avr/pgmspace.h>
#include <stdarg.h>

/**
 * @brief Log level definitions for message filtering
 */
#define LOG_LEVEL_TRACE 0     /**< Very detailed execution flow information */
#define LOG_LEVEL_INFO_SPAM 1 /**< High-frequency informational messages */
#define LOG_LEVEL_INFO 2      /**< General informational messages */
#define LOG_LEVEL_WARNING 3   /**< Warning messages for unusual conditions */
#define LOG_LEVEL_ERROR 4     /**< Error messages for recoverable problems */
#define LOG_LEVEL_FATAL 5     /**< Critical errors requiring program termination */
#define LOG_LEVEL_DISABLE 100 /**< Disables all logging output */

/**
 * @brief Default log level if not defined at compile time
 */
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_WARNING
#endif /* ifndef LOG_LEVEL */

/**
 * @brief Formatted debug output with PROGMEM support
 * @param prefix Log message prefix string stored in PROGMEM
 * @param format Printf-style format string stored in PROGMEM
 * @param ... Variable arguments for format string
 */
void __attribute__((unused)) debug_printf_P(const char *prefix, const char *format, ...);

/**
 * @brief Formatted debug output with regular strings
 * @param prefix Log message prefix string
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
void __attribute__((unused)) debug_printf(const char *prefix, const char *format, ...);

/**
 * @brief Internal macro for debug output routing
 */
#ifndef LOGGER_USE_PRINTF
#define DEBUG_PRINTF(prefix, format, ...) debug_printf_P(PSTR(prefix), PSTR(format), ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(prefix, format, ...) printf(prefix format, ##__VA_ARGS__)
#endif

/**
 * @brief Trace level logging for detailed execution flow
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_TRACE
#define TRACE(format, ...) DEBUG_PRINTF("[TRACE] ", format, ##__VA_ARGS__)
#else
#define TRACE(format, ...) ((void)0)
#endif

/**
 * @brief High-frequency informational logging
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_INFO_SPAM
#define INFO_SPAM(format, ...) DEBUG_PRINTF("[SPAM] ", format, ##__VA_ARGS__)
#else
#define INFO_SPAM(format, ...) ((void)0)
#endif

/**
 * @brief General informational logging
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define INFO(format, ...) DEBUG_PRINTF("[INFO] ", format, ##__VA_ARGS__)
#else
#define INFO(format, ...) ((void)0)
#endif

/**
 * @brief Warning level logging for unusual conditions
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define WARNING(format, ...) DEBUG_PRINTF("[WARN] ", format, ##__VA_ARGS__)
#else
#define WARNING(format, ...) ((void)0)
#endif

/**
 * @brief Error level logging for recoverable problems
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define ERROR(format, ...) DEBUG_PRINTF("[ERROR] ", format, ##__VA_ARGS__)
#else
#define ERROR(format, ...) ((void)0)
#endif

/**
 * @brief Fatal level logging for critical errors
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#if LOG_LEVEL <= LOG_LEVEL_FATAL
#define FATAL(format, ...) DEBUG_PRINTF("[FATAL] ", format, ##__VA_ARGS__)
#else
#define FATAL(format, ...) ((void)0)
#endif

/**
 * @brief User interface specific logging
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#define UI(format, ...) DEBUG_PRINTF("[UI] ", format, ##__VA_ARGS__)

/**
 * @brief Logging without prefix for custom formatting
 * @param format Printf-style format string
 * @param ... Variable arguments for format string
 */
#define BLANK(format, ...) DEBUG_PRINTF("", format, ##__VA_ARGS__)

#endif  // LOGGER_H
