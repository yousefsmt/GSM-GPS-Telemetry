#ifndef PERIPHERAL_GPIO_H_
#define PERIPHERAL_GPIO_H_

#include "mcu.h"

/**
 * @brief Initialize and configure GPIO
 * @param none
 * @return none
 *
 * Configuration and initialization GPIO. This enable PC13 for blinking and enable GPIO clocks
 */
void gpio_init( void );


#endif /* PERIPHERAL_GPIO_H_ */