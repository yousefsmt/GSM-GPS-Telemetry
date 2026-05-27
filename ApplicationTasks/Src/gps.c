#include "gps.h"
#include "uart.h"
#include "dma.h"
#include "stream_buffer.h"

static StreamBufferHandle_t xGpsStreamBuffer;
static TaskHandle_t         xGpsPackTaskHandle = NULL;

static void vGpsNmeaParser( void* pvParameters )
{
    ( void )pvParameters;
    uint8_t ucParsedMessage[STREAM_BUFFER_SIZE] = {0};
    size_t  xResult = 0;

    while ( 1 )
    {
        xResult = xStreamBufferReceive( xGpsStreamBuffer, ucParsedMessage, sizeof(ucParsedMessage), portMAX_DELAY );

        if ( xResult > 0)
        {
            /* * TODO: Parse NMEA sentences here (e.g., check for "$GPRMC" or "$GPGGA")
             * You can use strtok() or a lightweight NMEA library.
             */
            LOG( "%s\n", ucParsedMessage );
            /**
             * TODO: After see directly NMEA message NEO-6M <---> PC, then write a good NMEA parser
             */
        }
    }
}

static void vGpsPackTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    uint32_t   ulCurrentPosition = 0;
    uint32_t   ulLastPosition = 0;
    
    static uint8_t ucDmaBuffer[GPS_DMA_BUFFER_SIZE]; 

    dma1_init( ucDmaBuffer, GPS_DMA_BUFFER_SIZE );
    uart3_init( USART3_BAUD_RATE );

    while ( 1 )
    {
        xResult = xTaskNotifyWait( 0, 0, &ulCurrentPosition, portMAX_DELAY );
        
        if( xResult == pdPASS )
        {
            size_t xDataLength = 0;

            if ( ulCurrentPosition > ulLastPosition )
            {
                xDataLength = ulCurrentPosition - ulLastPosition;
                ( void )xStreamBufferSend( xGpsStreamBuffer, &ucDmaBuffer[ulLastPosition], xDataLength, 0 );
            }
            else if ( ulCurrentPosition < ulLastPosition )
            {

                size_t xFirstPartLen = GPS_DMA_BUFFER_SIZE - ulLastPosition;
                ( void )xStreamBufferSend( xGpsStreamBuffer, &ucDmaBuffer[ulLastPosition], xFirstPartLen, 0 );
                ( void )xStreamBufferSend( xGpsStreamBuffer, ucDmaBuffer, ulCurrentPosition, 0 );
            }

            ulLastPosition = ulCurrentPosition;
        }
    }
}

void vGpsStartupTask( void )
{
    const size_t xStreamBufferSizeBytes = STREAM_BUFFER_SIZE;
    const size_t xTriggerLevel          = STREAM_BUFFER_TRIGGER_LEVEL;
    BaseType_t   xCreationState;

    xGpsStreamBuffer = xStreamBufferCreate( xStreamBufferSizeBytes, xTriggerLevel );
    configASSERT( xGpsStreamBuffer != NULL );

    xCreationState = xTaskCreate( &vGpsPackTask,
                                  "GPSpack",
                                  taskGPS_PACK_STACK_SIZE,
                                  NULL,
                                  taskGPS_PACK_STACK_PRIORITY,
                                  &xGpsPackTaskHandle );
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
 * ISR for NEO-6M IDLE Line detection
 */
void USART3_IRQHandler( void )
{
    if ( USART3->SR & USART_SR_IDLE )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        volatile uint32_t ulTempStore;
        ulTempStore = USART3->DR;
        (void)ulTempStore;

        uint32_t ulNewPosition = GPS_DMA_BUFFER_SIZE - DMA1_Channel3->CNDTR;

        if ( xGpsPackTaskHandle != NULL )
        {
            xTaskNotifyFromISR( xGpsPackTaskHandle, ulNewPosition, eSetValueWithOverwrite, &xHigherPriorityTaskWoken );
        }

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}