#include "gps.h"
#include "uart.h"
#include "dma.h"
#include "stream_buffer.h"

#define GPS_DMA_BUF_SIZE   256

static StreamBufferHandle_t xGpsStreamBuffer;
static TaskHandle_t         xGpsPackTaskHandle = NULL;

static void vGpsNmeaParser( void* pvParameters )
{
    ( void )pvParameters;
    uint8_t ucParsedMessage[STREAM_BUFFER_SIZE] = {0};
    size_t  xResult = 0;

    while ( 1 )
    {
        // Blocks until a full sentence or trigger level is met
        xResult = xStreamBufferReceive( xGpsStreamBuffer, ucParsedMessage, sizeof(ucParsedMessage), portMAX_DELAY );

        if ( xResult > 0)
        {
            /* * TODO: Parse NMEA sentences here (e.g., check for "$GPRMC" or "$GPGGA")
             * You can use strtok() or a lightweight NMEA library.
             */
            LOG( "%s\n", ucParsedMessage );
        }
    }
}

static void vGpsPackTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    uint32_t   ulCurrentPosition = 0;
    uint32_t   ulLastPosition = 0;
    
    // CRITICAL: Must be static so DMA doesn't corrupt the task stack
    static uint8_t ucDmaBuffer[GPS_DMA_BUF_SIZE]; 

    // Initialize hardware
    dma1_init( ucDmaBuffer, GPS_DMA_BUF_SIZE );
    uart3_init( USART3_BAUD_RATE );

    while ( 1 )
    {
        // Wait for ISR to signal that an IDLE line condition occurred
        xResult = xTaskNotifyWait( 0, 0, &ulCurrentPosition, portMAX_DELAY );
        
        if( xResult == pdPASS )
        {
            size_t xDataLength = 0;

            // Calculate length of newly arrived data
            if ( ulCurrentPosition > ulLastPosition )
            {
                xDataLength = ulCurrentPosition - ulLastPosition;
                // Send data directly to the stream buffer
                xStreamBufferSend( xGpsStreamBuffer, &ucDmaBuffer[ulLastPosition], xDataLength, 0 );
            }
            else if ( ulCurrentPosition < ulLastPosition )
            {
                // Handle Circular Buffer Wrap-around
                size_t xFirstPartLen = GPS_DMA_BUF_SIZE - ulLastPosition;
                xStreamBufferSend( xGpsStreamBuffer, &ucDmaBuffer[ulLastPosition], xFirstPartLen, 0 );
                xStreamBufferSend( xGpsStreamBuffer, ucDmaBuffer, ulCurrentPosition, 0 );
            }

            ulLastPosition = ulCurrentPosition;
        }
    }
}

void vGpsStartupTask( void )
{
    const size_t xStreamBufferSizeBytes = STREAM_BUFFER_SIZE;
    const size_t xTriggerLevel          = STREAM_BUFFER_TRIGGER_LEVEL; // Set to 1 to parse as soon as characters arrive
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
    // Check if Idle Line interrupt occurred
    if ( USART3->SR & USART_SR_IDLE )
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        // Clear IDLE flag by reading SR followed by DR
        volatile uint32_t ulTempStore;
        ulTempStore = USART3->SR;
        ulTempStore = USART3->DR;
        (void)ulTempStore;

        // Calculate exact index where DMA is currently writing
        uint32_t ulNewPosition = GPS_DMA_BUF_SIZE - DMA1_Channel3->CNDTR;

        if ( xGpsPackTaskHandle != NULL )
        {
            // Notify the packing task with the new buffer position
            xTaskNotifyFromISR( xGpsPackTaskHandle, ulNewPosition, eSetValueWithOverwrite, &xHigherPriorityTaskWoken );
        }

        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
}