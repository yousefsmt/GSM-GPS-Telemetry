#include "FreeRTOSTasks.h"

#include "flash.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "dma.h"
#include "interrupt.h"

#include "gps.h"
#include "system_health.h"

volatile uint32_t new_pos;

#define USART1_BAUD_RATE   ( 115200U )

#ifdef HSE_CLOCK_SPEED
	#define FLASH_WAIT_STATE ( 0x02U )
#else
	#define FLASH_WAIT_STATE ( 0x00U )
#endif

#ifdef DEBUG
#define USART2_BAUD_RATE   ( 115200U )
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

int main( void )
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

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{
	
}