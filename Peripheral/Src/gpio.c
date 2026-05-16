#include "gpio.h"

void gpio_init( void )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	#ifdef DEBUG
	    /* Enable clock for PC13 */
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		/* Set output push-pull PC13 */
		GPIOC->CRH &= ~( GPIO_CRH_CNF13 | GPIO_CRH_MODE13 );
		GPIOC->CRH |= ( 0x01 << GPIO_CRH_MODE13_Pos );
	#endif /* DEBUG */
}