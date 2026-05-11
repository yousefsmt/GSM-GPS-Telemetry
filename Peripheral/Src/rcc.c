#include "rcc.h"

void RCC_Init( void )
{
	/* Use double-word aligned memory */
	SCB->CCR |= SCB_CCR_STKALIGN_Msk;

	/* Turn on HSI */
	RCC->CR |= RCC_CR_HSION;
	while ( ( RCC->CR & RCC_CR_HSIRDY ) == 0 )
	{
		
	}

	/* Switch SYSCLK clock source to HSI */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
	{

	}

	/* Disable CSS, HSE and PLL */
	RCC->CR &= ~( RCC_CR_CSSON | RCC_CR_HSEON | RCC_CR_PLLON );
	while ( ( RCC->CR & ( RCC_CR_HSERDY | RCC_CR_PLLRDY ) ) != 0 )
	{
		/* code */
	}

	/**
	 * Disable Below Options:
	 * MCO, PLL multiplexer
	 * PLL HSE divider
	 * PLL source
	 * ADC prescaler
	 * APB2 prescaler
	 * APB1 prescaler
	 * AHB prescaler
	 */
	RCC->CFGR &= ~( RCC_CFGR_MCO | RCC_CFGR_USBPRE | RCC_CFGR_PLLMULL |
					RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLSRC | RCC_CFGR_ADCPRE |
					RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE );
	
}