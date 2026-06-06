#include "system_health.h"

#include "queue.h"

#include "adc.h"
#include "timer.h"

static TaskHandle_t xHealthHandler;

static void vSystemHealthBlink( void )
{
	while ( 1 )
	{
		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS( 50 ) );


		toggle_pin();
		vTaskDelay( pdMS_TO_TICKS( 50 ) );
	}
}


static void vSystemHealthCheck( void* pvParameters )
{
	( void )pvParameters;
	BaseType_t           xResult;
	SystemHealthStatus_t xError;

	while( 1 )
	{
		xResult = xTaskNotifyWait( 0, 0, (uint32_t*)&xError, portMAX_DELAY );
		if( xResult == pdPASS )
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


void vSystemHealthStartupTask( void* pvParameters )
{
	( void )pvParameters;
	tim3_init();
	adc1_init();
	adc1_awd_init();

	BaseType_t xTaskReturn = xTaskCreate( vSystemHealthCheck,
										  "SystemHealth",
										  taskSYSTEM_HEALTH_STACK_SIZE,
										  NULL,
										  taskSYSTEM_HEALTH_STACK_PRIORITY,
										  &xHealthHandler );

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

		ADC1->SR &= ~ADC_SR_AWD;
		
		if ( xHealthHandler != NULL )
        {
			xTaskNotifyFromISR( xHealthHandler, xError, eSetValueWithOverwrite, &xSwitchRequired );
        }
		
		portYIELD_FROM_ISR(xSwitchRequired);
	}
	
	/**
	 * Check end of conversion
	 */
	if (ADC1->SR & ADC_SR_EOC)
	{
		// toggle_pin();
		/* Clear EOC flag */
		ADC1->SR &= ~ADC_SR_EOC;
	}
}