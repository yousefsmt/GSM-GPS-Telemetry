#include "gps.h"

#include "queue.h"

static QueueHandle_t xGgaMessage;

static void vGpsNmeaParser( void* pvParameters )
{
	( void )pvParameters;

	uint8_t ucBuffer[NMEA_MESSAGE_LENGTH];

	while ( 1 )
	{
		if (xQueueReceive(xGgaMessage, ucBuffer, portMAX_DELAY) == pdTRUE)
		{
			LOG( "%s\r\n", ucBuffer );
		}
	}
}

static void vGpsRemoveTail( void* pvGpsBuffer )
{
	( void )pvGpsBuffer;

	uint8_t  ucBuffer[NMEA_MESSAGE_LENGTH];
	uint8_t  ucTempChar      = 0x00U;
	uint8_t  ucTailIndex     = 0x00U;
	uint8_t  ucChecksumMask  = 0x00U;
	uint8_t  ucChecksumIndex = 0x00U;
	uint8_t  ucBufferIndex   = 0x00U;

	uint32_t ulChecksumTemp  = TAIL_CHECKSUM;

	static const uint8_t ASCIItable[ASCII_TABLE_SIZE]   = { 0x30U, 0x31U, 0x32U, 0x33U, 0x34U, 0x35U, 0x36U, 0x37U, 0x38U, 0x39U, 0x41U, 0x42U, 0x43U, 0x44U, 0x45U, 0x46U };
	       const char    ucTailBuffer[TAIL_BUFFER_SIZE] = "$GPGGA";

	// ( void )lwrb_init( &xRingBuffer, ucDataBuffer, sizeof( ucDataBuffer ) );

	/**
	 * TODO: This super loop must be optimize in future
	 * NOTE: In bare-metal version is good, but add rtos performance dumped. Shit!!!
	 */
	while ( 0x01 )
	{
        if ( 1 )
		{
            while ( 1 )
			{
				if ( ucTailIndex == 0x06U )
				{
					if ( ucTempChar == '*' )
					{
						ucChecksumIndex = 0x00U;
						ucTailIndex     = 0x00U;
						ucChecksumMask  = ulChecksumTemp;
						ulChecksumTemp  = TAIL_CHECKSUM;
						continue;
					}
					ucBuffer[ucBufferIndex] = ucTempChar;
					ucBufferIndex = ( ucBufferIndex + 0x01U ) % NMEA_MESSAGE_LENGTH;
					if ( ucTempChar == '\r' || ucTempChar == '\n' ) {ulChecksumTemp = TAIL_CHECKSUM; ucChecksumIndex = 0x00U; ucTailIndex = 0x00U;continue;}
					ulChecksumTemp ^= ucTempChar;
					continue;
				}
				if ( ucTailBuffer[ucTailIndex] == ucTempChar && ucTailIndex < 0x07U ){ucTailIndex++;ucBufferIndex = 0x00;continue;}
				else{ ucTailIndex = 0x00U;}
				switch (ucChecksumIndex)
				{
				case 0x00U:
					ucChecksumIndex = ( ucTempChar == ASCIItable[( ucChecksumMask & MSB_MASK ) >> 0x04U ] ) ? 0x01U : 0x00U;
					break;
				case 0x01U:
					if ( ucTempChar == ASCIItable[( ucChecksumMask & LSB_MASK )] ){xQueueSend( xGgaMessage, ucBuffer, 0 );ucBufferIndex = 0x00;}
					ucChecksumIndex++;
					break;
				default:
					break;
				}
            }
        }
    }
}

void vGpsStartupTask( void )
{
	BaseType_t  xCreationState;

	xGgaMessage = xQueueCreate( QUEUE_NMEA_LENGTH, ( sizeof( uint8_t ) * NMEA_MESSAGE_LENGTH ) );
	configASSERT( xGgaMessage != NULL );

	xCreationState = xTaskCreate( &vGpsRemoveTail,
								  "GPSremoveTail",
								  taskGPS_REMOVE_TAIL_STACK_SIZE,
								  NULL,
								  taskGPS_REMOVE_TAIL_STACK_PRIORITY,
								  NULL );
	configASSERT( xCreationState == pdPASS );

	xCreationState = xTaskCreate( &vGpsNmeaParser,
		                          "GPSparser",
								  taskGPS_PARSER_STACK_SIZE,
								  NULL,
								  taskGPS_PARSER_STACK_PRIORITY,
								  NULL );
	configASSERT( xCreationState == pdPASS );

	vTaskDelete( NULL );
}

/**
 * Set ISR for NEO-6M receive message
 */
void USART1_IRQHandler( void )
{
    if ( USART1->SR & USART_SR_RXNE )
    {
		uint8_t data = USART1->DR;
		( void )data;
		// ( void )lwrb_write(&xRingBuffer, &data, 1);
	}

	if ( ( USART1->SR & USART_SR_IDLE ) ) { }
}