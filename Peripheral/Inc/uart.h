#ifndef PERIPHERAL_UART_H_
#define PERIPHERAL_UART_H_

#include "mcu.h"

#ifdef DEBUG
/**
 * @brief Initialize and configure UART2
 * @param baud_rate 
 * @return none
 *
 * If enable DEBUG macro implement for print ( logging method ) and send string to PC.
 */
void uart2_init( const uint32_t baud_rate );
#endif /* DEBUG */

/**
 * @brief Initialize and configure UART1
 * @param baud_rate
 * @return none
 *
 * Implement UART1 ( AHB Bus!! ) for communicate with SIM800L send and receive AT-Command 
 */
void uart1_init( const uint32_t baud_rate );

/**
 * @brief Initialize and configure UART3
 * @param baud_rate
 * @return none
 *
 * Implement UART3 for receive NMEA messages from NEO-6M
 */
void uart3_init( const uint32_t baud_rate );

#endif /* PERIPHERAL_UART_H_ */