#include "gpio.h"

void GPIO_Init( void )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}