#ifndef PERIPHERAL_DMA_H_
#define PERIPHERAL_DMA_H_

#include "mcu.h"

/**
 * @brief  DMA1 Initialize ( Circular Mode )
 * @param  buffer       Start address aof memory for store peripheral data
 * @param  buffer_size  Size of buffer for CNDTR data register in DMA1 Channel3
 * @return none.
 * 
 * Implement DMA1 with circular mode, write NMEA messages received from UART3 ( NEO-6M )
 * and transfer data on ISR to memory buffer for next step (parser)
 */
void dma1_init( uint8_t *buffer, const uint32_t buffer_size);

#endif /* PERIPHERAL_DMA_H_ */