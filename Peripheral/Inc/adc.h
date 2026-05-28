#ifndef PERIPHERAL_ADC_H_
#define PERIPHERAL_ADC_H_

#include "mcu.h"

#define ADC1_AWD_HTR        ( 1961U ) /*!< converted 80 C to hex adc value  */
#define ADC1_AWD_LTR        ( 1481U ) /*!< converted -10 C to hex adc value */
#define ADC1_SENSE_PIN      ( 0x10U )  /*!< V_{SENSE} pin for internal temp sensor  */
#define ADC1_REFINT_PIN     ( 0x11U )  /*!< V_{REFINT} pin for internal temp sensor */
#define TIM3_TRIGGER_EVENT  ( 0x04U )  /*!< External trigger for measure ADC temp */

/**
 * @brief  Analog Watchdog Initialize
 * @param  none.
 * @return none.
 * 
 * Configuration and initialization AWD for set high and low threshold internal temp
 */
void adc1_awd_init();

/**
 * @brief  A/D 1 Initialize
 * @param  none
 * @return none.
 * 
 * Configuration and initialization A/D 1. setting PLL amd calibration
 */
void adc1_init( void );

#if defined( DEBUG ) && defined( MONITOR_INTERNAL_TEMP )
	/**
	 * @brief  Convert ADC value to Celsius unit
	 * @param  adc_value ADC extract value
	 * @return none.
	 * 
	 * Input value is 12 Bit and convert this value with reference voltage to temperature
	 */
	void print_adc_temp( const uint32_t adc_value );

	/**
	 * @brief  convert high and low threshold to ADC value
	 * @param  htr high threshold for AWD unit
	 * @param  ltr low threshold for AWD unit
	 * @return none.
	 * 
	 * Input value with celsius unit and convert these to acd val
	 */
	void change_htr_lte( const float htr, const float ltr);
#endif /* DEBUG && MONITOR_INTERNAL_TEMP */

#endif /* PERIPHERAL_ADC_H_ */