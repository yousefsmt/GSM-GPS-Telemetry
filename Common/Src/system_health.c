#include "system_health.h"

volatile bool sensor_task_ok = false;

static volatile bool mcu_temp_ok = true;

static volatile uint8_t conversion_done = 0;
static volatile uint16_t adv_val = 0;


// static void sys_health_task( void *pvParameters )
// {
// 	float internal_temp;

// 	if ( ( CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk ) == 0 )
// 	{


// 	}


// 	bool system_healthy;

// 	// const TickType_t xFrequency = pdMS_TO_TICKS( 1000 );
// 	// TickType_t xLastWakeTime = xTaskGetTickCount();

// 	while ( 1 )
// 	{
// 		// xTaskDelayUntil( &xLastWakeTime, xFrequency );

// 		if ( conversion_done )
// 		{
// 			conversion_done = 0;
// 		}

// 		system_healthy = ( sensor_task_ok & mcu_temp_ok );

// 		if ( system_healthy )
// 		{
// 			sensor_task_ok = false;
// 		}
// 		else
// 		{

// 		}

// 		( void )internal_temp;

// 	}
	
// }

void startup_task( void )
{
	// tim init
	// adc init

	/* task create */
}