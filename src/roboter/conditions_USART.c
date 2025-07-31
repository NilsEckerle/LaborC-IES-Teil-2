#include "roboter/conditions_USART.h"
#include "roboter/roboter.h"
#include "tools/iesusart.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

#define BUILD_FUNCTION_condition_USART_chr(chr)                         \
  uint8_t condition_USART_##chr(t_state *inst __attribute__((unused)),  \
                                void *vp_dto __attribute__((unused))) { \
    if (!USART_has_string()) {                                          \
      return 0;                                                         \
    }                                                                   \
    const char *c = USART_get_string();                                 \
    if (NULL != c && (strcmp(c, #chr) == 0)) {                          \
      USART_consume_string();                                           \
      return 1;                                                         \
    }                                                                   \
    return 0;                                                           \
  }

BUILD_FUNCTION_condition_USART_chr(S);

BUILD_FUNCTION_condition_USART_chr(C);

BUILD_FUNCTION_condition_USART_chr(L);

BUILD_FUNCTION_condition_USART_chr(M);

BUILD_FUNCTION_condition_USART_chr(R);

BUILD_FUNCTION_condition_USART_chr(W);

BUILD_FUNCTION_condition_USART_chr(P);

uint8_t condition_USART_helper_clear_invalid_input(t_state *inst __attribute__((unused)),
                                                   void *vp_dto __attribute__((unused))) {
  TRACE("[condition_USART_clear] Called - clearing buffer\n");
  USART_consume_on_second_call_string();
  return 0;
}

uint8_t condition_USART_questionmark(t_state *inst __attribute__((unused)),
                                     void *vp_dto __attribute__((unused))) {
  TRACE("[condition_USART_s] Entry - checking for '?' or 'help'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_s] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (strcmp(c, "?") == 0 || strcmp(c, "help") == 0)) {
    TRACE("[condition_USART_s] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_s] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_isdigit(t_state *inst __attribute__((unused)),
                                void *vp_dto __attribute__((unused))) {
  TRACE("[condition_USART_c] Entry - checking for isdigit\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_isdigit] No complete string available\n");
    return 0;
  }

  const char *str = USART_get_string();
  if (str == NULL) {
    TRACE("[condition_USART_isdigit] No string available\n");
    return 0;
  }

  TRACE("[condition_USART_isdigit] got str: '%s' (length: %d)\n", str, (int)strlen(str));

  uint8_t is_digit = 1;
  size_t len = strlen(str);

  if (len == 0) {
    TRACE("[condition_USART_isdigit] Empty string\n");
    return 0;
  }

  for (uint8_t i = 0; i < len; i++) {
    TRACE("[condition_USART_isdigit] Checking char[%d]: '%c' (ASCII %d)\n", i, str[i], (int)str[i]);
    if (!isdigit(str[i])) {
      TRACE("[condition_USART_isdigit] Non-digit found at position %d\n", i);
      is_digit = 0;
      break;
    }
  }

  TRACE("[condition_USART_isdigit] str: %s", str);

  ROBOTER_set_dto(atoi(str));
  USART_consume_string();

  return is_digit;
}
