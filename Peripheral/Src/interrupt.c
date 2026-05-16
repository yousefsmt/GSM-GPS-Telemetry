#include "interrupt.h"

void interrupt_set_priorites( void )
{
	/* ADC interrupt */
	NVIC_SetPriority(ADC1_2_IRQn, 3);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	/* USART1 interrupt */
	NVIC_SetPriority(USART1_IRQn, 2);
	NVIC_EnableIRQ(USART1_IRQn);

	/* DMA interrupt */
	NVIC_SetPriority(DMA1_Channel5_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}