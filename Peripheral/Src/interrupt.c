#include "interrupt.h"

void interrupt_set_priorites( void )
{
	NVIC_SetPriority(ADC1_2_IRQn, 0x03);
	NVIC_EnableIRQ(ADC1_2_IRQn);
}