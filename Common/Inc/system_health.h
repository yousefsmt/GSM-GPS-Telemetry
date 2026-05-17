#ifndef COMMON_SYSTEM_HEALTH_H_
#define COMMON_SYSTEM_HEALTH_H_

#include <stdbool.h>

#include "mcu.h"

#define INTERNALL_TEMP_LOW_THRESHOLD  ( -20 )
#define INTERNALL_TEMP_HIGH_THRESHOLD ( 80  )

extern volatile bool sensor_task_ok;

void system_health_task_start( void );

#endif /* COMMON_SYSTEM_HEALTH_H_ */