#ifndef PERIPHERAL_RCC_H_
#define PERIPHERAL_RCC_H_

#include "mcu.h"

#ifdef DEBUG
#define CLOCK_SW_HSI    ( 0x00U ) /*!< Switch clock sets HSI (internal) */
#define CLOCK_SW_PLLCLK ( 0x01U ) /*!< Switch clock sets PLL (PLLMUL putput) */
#define CLOCK_SW_HSE    ( 0x02U ) /*!< Switch clock sets HSE (external) */
#define CLOCK_NO        ( 0x03U ) /*!< No Clock!!!! (something wrong) */

/**
 * @brief Getter AHB Bus Clock
 * @param none
 * @return uint32_t AHB clock speed
 */
uint32_t get_ahb_clock( void );

/**
 * @brief Getter APB1 Bus Clock
 * @param none
 * @return uint32_t APB1 clock speed
 */
uint32_t get_apb1_clock( void );

/**
 * @brief Getter APB2 Bus Clock
 * @param none
 * @return uint32_t APB2 clock speed
 */
uint32_t get_apb2_clock( void );

/**
 * @brief Getter ADC Bus Clock
 * @param none
 * @return uint32_t ADC clock speed
 */
uint32_t get_adc_clock( uint32_t apb2_clk );

/**
 * @brief Getter TIM1 Bus Clock
 * @param none
 * @return uint32_t TIM1 clock speed
 */
uint32_t get_tim1_clock( uint32_t apb1_clk );

/**
 * @brief Getter TIMx Bus Clock
 * @param none
 * @return uint32_t TIMx clock speed
 */
uint32_t get_timx_clock( TIM_TypeDef *tim_id, uint32_t apb1_clk );

/**
 * @brief Getter MCO Bus Clock
 * @param none
 * @return uint32_t MCO clock speed
 */
uint32_t get_mco_clock( void );

/**
 * @brief Getter IWDG Bus Clock
 * @param none
 * @return uint32_t IWDG clock speed
 */
uint32_t get_iwdg_clock( void );

/**
 * @brief Getter RTC Bus Clock
 * @param none
 * @return uint32_t RTC clock speed
 */
uint32_t get_rtc_clock( void );

/**
 * @brief Getter PLL Bus Clock
 * @param none
 * @return uint32_t PLL clock speed
 */
uint32_t get_pll_clock( void );

/**
 * @brief Getter SYSCLK Bus Clock
 * @param none
 * @return uint32_t SYSCLK clock speed
 */
uint32_t get_sys_clock( void );

/**
 * @brief Getter USB Bus Clock
 * @param none
 * @return uint32_t USB clock speed
 */
uint32_t get_usb_clock( void );

/**
 * @brief Getter FLITF Bus Clock
 * @param none
 * @return uint32_t FLITF clock speed
 */
uint32_t get_flitf_clock( void );

/**
 * @brief Getter Free Bus Clock
 * @param none
 * @return uint32_t Free clock speed
 */
uint32_t get_free_clock( void );

/**
 * @brief Getter Cortex Bus Clock
 * @param none
 * @return uint32_t Cortex clock speed
 */
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