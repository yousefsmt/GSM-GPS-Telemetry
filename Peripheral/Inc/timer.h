#ifndef PERIPHERAL_TIMER_H_
#define PERIPHERAL_TIMER_H_

#include "mcu.h"

/**
 * TODO: Implement TIM2 for chceck periodically system health
 * TODO: Implement TIM3 for high frequency interrupt statistics
 */

/**
 * Implement general TIM2 for system health monitor
 * check internal sensor periodically
 */
void tim2_init( void );
#endif /* PERIPHERAL_TIMER_H_ */