#ifndef PERIPHERAL_UART_H_
#define PERIPHERAL_UART_H_

#include "mcu.h"

#ifdef DEBUG
	void uart2_init( const uint32_t baud_rate );
#endif /* DEBUG */

#endif /* PERIPHERAL_UART_H_ */