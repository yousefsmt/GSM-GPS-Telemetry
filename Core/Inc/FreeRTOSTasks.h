#ifndef FREERTOS_TASKS_H
#define FREERTOS_TASKS_H

#include "FreeRTOS.h"
#include "task.h"

#define tasksSTARTUP_STACK_SIZE           ( 128 )
#define tasksSTARTUP_STACK_PRIORITY       (configMAX_PRIORITIES - 1 )

#define tasksERROR_HANDLER_STACK_SIZE     ( 128 )
#define tasksERROR_HANDLER_STACK_PRIORITY ( configMAX_PRIORITIES - 2 )

#define tasksSENSE_MEASURE_STACK_SIZE     ( 128 )
#define tasksSENSE_MEASURE_STACK_PRIORITY ( configMAX_PRIORITIES - 3 )

#define tasksSENSE_LCD_STACK_SIZE         ( 128 )
#define tasksSENSE_LCD_STACK_PRIORITY     ( configMAX_PRIORITIES - 3 )

#endif /* FREERTOS_TASKS_H */