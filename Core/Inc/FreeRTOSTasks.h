#ifndef FREERTOS_TASKS_H
#define FREERTOS_TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define taskSTARTUP_STACK_SIZE             ( 128 )
#define taskSTARTUP_STACK_PRIORITY         (configMAX_PRIORITIES - 1 )

#define taskSYSTEM_HEALTH_STACK_SIZE       ( 128 )
#define taskSYSTEM_HEALTH_STACK_PRIORITY   ( configMAX_PRIORITIES - 1 )

#define taskGPS_PACK_STACK_SIZE     ( 128 )
#define taskGPS_PACK_STACK_PRIORITY (configMAX_PRIORITIES - 5 )

#define taskGPS_PARSER_STACK_SIZE          ( 128 )
#define taskGPS_PARSER_STACK_PRIORITY      ( configMAX_PRIORITIES - 1 )

#endif /* FREERTOS_TASKS_H */