#ifndef APPLICATION_TASKS_GPS_H_
#define APPLICATION_TASKS_GPS_H_

#include <stdint.h>

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define ISR_RING_BUFFER_SIZE ( 128 )
#define TAIL_BUFFER_SIZE     ( 6 )
#define ASCII_TABLE_SIZE     ( 16 )

#define NMEA_MESSAGE_LENGTH  ( 85 )
#define QUEUE_NMEA_LENGTH    ( 10 )
#define TAIL_CHECKSUM        ( 0x56 )

#define MSB_MASK ( 0xf0 )
#define LSB_MASK ( 0x0f )

void vGpsStartupTask( void );

#endif /* APPLICATION_TASKS_GPS_H_ */
