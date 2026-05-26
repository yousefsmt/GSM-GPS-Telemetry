#include "system_health.h"

#include "queue.h"

#include "adc.h"
#include "timer.h"


static QueueHandle_t xSystemHealthQueue;


static void vSystemHealthBlink( void )
{
	while ( 1 )
	{
		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS( 200 ) );


		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS( 200 ) );
	}
}


static void vSystemHealthCheck( void* pvParameters )
{
	( void )pvParameters;
	BaseType_t           xState;
	SystemHealthStatus_t xError;

	while( 1 )
	{
		xState = xQueueReceive( xSystemHealthQueue, &xError, portMAX_DELAY );
		if( xState == pdTRUE )
		{
			/* Can store all runtime errors
			if (xError < ERROR_MAX_NUMBER)
			{

			}
			*/

			switch ( xError )
			{
			case ERROR_TEMPERATURE_THRESHOLD_EXCEEDED:
				vSystemHealthBlink();
				break;
			default:
				break;
			}
		}
	}
}


void vSystemHealthStartupTask( void )
{
	tim3_init();
	adc1_init();
	adc1_awd_init();

	xSystemHealthQueue = xQueueCreate( QUEUE_SYSTEM_HEALTH_LENGTH, QUEUE_SYSTEM_HEALTH_ITEM_SIZE );
	configASSERT( xSystemHealthQueue != NULL );

  	vQueueAddToRegistry( xSystemHealthQueue, "SystemHealthQueue" );

	BaseType_t xTaskReturn = xTaskCreate( vSystemHealthCheck,
										  "SystemHealth",
										  taskSYSTEM_HEALTH_STACK_SIZE,
										  NULL,
										  taskSYSTEM_HEALTH_STACK_PRIORITY,
										  NULL );

	configASSERT( xTaskReturn == pdPASS );

	vTaskDelete( NULL );
}

/**
 * When interrupt occur this function check why happend?
 */
void ADC1_2_IRQHandler( void )
{
	/**
	 * TODO: Check AWD occur
	 * TODO: Check End Of Conversion
	 */

	/**
	 * Check AWD occur break high or low threshold
	 */
	if (ADC1->SR & ADC_SR_AWD)
	{
		BaseType_t           xSwitchRequired = pdFALSE;
		SystemHealthStatus_t xError          = ERROR_TEMPERATURE_THRESHOLD_EXCEEDED;

  		( void )xQueueSendFromISR(xSystemHealthQueue, &xError, &xSwitchRequired);

  		portYIELD_FROM_ISR(xSwitchRequired);

		ADC1->SR &= ~ADC_SR_AWD;
	}

	/**
	 * Check end of conversion
	 */
	if (ADC1->SR & ADC_SR_EOC)
	{
		/* Clear EOC flag */
		ADC1->SR &= ~ADC_SR_EOC;
	}
}