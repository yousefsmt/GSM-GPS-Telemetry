#include "FreeRTOSTasks.h"

#include "flash.h"
#include "rcc.h"
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

#define USART3_BAUD_RATE ( 9600 ) /* Communicate by SIM800L */

static void vStartupTask( void* pvParameters );
static void vInitializePeripheral( void );

int main()
{
	vInitializePeripheral();

	BaseType_t xReturn = xTaskCreate( &vStartupTask,
									  "Task1",
									  taskSTARTUP_STACK_SIZE,
									  NULL,
									  taskSTARTUP_STACK_PRIORITY,
									  NULL );

	configASSERT( xReturn == pdPASS );

	vTaskStartScheduler();

	for( ;; ) { }
}

static void vStartupTask( void* pvParameters )
{
	( void )pvParameters;

	vGpsStartupTask();

	vSystemHealthStartupTask();

	vTaskDelete( NULL );
}

static void vInitializePeripheral( void )
{
	flash_set_latency( FLASH_WAIT_STATE );
	rcc_init();
	SystemCoreClockUpdate();
	gpio_init();

	uart1_init( USART1_BAUD_RATE );

	#ifdef DEBUG
		uart2_init( USART2_BAUD_RATE );
	#endif /* DEBUG */

	interrupt_set_priorites();
}