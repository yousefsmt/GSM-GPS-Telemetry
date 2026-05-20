#include "FreeRTOSTasks.h"

#include "flash.h"
#include "rcc.h"
#include "gpio.h"

static void vBlinkTask( void* args );

int main()
{
	flash_set_latency( 0 );
	rcc_init();
	SystemCoreClockUpdate();
	gpio_init();

	xTaskCreate( &vBlinkTask, "Task1", 128, NULL, 1, NULL );

	vTaskStartScheduler();

	for( ;; ){}
}

static void vBlinkTask( void* args )
{
	( void )args;

	while ( 1 )
	{
		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS(500) );

		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS(200) );
	}
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{

}