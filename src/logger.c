#include "logger.h"

void LOGGER_init_USART() {
	USART_init(UBRR_SETTING);
	return;
}
