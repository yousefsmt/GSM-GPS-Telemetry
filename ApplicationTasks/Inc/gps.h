#ifndef APPLICATION_TASKS_GPS_H_
#define APPLICATION_TASKS_GPS_H_

#include <stdint.h>

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define GPS_DMA_BUFFER_SIZE         ( 256U )    /*!< Size of memory used by DMA */
#define USART3_BAUD_RATE            ( 115200U ) /*!< Receiver baud rate */
#define NMEA_MAX_MESSAGE_LENGTH     ( 85U )     /*!< Maximum message length */

/**
 * @brief  GPS Startup Task
 * @param  none.
 * @return none.
 * 
 * Initialize buffer and run other tasks
 */
void vGpsStartupTask( void* pvParameters );

#endif /* APPLICATION_TASKS_GPS_H_ */
