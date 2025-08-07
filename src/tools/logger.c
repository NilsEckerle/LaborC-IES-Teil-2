#include "tools/logger.h"

static char debug_buffer[128];

// print from SRAM
void __attribute__((unused)) debug_printf(const char *prefix, const char *format, ...) {
  va_list args;
  va_start(args, format);

  // Format the message
  snprintf(debug_buffer, sizeof(debug_buffer), "%s", prefix);
  vsnprintf(debug_buffer + strlen(debug_buffer), sizeof(debug_buffer) - strlen(debug_buffer),
            format, args);

  va_end(args);

  // Send to USART
  USART_print(debug_buffer);
}

// print from PROGRAM MEMORY
void __attribute__((unused)) debug_printf_P(const char *prefix, const char *format, ...) {
  va_list args;
  va_start(args, format);

  strcpy_P(debug_buffer, prefix);

  // Format the message
  vsnprintf_P(debug_buffer + strlen(debug_buffer), sizeof(debug_buffer) - strlen(debug_buffer),
              format, args);

  va_end(args);

  // Send to USART
  USART_print(debug_buffer);
}
