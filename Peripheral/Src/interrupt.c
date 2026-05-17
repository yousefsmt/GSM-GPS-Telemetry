#include "interrupt.h"

void interrupt_set_priorites( void )
{
	/* ADC interrupt ( System Health ) */
	NVIC_SetPriority(ADC1_2_IRQn, 3);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	/* USART1 interrupt ( NEO-6M GPS message ) */
	NVIC_SetPriority(USART1_IRQn, 2);
	NVIC_EnableIRQ(USART1_IRQn);

	#ifdef TEST_DMA
		/* USART3 interrupt ( SIM800L AT command ) */
		NVIC_SetPriority(USART3_IRQn, 1);
		NVIC_EnableIRQ(USART3_IRQn);

		/* DMA interrupt ( Test DMA )*/
		NVIC_SetPriority(DMA1_Channel5_IRQn, 1);
		NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	#else
		/* USART3 interrupt ( SIM800L AT command ) */
		NVIC_SetPriority(USART3_IRQn, 1);
		NVIC_EnableIRQ(USART3_IRQn);
	#endif /* TEST_DMA */
}