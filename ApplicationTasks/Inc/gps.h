#ifndef APPLICATION_TASKS_GPS_H_
#define APPLICATION_TASKS_GPS_H_

#include <stdint.h>

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define GPS_BUFFER_SIZE ( 128 )

typedef struct
{
	uint8_t  buffer[GPS_BUFFER_SIZE];
    uint32_t buffer_idx;
    uint32_t collecting;
}GpsBuffer_t;

void vGpsStartupTask( void );

#endif /* APPLICATION_TASKS_GPS_H_ */