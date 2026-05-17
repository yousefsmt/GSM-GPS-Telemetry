#ifndef PERIPHERAL_ADC_H_
#define PERIPHERAL_ADC_H_

#include "mcu.h"

#define ADC1_AWD_HTR ( 0x5c9U ) /* 80 C */
#define ADC1_AWD_LTR ( 0x7a9U ) /* -10 C */


#define ADC1_SENSE_PIN  ( 16 )
#define ADC1_REFINT_PIN ( 17 )

#define TIM3_TRGO_EVENT  ( 0x04U )

void adc1_awd_init();

void adc1_init( void );

#if defined( DEBUG ) && defined( MONITOR_INTERNALL_TEMP )
	void print_adc_temp( const uint32_t adc_value );
	void change_htr_lte( const float htr, const float ltr);
#endif /* DEBUG && MONITOR_INTERNALL_TEMP */

#endif /* PERIPHERAL_ADC_H_ */