#include "FreeRTOSTasks.h"
#include "queue.h"
#include "uart.h"
#include "sim800l.h"
#include "minmea.h"

static QueueHandle_t xSimQueue;

static void vSimSendInfoTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    struct minmea_sentence_gga xNmeaInfo;

    while ( 1 )
    {
        xResult = xQueueReceive( xSimQueue, &xNmeaInfo, portMAX_DELAY );
        if ( xResult == pdTRUE )
        {
            /* Send message */
            /* Here must be send xNmeaInfo through SIM800L */
            // toggle_pin();
        }
        else
        {
            /* Error Handler */
        }
    }
}

static void vSimGenerateMesssageTest( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    static uint8_t ucSimpleMessage[] = "$GPGGA,065001.778,5230.789,N,01323.659,E,1,12,1.0,0.0,M,0.0,M,,*6F\r\n";

    while ( 1 )
    {
        switch ( minmea_sentence_id( ( const char* )ucSimpleMessage, false ) )
        {
            case MINMEA_SENTENCE_GGA:
                struct minmea_sentence_gga xExtractInfo;
                if (minmea_parse_gga( &xExtractInfo, ( const char* )ucSimpleMessage ) == true )
                {
                    xResult = xQueueSend( xSimQueue, &xExtractInfo, 0 );
                    // if( xResult == pdTRUE ){toggle_pin();}
                }
                break;
            default:
                break;
        }

        vTaskDelay( pdMS_TO_TICKS(500) );
    }
}

void vSimStartupTask( void* pvParameters )
{
    ( void )pvParameters;
    BaseType_t xResult;
    uart1_init( USART1_BAUD_RATE );

    const UBaseType_t xQueueLength   = QUEUE_LENGTH;
    const UBaseType_t xQueueItemSize = ( const UBaseType_t )sizeof( struct minmea_sentence_gga );

    xSimQueue = xQueueCreate( xQueueLength, xQueueItemSize );
    configASSERT( xSimQueue != NULL );

    xResult = xTaskCreate( vSimSendInfoTask,
                           "sendInfo",
                           taskSIM800_SENDER_STACK_SIZE,
                           NULL,
                           taskSIM800_SENDER_STACK_PRIORITY,
                           NULL );
    configASSERT( xResult == pdPASS );

    /* Test */
    xResult = xTaskCreate( vSimGenerateMesssageTest,
                           "test",
                           taskSIM800_SENDER_STACK_SIZE,
                           NULL,
                           taskSIM800_SENDER_STACK_PRIORITY,
                           NULL );
    configASSERT( xResult == pdPASS );

    vTaskDelete( NULL );
}