#include "timer.h"

void tim3_init( void )
{
	/* Enable APB1 clock for TIM3 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->CR1 &= ~( TIM_CR1_CEN );

	/**
	 * Setting Prescaler and Auto-Reload
	 * Clock Source HSI = 8 MHz = 8000000
	 * Timer 2 Clock = ( Clock Source ) / ( TIM3->PSC + 1 ) = 10000 = 10 kHz
	 * Auto Reload = ( Timer 2 Clock ) / ( TIM3->ARR + 1 )  = 1
	 */
	TIM3->PSC = 7199;
	TIM3->ARR = 9999;

	/**
	 * Now, I set TIM3_TRGO for trigger ADC temperature measurement
	*/
	TIM3->CR2 &= ~( TIM_CR2_MMS );
  	TIM3->CR2 |= (TIM_CR2_MMS_1);

	/* Enable update interrupt generation */
	TIM3->EGR |= (TIM_EGR_UG);

	/* Enable TIM3 counter */
	TIM3->CR1 |= (TIM_CR1_CEN);
}
