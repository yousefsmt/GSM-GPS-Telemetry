#ifndef PERIPHERAL_FLASH_H_
#define PERIPHERAL_FLASH_H_

#include "mcu.h"

/**
 * @brief Initialize and configure prefetch buffer
 * @param none
 * @return none
 *
 * This function sets up the prefetch buffer and latency between CPU and Flash memory, with wait state setting.
 */
void flash_set_latency( uint32_t wate_state );

#endif /* PERIPHERAL_FLASH_H_ */