#include "gps.h"
#include "uart.h"
#include "dma.h"
#include "stream_buffer.h"
#include "minmea.h"

static TaskHandle_t xGpsPackTaskHandle = NULL;
static uint8_t      ucDmaBuffer[GPS_DMA_BUFFER_SIZE];

static void vGpsNmeaParser( const uint32_t ulIndex, const size_t xLength )
{
    static uint8_t  ucPackedMessage[NMEA_MAX_MESSAGE_LENGTH];
    static uint32_t ulPackedMessageIndex = 0;

    uint32_t ulLoopIndex;
    uint8_t  ucTempChar;

    for ( ulLoopIndex = ulIndex; ulLoopIndex < xLength; ulLoopIndex++ )
    {
        ucTempChar = ucDmaBuffer[ulLoopIndex];
        if ( ucTempChar == '$' )
        {
            ucPackedMessage[0]   = '$';
            ulPackedMessageIndex = 1;
            continue;
        }

        if ( ucTempChar == '\n' )
        {
            ucPackedMessage[ulPackedMessageIndex] = '\n';
            ulPackedMessageIndex++;
            for ( ; ulPackedMessageIndex < NMEA_MAX_MESSAGE_LENGTH; ulPackedMessageIndex++)
            {
                ucPackedMessage[ulPackedMessageIndex] = '\0';
            }
            switch ( minmea_sentence_id( ( const char* )ucPackedMessage, false ) )
            {
                case MINMEA_SENTENCE_GGA:
                    struct minmea_sentence_gga xExtractInfo;
                    if (minmea_parse_gga( &xExtractInfo, ( const char* )ucPackedMessage ) == true )
                    {
                        toggle_pin();
                    }
                    break;
                default:
                    break;
            }
            ulPackedMessageIndex = 0;
            continue;
        }

        ucPackedMessage[ulPackedMessageIndex] = ucTempChar;
        ulPackedMessageIndex++;
    }
}

static void vGpsPackTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    uint32_t   ulCurrentPosition = 0;
    uint32_t   ulLastPosition    = 0;

    while ( 1 )
    {
        xResult = xTaskNotifyWait( 0, 0, &ulCurrentPosition, portMAX_DELAY );
        
        if( xResult == pdPASS )
        {
            if ( ulCurrentPosition > ulLastPosition )
            {
                vGpsNmeaParser( ulLastPosition, ulCurrentPosition );
            }
            else if ( ulCurrentPosition < ulLastPosition )
            {
                vGpsNmeaParser( ulLastPosition, GPS_DMA_BUFFER_SIZE );
                vGpsNmeaParser( 0, ulCurrentPosition );
            }

            ulLastPosition = ulCurrentPosition;
        }
    }
}

void vGpsStartupTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xCreationState;

    dma1_init( ucDmaBuffer, GPS_DMA_BUFFER_SIZE );
    uart3_init( USART3_BAUD_RATE );

    xCreationState = xTaskCreate( vGpsPackTask,
                                  "GPSpack",
                                  taskGPS_PACK_STACK_SIZE,
                                  NULL,
                                  taskGPS_PACK_STACK_PRIORITY,
                                  &xGpsPackTaskHandle );
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