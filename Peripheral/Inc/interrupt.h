#ifndef PERIPHERAL_INTERRUPT_H_
#define PERIPHERAL_INTERRUPT_H_

#include "mcu.h"

/**
 * @brief  Set priority for all interrupt handler
 * @param  none 
 * @return none.
 * 
 * This function setup NVIC vector for best interrupt handler
 */
void interrupt_set_priorites( void );

#endif /* PERIPHERAL_INTERRUPT_H_ */