#ifndef PERIPHERAL_UART_H_
#define PERIPHERAL_UART_H_

#include "mcu.h"

#ifdef DEBUG
	void uart2_init( const uint32_t baud_rate );
#endif /* DEBUG */

void uart1_init( const uint32_t baud_rate );

void uart3_init( const uint32_t baud_rate );

#endif /* PERIPHERAL_UART_H_ */