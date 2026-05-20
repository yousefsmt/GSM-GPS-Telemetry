#include "gps.h"

#include "lwrb/lwrb.h"

static lwrb_t xRingBuffer;

static void vGpsBufferInit( GpsBuffer_t* xGpsBuffer )
{
	xGpsBuffer->buffer_idx = 0x00;
	xGpsBuffer->collecting = 0x00;

	memset( xGpsBuffer->buffer, 0, GPS_BUFFER_SIZE );
}

static void vGpsParseNmea( void* pvGpsBuffer )
{
	uint8_t data;
	uint8_t ucDataBuffer[128];
	GpsBuffer_t* xGpsBuffer = ( GpsBuffer_t* )pvGpsBuffer;

	lwrb_init( &xRingBuffer, ucDataBuffer, sizeof(ucDataBuffer) );

	while ( 1 )
	{
        if ( lwrb_get_full(&xRingBuffer) )
		{
            while ( lwrb_read(&xRingBuffer, &data, 1) == 1 )
			{
				if ( data == '$' )
				{
					xGpsBuffer->collecting = 0x01;
					xGpsBuffer->buffer_idx = 0x00;
				}
				if ( !xGpsBuffer->collecting )
				{
					continue;
				}

				if ( xGpsBuffer->buffer_idx >= (GPS_BUFFER_SIZE - 0x01) )
				{
					xGpsBuffer->collecting = 0x00;
					xGpsBuffer->buffer_idx = 0x00;
					continue;
				}

				xGpsBuffer->buffer[xGpsBuffer->buffer_idx++] = data;

				if (data == '\n')
				{
					xGpsBuffer->buffer[xGpsBuffer->buffer_idx] = '\0';

					LOG( "NMEA: %s", xGpsBuffer->buffer ); /* Add to praser */

					xGpsBuffer->collecting = 0x00;
					xGpsBuffer->buffer_idx = 0x00;
				}
            }
        }
    }

}

void vGpsStartupTask( void )
{
	BaseType_t  xCreationState;
	GpsBuffer_t xGpsBuffer;

	vGpsBufferInit( &xGpsBuffer );

	xCreationState = xTaskCreate( &vGpsParseNmea, "GPS Parser", 128, ( void* )&xGpsBuffer, 1, NULL );
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
		lwrb_write(&xRingBuffer, &data, 1);
	}

	if ( ( USART1->SR & USART_SR_IDLE ) ) { }
}