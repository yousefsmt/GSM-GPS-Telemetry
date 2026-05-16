#include "FreeRTOS.h"
#include "FreeRTOSTasks.h"

#include "rcc.h"
#include "flash.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "interrupt.h"
#include "dma.h"

#define mainWAITE_STATE ( 0x00U )

#ifdef DEBUG
	#define mainBAUD_RATE   ( 115200U )
	extern volatile uint32_t counter;
#endif /* DEBUG */

static void parse_buffer(const char* buffer, const uint32_t buffer_size );

int main( void )
{
	flash_set_latency( mainWAITE_STATE );

	rcc_init();

	SystemCoreClockUpdate();

	gpio_init();

	#ifdef DEBUG
		uart2_init( mainBAUD_RATE );
	#endif /* DEBUG */

	const uint32_t buffer_size = 256U;
	char buffer[256U] = {0};

	uart1_init( 9600 );
	tim3_init();
	adc1_init();
	adc1_awd_init();
	dma_init(buffer, buffer_size);

	interrupt_set_priorites();

	#ifdef DEBUG
		uint32_t ahb_clk  = get_ahb_clock();
		uint32_t apb1_clk = get_apb1_clock();
		uint32_t apb2_clk = get_apb2_clock();

		uint32_t adc_clk = get_adc_clock( apb2_clk );
		uint32_t tim1_clk = get_tim1_clock( apb1_clk );
		uint32_t timx_clk = get_timx_clock( TIM2, apb1_clk );

		uint32_t mco_clk    = get_mco_clock();
		uint32_t iwdg_clk   = get_iwdg_clock();
		uint32_t rtc_clk    = get_rtc_clock();
		uint32_t pll_clk    = get_pll_clock();
		uint32_t sys_clk    = get_sys_clock();
		uint32_t usb_clk    = get_usb_clock();
		uint32_t flitf_clk  = get_flitf_clock();
		uint32_t free_clk   = get_free_clock();
		uint32_t cortex_clk = get_cortex_clock();

		/**
		 * Remove debug variable for increase stack size
		 * if you want check param please set breakpoint after "get_cortex_clock" call
		 */
		( void )ahb_clk;
		( void )apb1_clk;
		( void )apb2_clk;
		( void )adc_clk;
		( void )tim1_clk;
		( void )timx_clk;
		( void )mco_clk;
		( void )iwdg_clk;
		( void )rtc_clk;
		( void )pll_clk;
		( void )sys_clk;
		( void )usb_clk;
		( void )flitf_clk;
		( void )free_clk;
		( void )cortex_clk;
	#endif /* DEBUG */

	while ( 1 )
	{
		if (counter == 1)
		{
			parse_buffer(buffer, buffer_size );
			toggle_pin();
		}
	}

	return 0;
}

static void parse_buffer(const char* buffer, const uint32_t buffer_size )
{
	uint32_t idx = buffer_size - DMA1_Channel5->CNDTR;

	/* Can I parse NMEA message here???? */
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{

}