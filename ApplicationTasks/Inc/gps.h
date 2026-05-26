#ifndef APPLICATION_TASKS_GPS_H_
#define APPLICATION_TASKS_GPS_H_

#include <stdint.h>

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define USART3_BAUD_RATE            ( 115200U ) /*!< Receiver baud rate */
#define STREAM_BUFFER_SIZE          ( 85U ) /*!< ISR ring buffer size */
#define STREAM_BUFFER_TRIGGER_LEVEL ( 20U ) /*!< ISR ring buffer size */

/**
 * @brief  GPS Startup Task
 * @param  none.
 * @return none.
 * 
 * Initialize buffer and run other tasks
 */
void vGpsStartupTask( void );

#endif /* APPLICATION_TASKS_GPS_H_ */
