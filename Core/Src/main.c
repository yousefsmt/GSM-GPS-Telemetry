#include "rcc.h"
#include "flash.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "interrupt.h"

volatile uint32_t new_pos;

// #define USART1_BAUD_RATE   ( 9600U )
#define USART3_BAUD_RATE   ( 115200U )

#ifdef HSE_CLOCK_SPEED
	#define FLASH_WAITE_STATE ( 0x02U )
#else
	#define FLASH_WAITE_STATE ( 0x00U )
#endif

#ifdef DEBUG
	#define USART2_BAUD_RATE   ( 115200U )
#endif /* DEBUG */

static void par( const uint8_t* buffer )
{
    static uint32_t local_pos = 0x00U;

    while ( local_pos != new_pos)
    {
        printf("%c", buffer[local_pos]);
        local_pos = ( local_pos + 1 ) % 256;
    }

    printf("\n");
    printf("local_pos: %d\n", local_pos);
}

int main( void )
{
	flash_set_latency( FLASH_WAITE_STATE );

	rcc_init();

	SystemCoreClockUpdate();

	gpio_init();
	// uart1_init( USART1_BAUD_RATE );
	uart3_init( USART3_BAUD_RATE ); /* nmea message */
	uart2_init( USART2_BAUD_RATE ); /* printf */

	uint8_t buffer[256];
	dma_init( buffer, 256 );

	interrupt_set_priorites();

	while ( 1 )
	{
		buffer[new_pos] = '\0';
		printf("%s\n", buffer );
		for (size_t i = 0; i < 100000; i++) { }
	}

	return 0;
}

void USART3_IRQHandler( void )
{
    if ( USART3->SR & USART_SR_IDLE )
    {
        volatile uint32_t tmp;
        tmp = USART3->DR;
        (void)tmp;

        new_pos = 256 - DMA1_Channel3->CNDTR;
    }
}

#ifdef DEBUG
	/**
	 * Implement putchar for printf syscall
	 * to send character instead standard I/O Linux to usart2
	 */
	int __io_putchar( int ch )
	{
		while ( ( USART2->SR & USART_SR_TXE ) == 0 ) { }
		USART2->DR = ch;
		return ch;
	}
#endif /* DEBUG */



const TickType_t xInterruptFrequency = pdMS_TO_TICKS( 500UL );

static void vHandlerTask( void *pvParameters )
{
	/* xMaxExpectedBlockTime is set to be a little longer than the maximum expected time
	between events. */
	const TickType_t xMaxExpectedBlockTime = xInterruptFrequency + pdMS_TO_TICKS( 10 );
	uint32_t ulEventsToProcess;
	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* Wait to receive a notification sent directly to this task from the
		interrupt service routine. */
		ulEventsToProcess = ulTaskNotifyTake( pdTRUE, xMaxExpectedBlockTime );
		if( ulEventsToProcess != 0 )
		{
			/* To get here at least one event must have occurred. Loop here until
			all the pending events have been processed (in this case, just print out
			a message for each event). */
			while( ulEventsToProcess > 0 )
			{
				vPrintString( "Handler task - Processing event.\r\n" );
				ulEventsToProcess--;
			}
		}
		else
		{
			/* If this part of the function is reached then an interrupt did not
			arrive within the expected time, and (in a real application) it may be
			necessary to perform some error recovery operations. */
		}
	}
}

static uint32_t ulExampleInterruptHandler( void )
{
	BaseType_t xHigherPriorityTaskWoken;
	/* The xHigherPriorityTaskWoken parameter must be initialized to pdFALSE as
	it will get set to pdTRUE inside the interrupt safe API function if a
	context switch is required. */
	xHigherPriorityTaskWoken = pdFALSE;
	/* Send a notification directly to the task to which interrupt processing is
	being deferred. */
	vTaskNotifyGiveFromISR( /* The handle of the task to which the notification
								is being sent. The handle was saved when the task
								was created. */
								xHandlerTask,
								/* xHigherPriorityTaskWoken is used in the usual
								way. */
								&xHigherPriorityTaskWoken );
	/* Pass the xHigherPriorityTaskWoken value into portYIELD_FROM_ISR(). If
	xHigherPriorityTaskWoken was set to pdTRUE inside vTaskNotifyGiveFromISR()
	then calling portYIELD_FROM_ISR() will request a context switch. If
	xHigherPriorityTaskWoken is still pdFALSE then calling
	portYIELD_FROM_ISR() will have no effect. The implementation of
	portYIELD_FROM_ISR() used by the Windows port includes a return statement,
	which is why this function does not explicitly return a value. */
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}