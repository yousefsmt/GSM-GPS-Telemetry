#ifndef APPLICATION_TASKS_SYSTEM_HEALTH_H_
#define APPLICATION_TASKS_SYSTEM_HEALTH_H_

#include "FreeRTOSTasks.h"

#include "mcu.h"

#define QUEUE_SYSTEM_HEALTH_LENGTH    ( ( UBaseType_t )10 )
#define QUEUE_SYSTEM_HEALTH_ITEM_SIZE ( ( UBaseType_t )sizeof(SystemHealthStatus_t) )
typedef enum
{
	ERROR_TEMPERATURE_THRESHOLD_EXCEEDED,
	ERROR_MAX_NUMBER
}SystemHealthStatus_t;

void vSystemHealthStartupTask( void );

#endif /* APPLICATION_TASKS_SYSTEM_HEALTH_H_ */