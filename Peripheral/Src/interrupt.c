#include "interrupt.h"

void interrupt_set_priorites( void )
{
	/* ADC interrupt ( System Health ) */
	// NVIC_SetPriority(ADC1_2_IRQn, 2);
	// NVIC_EnableIRQ(ADC1_2_IRQn);

	/* USART3 interrupt ( NEO-6M Message ) */
	NVIC_SetPriority(USART3_IRQn, 6);
	NVIC_EnableIRQ(USART3_IRQn);
}