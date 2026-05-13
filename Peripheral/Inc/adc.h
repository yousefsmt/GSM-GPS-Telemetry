#ifndef PERIPHERAL_ADC_H_
#define PERIPHERAL_ADC_H_

#include "mcu.h"

#define ADC1_SENSE_PIN  ( 16 )
#define ADC1_REFINT_PIN ( 17 )

#define TIM2_CC2_EVENT  ( 0x03 )

void adc1_awd_init(ADC_TypeDef *adc, uint32_t high_threshold, uint32_t low_threshold);

void adc1_init( void );

#endif /* PERIPHERAL_ADC_H_ */