#ifndef LOGGER_H
#define LOGGER_H

#include "tools/iesusart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_INFO_SPAM 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARNING 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5
#define LOG_LEVEL_DISABLE 100

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif /* ifndef LOG_LEVEL */

// Helper function for formatted debug output
// Use __attribute__((unused)) to suppress unused function warnings
static void __attribute__((unused)) debug_printf(const char *prefix, const char *format, ...) {
	char debug_buffer[256];
  va_list args;
  va_start(args, format);

  // Format the message
  snprintf(debug_buffer, sizeof(debug_buffer), "%s", prefix);
  vsnprintf(debug_buffer + strlen(debug_buffer),
            sizeof(debug_buffer) - strlen(debug_buffer), format, args);

  va_end(args);

  // Send to USART
  USART_print(debug_buffer);
}

// TRACE: Very detailed execution flow
#if LOG_LEVEL <= LOG_LEVEL_TRACE
#define TRACE(format, ...) debug_printf("[TRACE] ", format, ##__VA_ARGS__)
#else
#define TRACE(format, ...) ((void)0)
#endif

// INFO_SPAM: General information whith high apperence volume
#if LOG_LEVEL <= LOG_LEVEL_INFO_SPAM
#define INFO_SPAM(format, ...) debug_printf("[SPAM] ", format, ##__VA_ARGS__)
#else
#define INFO_SPAM(format, ...) ((void)0)
#endif

// INFO: General information
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define INFO(format, ...) debug_printf("[INFO] ", format, ##__VA_ARGS__)
#else
#define INFO(format, ...) ((void)0)
#endif

// WARNING: Something unusual but not critical
#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define WARNING(format, ...) debug_printf("[WARN] ", format, ##__VA_ARGS__)
#else
#define WARNING(format, ...) ((void)0)
#endif

// ERROR: Something went wrong but program can continue
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define ERROR(format, ...) debug_printf("[ERROR] ", format, ##__VA_ARGS__)
#else
#define ERROR(format, ...) ((void)0)
#endif

// FATAL: Critical error, program should stop
#if LOG_LEVEL <= LOG_LEVEL_FATAL
#define FATAL(format, ...) debug_printf("[FATAL] ", format, ##__VA_ARGS__)
#else
#define FATAL(format, ...) ((void)0)
#endif

#endif // LOGGER_H
