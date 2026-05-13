#include "adc.h"

/**
 * Self-contained calibration after power-down
 */
static void adc1_calibrate( void )
{
	/* Reset calibration*/
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while ( ( ADC1->CR2 & ADC_CR2_RSTCAL ) ) { }

	/* Start calibration */
	ADC1->CR2 |= ADC_CR2_CAL;
	while ( ( ADC1->CR2 & ADC_CR2_CAL ) ) { }
}

static void adc1_set_pll( void )
{
	/**
	 * NOTE: In some reference say if ADCCLK in lower better accuracy ( I must to be check deeply!! )
	 */

	/* Enable PLL */
	if ( ( RCC->CR & RCC_CR_PLLRDY ) == 0 )
	{
		RCC->CR |= RCC_CR_PLLON;
		while ( ( RCC->CR & RCC_CR_PLLRDY ) == 0) { }
	}

	/* Clear AHB and APB2 prescaler */
	RCC->CFGR &= ~( RCC_CFGR_HPRE | RCC_CFGR_PPRE2 );

	/* Set /2 ADC PLL */
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV2;
	
	/* Enable ADC1 source clock */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
}

/**
 * This function set all values in control register for ADC1
 * used by system health (check internall temp and AWD)
 */
static void adc1_config( void )
{
	/* Clear control register */
	ADC1->CR1 = 0x00U;
	ADC1->CR2 = 0x00U;

	/* Enable EOC (End of Conversion) interrupt */
	ADC1->CR1 |= ADC_CR1_EOCIE;

	/**
	 * External trigger enable
	 * Set external trigger to TIM2_CC2_EVENT
	 */
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	ADC1->CR2 |= ( TIM2_CC2_EVENT << ADC_CR2_EXTSEL_Pos );

	/* Enable internal temperature sensor */
	ADC1->CR2 |= ADC_CR2_TSVREFE;

	/**
	 * Set sampling time 71.5 cycles = ( 71.5 * 0.25 ) = 17.875 us It's approximately good!!
	 * read each 17.875 us data from ADC1_IN16 and ADC1_IN17 for V_sense and V_refint
	 */
	ADC1->SMPR1 |= ( ( 0x06 << ADC_SMPR1_SMP16_Pos ) | ( 0x06 << ADC_SMPR1_SMP17_Pos ) );

	/**
	 * At this point I just read V_sense
	 * TODO: Must be read V_refint for giving more accuracy for convert sense to voltage
	 */
	ADC1->SQR3 |= ( ADC1_SENSE_PIN << ADC_SQR3_SQ1_Pos );

	/**
	 * NOTE: In reference manual say after set ADON must be wait t_stat for turn on in f103 max 1us
	 */
	/* Turn on ADC1 */
	ADC1->CR2 |= ADC_CR2_ADON;
}

/**
 * The The temperature sensor is internally
 * connected to the ADC12_IN16 and must be enable ADC_CR2_TSVREFE
 * NOTE: The best sampling time for internal sensor is 17.1 us
 * Clock = 4 MHz
 * Cycle Duration = 0.25 us
 * Total Cycle Need for SMPR1 = Ideal SR / Cycle Duration = 68.4 ~ 71.5 Cycles (SMP16 = 110)
 */
void adc1_init( void )
{
	/* Enable clock and set PLL */
	adc1_set_pll();

	/* Configuration control register (CR1 & CR2), then turn ADC on*/
	adc1_config();

	/* After turned on based RM0008 (reference manual) section 11.4 */
	adc1_calibrate();

	/* Add ADC1 to interrupt vector */
	NVIC_EnableIRQ( ADC1_IRQn );
}

void adc1_awd_init(ADC_TypeDef *adc, uint32_t high_threshold, uint32_t low_threshold)
{
	adc->HTR = high_threshold;
	adc->LTR = low_threshold;

	adc->CR1 |= ( ADC1_SENSE_PIN << ADC_CR1_AWDCH_Pos );

	adc->CR1 |= ( ADC_CR1_AWDIE | ADC_CR1_AWDEN );
}

void ADC_IRQHandler(void)
{
	if(ADC1->SR & ADC_SR_AWD)
	{
		// watchDogTrig=1;
		ADC1->SR &=~ADC_SR_AWD;

	}
}
