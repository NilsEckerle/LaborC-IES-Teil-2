#include "roboter/conditions_USART.h"
#include "roboter/roboter.h"
#include "tools/iesusart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// #define LOG_LEVEL LOG_LEVEL_TRACE
#include "tools/logger.h"

uint8_t condition_USART_helper_clear_invalid_input(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_clear] Called - clearing buffer\n");
	USART_consume_on_second_call_string();
  return 0;
}

uint8_t condition_USART_r(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_r] Entry - checking for 'r' or 'R'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_r] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (strcmp(c, "r") == 0 || strcmp(c, "R") == 0)) {
    TRACE("[condition_USART_r] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_r] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_lfconfigstatic(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_lf-config-static] Entry - checking for 'lfs'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_lf-config-static] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (strcmp(c, "lfs") == 0 )) {
    TRACE("[condition_USART_lf-config-static] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_lf-config-static] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_lnum(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_lnum] Entry - checking for 'lnum'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_lnum] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (0)) { // TODO: check start with "l " and rest is num
    TRACE("[condition_USART_lnum] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_lnum] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_mnum(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_lnum] Entry - checking for 'mnum'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_lnum] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (0)) { // TODO: check start with "m " and rest is num
    TRACE("[condition_USART_lnum] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_lnum] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_rnum(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_rnum] Entry - checking for 'rnum'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_rnum] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (0)) { // TODO: check start with "r " and rest is num
    TRACE("[condition_USART_rnum] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_rnum] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_s(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_s] Entry - checking for 's' or 'S'\n");

  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_s] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (strcmp(c, "s") == 0 || strcmp(c, "S") == 0)) {
    TRACE("[condition_USART_s] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_s] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_c(t_state *inst __attribute__((unused)), void *vp_dto) {
  TRACE("[condition_USART_c] Entry - checking for 'c' or 'C'\n");
	
  // Only check if we actually have a complete string
  if (!USART_has_string()) {
    TRACE("[condition_USART_c] No complete string available\n");
    return 0;
  }

  const char *c = USART_get_string();
  if (NULL != c && (strcmp(c, "c") == 0 || strcmp(c, "C") == 0)) {
    TRACE("[condition_USART_c] Match found! String='%s' - consuming\n", c);
    USART_consume_string();
    return 1;
  }
  TRACE("[condition_USART_c] No match. String='%s'\n", c ? c : "NULL");
  return 0;
}

uint8_t condition_USART_isdigit(t_state *inst __attribute__((unused)), void *vp_dto) {
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
  
  if (is_digit) {
    int value = atoi(str);
    UI("rounds set to: %d\n", value);
    ROBOTER_get_instance()->i8_rounds = value;
    INFO("[condition_USART_isdigit] Valid number found! Consuming string.\n");
    USART_consume_string();
  } else {
    TRACE("[condition_USART_isdigit] Not a valid number\n");
  }
  
  return is_digit;
}
