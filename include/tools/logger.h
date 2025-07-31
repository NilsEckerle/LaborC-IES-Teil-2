/**
 * @file 
 * @brief 
 * @author Nils Eckerle
 * @date 2025-07-30
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "tools/iesusart.h"
#include <avr/pgmspace.h>
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
#define LOG_LEVEL LOG_LEVEL_WARNING
#endif /* ifndef LOG_LEVEL */

// Helper function for formatted debug output with PROGMEM support
void __attribute__((unused)) debug_printf_P(const char *prefix, const char *format, ...);

// Helper function for formatted debug output
void __attribute__((unused)) debug_printf(const char *prefix, const char *format, ...);

#ifndef LOGGER_USE_PRINTF
#define DEBUG_PRINTF(prefix, format, ...) debug_printf_P(PSTR(prefix), PSTR(format), ##__VA_ARGS__)
#else
#define DEBUG_PRINTF(prefix, format, ...) printf(prefix format, ##__VA_ARGS__)
#endif

// TRACE: Very detailed execution flow
#if LOG_LEVEL <= LOG_LEVEL_TRACE
#define TRACE(format, ...) DEBUG_PRINTF("[TRACE] ", format, ##__VA_ARGS__)
#else
#define TRACE(format, ...) ((void)0)
#endif

// INFO_SPAM: General information whith high apperence volume
#if LOG_LEVEL <= LOG_LEVEL_INFO_SPAM
#define INFO_SPAM(format, ...) DEBUG_PRINTF("[SPAM] ", format, ##__VA_ARGS__)
#else
#define INFO_SPAM(format, ...) ((void)0)
#endif

// INFO: General information
#if LOG_LEVEL <= LOG_LEVEL_INFO
#define INFO(format, ...) DEBUG_PRINTF("[INFO] ", format, ##__VA_ARGS__)
#else
#define INFO(format, ...) ((void)0)
#endif

// WARNING: Something unusual but not critical
#if LOG_LEVEL <= LOG_LEVEL_WARNING
#define WARNING(format, ...) DEBUG_PRINTF("[WARN] ", format, ##__VA_ARGS__)
#else
#define WARNING(format, ...) ((void)0)
#endif

// ERROR: Something went wrong but program can continue
#if LOG_LEVEL <= LOG_LEVEL_ERROR
#define ERROR(format, ...) DEBUG_PRINTF("[ERROR] ", format, ##__VA_ARGS__)
#else
#define ERROR(format, ...) ((void)0)
#endif

// FATAL: Critical error, program should stop
#if LOG_LEVEL <= LOG_LEVEL_FATAL
#define FATAL(format, ...) DEBUG_PRINTF("[FATAL] ", format, ##__VA_ARGS__)
#else
#define FATAL(format, ...) ((void)0)
#endif

#define UI(format, ...) DEBUG_PRINTF("[UI] ", format, ##__VA_ARGS__)
#define BLANK(format, ...) DEBUG_PRINTF("", format, ##__VA_ARGS__)

#endif  // LOGGER_H
