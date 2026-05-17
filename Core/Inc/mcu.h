#ifndef CORE_MCU_H_
#define CORE_MCU_H_

#include <stdio.h>
#include "stm32f1xx.h"

#ifdef DEBUG
	#define LOG( msg... ) printf( msg )

	#define toggle_pin() \
		if (GPIOC->ODR & GPIO_ODR_ODR13) 	\
		{                                	\
			GPIOC->BSRR = GPIO_BSRR_BR13;	\
		}									\
		else								\
		{									\
			GPIOC->BSRR = GPIO_BSRR_BS13;	\
		}
#else
	#define toggle_pin()
	#define LOG( msg... )
#endif /* DEBUG */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#endif /* CORE_MCU_H_ */