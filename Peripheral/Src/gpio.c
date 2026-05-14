#include "gpio.h"

void gpio_init( void )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	#ifdef DEBUG
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
		GPIOC->CRH &= ~( GPIO_CRH_CNF13 | GPIO_CRH_MODE13 );
		GPIOC->CRH |= ( 0x01 << GPIO_CRH_MODE13_Pos );
	#endif /* DEBUG */
}