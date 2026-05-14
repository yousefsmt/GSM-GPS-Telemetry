#ifndef PERIPHERAL_RCC_H_
#define PERIPHERAL_RCC_H_

#include "mcu.h"

#ifdef DEBUG
	#define CLOCK_SW_HSI    ( 0x00U )
	#define CLOCK_SW_PLLCLK ( 0x01U )
	#define CLOCK_SW_HSE    ( 0x02U )
	#define CLOCK_NO        ( 0x03U )
	/**
	 * extract main bus speed
	 */
	uint32_t get_ahb_clock( void );
	uint32_t get_apb1_clock( void );
	uint32_t get_apb2_clock( void );
	/*----------------------------------------*/

	/**
	 * extract adc, timer clock speed after APBx bus
	 */
	uint32_t get_adc_clock( uint32_t apb2_clk );
	uint32_t get_tim1_clock( uint32_t apb1_clk );
	uint32_t get_timx_clock( TIM_TypeDef *tim_id, uint32_t apb1_clk );
	/*----------------------------------------*/

	uint32_t get_mco_clock( void );

	uint32_t get_iwdg_clock( void );

	uint32_t get_rtc_clock( void );

	uint32_t get_pll_clock( void );

	uint32_t get_sys_clock( void );

	uint32_t get_usb_clock( void );

	uint32_t get_flitf_clock( void );

	uint32_t get_free_clock( void );

	uint32_t get_cortex_clock( void );
#endif /* DEBUG */

/**
 * @brief Initialize and configure the Reset and Clock Control (RCC) peripheral
 * @param none
 * @return none
 * 
 * This function sets up the system clock
 */
void rcc_init( void );

#endif /* PERIPHERAL_RCC_H_ */