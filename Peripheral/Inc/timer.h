#ifndef PERIPHERAL_TIMER_H_
#define PERIPHERAL_TIMER_H_

#include "mcu.h"

/**
 * @brief Initialize and configure TIM3
 * @param none
 * @return none
 *
 * Implement trigger event for measure temperature of internal sensor with ADC1.
 * When overflow send trigger then start measurement.
 */
void tim3_init( void );

#endif /* PERIPHERAL_TIMER_H_ */