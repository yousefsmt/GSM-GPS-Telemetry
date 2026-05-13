#include "gpio.h"

void gpio_init( void )
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
}