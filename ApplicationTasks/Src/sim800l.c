#include "uart.h"

#include "sim800l.h"

void vSimStartupTask( void* pvParameters )
{
    ( void )pvParameters;
    uart1_init( USART1_BAUD_RATE );

}