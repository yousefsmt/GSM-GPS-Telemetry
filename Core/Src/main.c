#include "rcc.h"
#include "flash.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"

#define mainWAITE_STATE ( 0x00U )
#define mainBAUD_RATE   ( 115200U )

int main( void )
{
	FLASH_ConfigWaitState( mainWAITE_STATE );

	RCC_Init();

	SystemCoreClockUpdate();

	GPIO_Init();

	UART_Init( mainBAUD_RATE );

	ADC_Init();

	while ( 1 )
	{
		printf( "Hello\n" );
		for (volatile unsigned int i = 0; i < 100000; i++)
		{
			/* code */
		}
	}

	return 0;
}