#include "timer.h"

void tim2_init( void )
{
	/* Enable PLL */
	if ( ( RCC->CR & RCC_CR_PLLRDY ) == 0 )
	{
		RCC->CR |= RCC_CR_PLLON;
		while ( ( RCC->CR & RCC_CR_PLLRDY ) == 0) { }
	}

	/* Clear AHB and APB1 prescaler */
	RCC->CFGR &= ~( RCC_CFGR_HPRE | RCC_CFGR_PPRE1 );

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// TIM2->PSC |= TIM_PCS_

}