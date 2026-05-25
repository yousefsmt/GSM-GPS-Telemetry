#ifndef PERIPHERAL_FLASH_H_
#define PERIPHERAL_FLASH_H_

#include "mcu.h"

/**
 * @brief Initialize and configure prefetch buffer
 * @param wate_state flash access time
 * @return none
 *
 * This function sets up the prefetch buffer and latency between CPU and Flash memory, with wait state setting.
 * NOTE: please if enabled "HSE_CLOCK_SPEED" for 72MHz clock speed you must check input this function equal 2 wait states
 * 
 * * 000 Zero wait state, if 0 < SYSCLK≤ 24 MHz
 * * 001 One wait state, if 24 MHz < SYSCLK ≤ 48 MHz
 * * 010 Two wait states, if 48 MHz < SYSCLK ≤72 MHz
 */
void flash_set_latency( uint32_t wate_state );

#endif /* PERIPHERAL_FLASH_H_ */