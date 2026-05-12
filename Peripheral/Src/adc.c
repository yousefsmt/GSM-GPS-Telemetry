#include "adc.h"

/**
 * The The temperature sensor is internally
 * connected to the ADC12_IN16 and must be enable ADC_CR2_TSVREFE
 * NOTE: The best sampling time for internal sensor is 17.1 us
 * Clock = 4 MHz
 * Cycle Duration = 0.25 us
 * Total Cycle Need for SMPR1 = Ideal SR / Cycle Duration = 68.4 ~ 71.5 Cycles (SMP16 = 110)
 */
void ADC_Init( void )
{
	/* Enable PLL */
	RCC->CR |= RCC_CR_PLLON;
	while ( ( RCC->CR & RCC_CR_PLLRDY ) == 0) { LOG( "wait pllrdy\n" ); }

	/* Enable AHB & APB2 PLL nor divider */
	RCC->CFGR &= ~( RCC_CFGR_HPRE | RCC_CFGR_PPRE2 );

	/* Set /2 ADC PLL */
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;
	
	/* Enable ADC1 source clock */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	ADC1->CR1 = 0;
	ADC1->CR2 = 0;

	ADC1->CR1 |= ADC_CR1_EOCIE;

	/**
	 * Set sampling time 71.5 cycles = ( 71.5 * 0.25 ) = 17.875 us It's approximately good!!
	 * read each 17.875 us data from ADC1_IN16 and ADC1_IN17 for V_sense and V_refint
	 */
	ADC1->SMPR1 |= ( ( 0x06 << ADC_SMPR1_SMP16_Pos ) | ( 0x06 << ADC_SMPR1_SMP17_Pos ) );

	/* Enable internal temperature sensor */
	ADC1->CR2 |= ADC_CR2_TSVREFE;

	/* External event set to SWSTART */
	ADC1->CR2 |= ( 0x07 << ADC_CR2_EXTSEL_Pos );

	/* Turn on ADC1 */
	ADC1->CR2 |= ADC_CR2_ADON;

	for (volatile unsigned int i = 0; i < 5; i++)
	{
		/* code */
	}
	

	/* Reset calibration*/
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while ( ( ADC1->CR2 & ADC_CR2_RSTCAL ) ) { LOG( "wait rest\n" ); }

	/* Start calibration */
	ADC1->CR2 |= ADC_CR2_CAL;
	while ( ( ADC1->CR2 & ADC_CR2_CAL ) ) { LOG( "wait callibre\n" ); }
	
}