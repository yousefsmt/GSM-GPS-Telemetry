#include "gpio.h"

void gpio_init( void )
{
	/**
	 * Port A used by USART1 ( NEO-6M ) & USART2 ( Debug version "printf" )
	 * Port B used by USART3
	 */
	RCC->APB2ENR |= ( RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN );

	/**
	 * Enable PC13 clock and CNF & mode for toggle_pin()
	 */
	#ifdef DEBUG
	    /* Enable clock for PC13 */
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		/* Set output push-pull PC13 */
		GPIOC->CRH &= ~( GPIO_CRH_CNF13 | GPIO_CRH_MODE13 );
		GPIOC->CRH |= ( 0x01 << GPIO_CRH_MODE13_Pos );
	#endif /* DEBUG */
}