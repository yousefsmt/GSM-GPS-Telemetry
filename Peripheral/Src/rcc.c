#include "rcc.h"

#ifdef DEBUG
	static uint32_t clock_src = CLOCK_SW_HSI;
#endif /* DEBUG */

void rcc_init( void )
{
	/* Use double-word aligned memory */
	SCB->CCR |= SCB_CCR_STKALIGN_Msk;

	/* Turn on HSI */
	RCC->CR |= RCC_CR_HSION;
	while ( ( RCC->CR & RCC_CR_HSIRDY ) == 0 ) { }

	/* Switch SYSCLK clock source to HSI */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) { }

	/* Disable CSS, HSE and PLL */
	RCC->CR &= ~( RCC_CR_CSSON | RCC_CR_HSEON | RCC_CR_PLLON );
	while ( ( RCC->CR & ( RCC_CR_HSERDY | RCC_CR_PLLRDY ) ) != 0 ) { }

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

	#ifdef DEBUG
		switch ( ( RCC->CFGR & RCC_CFGR_SW ) )
		{
		case 0b00:
			clock_src = CLOCK_SW_HSI;
			break;
		case 0b01:
			clock_src = CLOCK_SW_HSE;
			break;
		case 0b10:
			clock_src = CLOCK_SW_PLLCLK;
			break;
		case 0b11:
			clock_src = CLOCK_NO;
			break;
		default:
			break;
		}
	#endif /* DEBUG */

}

#ifdef DEBUG

	uint32_t get_mco_clock( void )
	{
		switch ( ( RCC->CFGR & RCC_CFGR_MCO ) )
		{
		case 0b000000000000000000000000000:
			return 0x00U;
		case 0b100000000000000000000000000:
		case 0b110000000000000000000000000:
			return get_sys_clock();
		case 0b101000000000000000000000000:
			return 8000000U;
		case 0b111000000000000000000000000:
			return get_pll_clock() / 2;
		default:
			return 1;
		}
	}

	uint32_t get_iwdg_clock( void )
	{
		return ( RCC->CIR & RCC_CIR_LSIRDYF ) ? 40000U : 0U;
	}

	uint32_t get_rtc_clock( void )
	{
		switch ( ( RCC->BDCR & RCC_BDCR_RTCSEL ) )
		{
		case 0b0000000000:
			return 0;
		case 0b0100000000:
			return 32768;
		case 0b1000000000:
			return 40000;
		case 0b1100000000:
			return get_sys_clock() / 128;
		default:
			break;
		}

		return 0;
	}

	uint32_t get_pll_clock( void )
	{
		switch ( ( RCC->CFGR & RCC_CFGR_PLLSRC ) )
		{
		case 0b000000000000000000:
			return ( ( RCC->CFGR & RCC_CFGR_PLLMULL_Msk ) + 2 ) * 4;
		case 0b010000000000000000:
			return ( ( RCC->CFGR & RCC_CFGR_PLLMULL_Msk ) + 2 ) * ( SystemCoreClock / 1000 );
		default:
			break;
		}

		return 0;
	}

	uint32_t get_sys_clock( void )
	{
		switch ( ( RCC->CFGR & RCC_CFGR_SW ) )
		{
		case 0b00:
			clock_src = CLOCK_SW_HSI;
			break;
		case 0b01:
			clock_src = CLOCK_SW_HSE;
			break;
		case 0b10:
			clock_src = CLOCK_SW_PLLCLK;
			break;
		case 0b11:
			clock_src = CLOCK_NO;
			break;
		default:
			break;
		}

		return 0;
	}

	uint32_t get_usb_clock( void )
	{
		uint32_t pllclk = 0;
		switch ( ( RCC->CFGR & RCC_CFGR_PLLSRC ) )
		{
		case 0b000000000000000000:
			pllclk =  ( ( RCC->CFGR & RCC_CFGR_PLLMULL_Msk ) + 2 ) * 4;
			break;
		case 0b010000000000000000:
			pllclk = ( ( RCC->CFGR & RCC_CFGR_PLLMULL_Msk ) + 2 ) * ( SystemCoreClock / 1000 );
			break;
		default:
			break;
		}

		return ( ( RCC->CFGR & RCC_CFGR_USBPRE ) ) ? pllclk : ( pllclk / ( 1.5 ) );
	}

	uint32_t get_flitf_clock( void )
	{
		if ( (RCC->CR & RCC_CR_HSIRDY ) )
		{
			return 8000000;
		}
		else
		{
			return 0;
		}

	}

	uint32_t get_adc_clock( uint32_t apb2_clk )
	{
		switch ( ( RCC->CFGR & RCC_CFGR_ADCPRE ) )
		{
		case RCC_CFGR_ADCPRE_DIV2:
			return apb2_clk / 2;
		case RCC_CFGR_ADCPRE_DIV4:
			return apb2_clk / 4;
		case RCC_CFGR_ADCPRE_DIV6:
			return apb2_clk / 6;
		case RCC_CFGR_ADCPRE_DIV8:
			return apb2_clk / 8;
		default:
			break;
		}

		return 0;
	}

	uint32_t get_tim1_clock( uint32_t apb2_clk )
	{
		return apb2_clk / ( TIM1->PSC + 0x01U );
	}

	uint32_t get_timx_clock( TIM_TypeDef *tim_id, uint32_t apb1_clk )
	{
		return apb1_clk / ( tim_id->PSC + 0x01U );
	}

	uint32_t get_free_clock( void )
	{
		return get_ahb_clock();
	}

	uint32_t get_cortex_clock( void )
	{
		return get_ahb_clock() / 8;
	}

	/*-----------------------------------------------------------------*/
	uint32_t get_ahb_clock( void )
	{
		SystemCoreClockUpdate();
		uint32_t ahb_clk  = SystemCoreClock;

		switch ( ( RCC->CFGR & RCC_CFGR_HPRE ) )
		{
		case 0b00000000:
			ahb_clk /= 1;
			break;
		case 0b10000000:
			ahb_clk /= 2;
			break;
		case 0b10010000:
			ahb_clk /= 4;
			break;
		case 0b10100000:
			ahb_clk /= 8;
			break;
		case 0b10110000:
			ahb_clk /= 16;
			break;
		case 0b11000000:
			ahb_clk /= 64;
			break;
		case 0b11010000:
			ahb_clk /= 128;
			break;
		case 0b11100000:
			ahb_clk /= 256;
			break;
		case 0b11110000:
			ahb_clk /= 512;
			break;
		default:
			break;
		}

		return ahb_clk;

	}

	uint32_t get_apb1_clock( void )
	{
		uint32_t apb1_clk = get_ahb_clock();

		switch ( ( RCC->CFGR & RCC_CFGR_PPRE1 ) )
		{
		case 0b00000000000:
			apb1_clk /= 1;
			break;
		case 0b10000000000:
			apb1_clk /= 2;
			break;
		case 0b10100000000:
			apb1_clk /= 4;
			break;
		case 0b11000000000:
			apb1_clk /= 8;
			break;
		case 0b11100000000:
			apb1_clk /= 16;
			break;
		default:
			break;
		}

		return apb1_clk;

	}

	uint32_t get_apb2_clock( void )
	{
		uint32_t apb2_clk  = get_ahb_clock();

		switch ( ( RCC->CFGR & RCC_CFGR_PPRE2 ) )
		{
		case 0b00000000000000:
			apb2_clk /= 1;
			break;
		case 0b10000000000000:
			apb2_clk /= 2;
			break;
		case 0b10100000000000:
			apb2_clk /= 4;
			break;
		case 0b11000000000000:
			apb2_clk /= 8;
			break;
		case 0b11100000000000:
			apb2_clk /= 16;
			break;
		default:
			break;
		}

		return apb2_clk;

	}

#endif /* DEBUG */