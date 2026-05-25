#ifndef APPLICATION_TASKS_GPS_H_
#define APPLICATION_TASKS_GPS_H_

#include <stdint.h>

#include "FreeRTOSTasks.h"
#include "mcu.h"

#define ISR_RING_BUFFER_SIZE ( 0x80 ) /*!< ISR ring buffer size */
#define TAIL_BUFFER_SIZE     ( 0x06 ) /*!< Head NMEA message ( $GPGGA ) */
#define ASCII_TABLE_SIZE     ( 0x10 ) /*!< ASCII to integer table length */

#define NMEA_MESSAGE_LENGTH  ( 85 )   /*!< Maximum NMEA messages length */
#define QUEUE_NMEA_LENGTH    ( 10 )   /*!< Queue length for store NMEA */
#define TAIL_CHECKSUM        ( 0x56 ) /*!< Head checksum */

#define MSB_MASK ( 0xf0 ) /*!< LSB value */
#define LSB_MASK ( 0x0f ) /*!< MSB value */

/**
 * @brief  GPS Startup Task
 * @param  none.
 * @return none.
 * 
 * Initialize buffer and run other tasks
 */
void vGpsStartupTask( void );

#endif /* APPLICATION_TASKS_GPS_H_ */
