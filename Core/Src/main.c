#include "FreeRTOSTasks.h"
#include "rcc.h"
#include "flash.h"
#include "gpio.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "interrupt.h"
#include "dma.h"

#define USART1_BAUD_RATE   ( 9600U )
#define USART3_BAUD_RATE   ( 9600U )

#define FLASH_WAITE_STATE ( 0x00U )

#ifdef DEBUG
	#define USART2_BAUD_RATE   ( 115200U )
#endif /* DEBUG */

int main( void )
{
	flash_set_latency( FLASH_WAITE_STATE );

	rcc_init();

	SystemCoreClockUpdate();

	gpio_init();

	#ifdef DEBUG
		uart2_init( USART2_BAUD_RATE );
	#endif /* DEBUG */

	uart1_init( USART1_BAUD_RATE );
	uart3_init( USART3_BAUD_RATE );
	tim3_init();
	adc1_init();
	adc1_awd_init();

	#if defined( DEBUG ) && defined( TEST_DMA )
		const uint32_t buffer_size = 256U;
		char buffer[256U] = {0};
		dma_init(buffer, buffer_size);
	#endif /* DEBUG && TEST_DMA */

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

	vTaskStartScheduler();

	while ( 1 ) { }

	return 0;
}

/**
 * Set ISR for NEO-6M receive message
 */
void USART1_IRQHandler( void )
{
    if ( USART1->SR & USART_SR_RXNE )
    {
		uint8_t data = USART1->DR;
		ringbuf_write( ring_buffer_usart1, data );
		if ( data == '\r' ) { newline_usart1 = 1; }
    }

	if ( ( USART1->SR & USART_SR_IDLE ) ) { }
}

/**
 * Set ISR for transmit and receive AT command to SIM800L
 */
void USART3_IRQHandler( void )
{
    if ( USART1->SR & USART_SR_RXNE )
    {
        uint8_t data = USART1->DR;
		ringbuf_write( ring_buffer_usart3, data );
		if ( data == '\r' ) { newline_usart3 = 1; }
    }

	if ( ( USART1->SR & USART_SR_IDLE ) ) { }

	if ( ( USART1->SR & USART_SR_TXE ) ) { }

	if ( ( USART1->SR & USART_SR_TC ) ) { }
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

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName)
{

}