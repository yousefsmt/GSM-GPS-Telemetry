#ifndef APPLICATION_TASKS_SIM800L_H_
#define APPLICATION_TASKS_SIM800L_H_

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define USART1_BAUD_RATE            ( 115200U ) /*!< Receiver baud rate */
#define QUEUE_LENGTH ( 10 )

/**
 * @brief  SIM800L Startup Task
 * @param  none.
 * @return none.
 * 
 * Initialize buffer and run other tasks
 */
void vSimStartupTask( void* pvParameters );

#endif /* APPLICATION_TASKS_SIM800L_H_ */
