#include "uart.h"

/**
 * TODO: Implement printf function with UART2 to send string array to serial port [DONE]
 * TODO: Implement NMEA GPS message parser. Incoming message from NEO-6M with UART1 connection
 * TODO: Implement SIM800L connection with UART3. Handle AT-Command
 */

#ifdef DEBUG
void uart2_init( const uint32_t baud_rate )
{
	/**
	 * printf logging method
	 * 1: Enable USART2 Clock
	 * 2: Enable GPIOA clock ( GPIO_Init() )
	 * 3: Set Alternative function output push-pull on CRL
	 * 4: Set baud rate
	 * 5: Enable transmission
	 */

	#ifdef HSE_CLOCK_SPEED
		uint32_t usartdiv = ( SystemCoreClock / ( baud_rate * 2 ) );
	#else
		uint32_t usartdiv = ( SystemCoreClock / baud_rate );
	#endif

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

	GPIOA->CRL &= ~( GPIO_CRL_CNF2 | GPIO_CRL_MODE2 );

	/* Enable alternative function output push-pull */
	GPIOA->CRL |= ( ( 0x02 << GPIO_CRL_CNF2_Pos ) | ( 0x01 << GPIO_CRL_MODE2_Pos ) );

	/* Enable USART2 */
	USART2->CR1 |= USART_CR1_UE;

	/* Set manually baud rate */
	USART2->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) | ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	/* Enable transmission */
	USART2->CR1 |= USART_CR1_TE;
}
#endif /* DEBUG */


void uart1_init( const uint32_t baud_rate )
{
	/**
	 * NOTE: USART1 placed on APB2 bus, this bus used SYSCLK clock speed without any prescaler.
	 */
	uint32_t usartdiv = ( SystemCoreClock / baud_rate );

	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Clear PA10 (RX) and PA9 (TX) I/Os*/
	GPIOA->CRH &= ~( GPIO_CRH_CNF9 | GPIO_CRH_MODE9 );
	GPIOA->CRH &= ~( GPIO_CRH_CNF10 | GPIO_CRH_MODE10 );

	/**
	 * GPIO Configuration
	 * PA10: Input Floating
	 * PA9:  Alternate function push-pull
	 */
	GPIOA->CRH |= ( ( 0x02 << GPIO_CRH_CNF9_Pos ) | ( 0x01 << GPIO_CRH_MODE9_Pos ) );
	GPIOA->CRH |= ( 0x01 << GPIO_CRH_CNF10_Pos );


	/**
	 * USART1 Configuration
	 * Set 1 Start bit, 8 Data Bit, STOP zero
	 */
	USART1->CR1 = 0x00U;
	USART1->CR1 &= ~( USART_CR1_M );
	USART1->CR2 &= ~( USART_CR2_STOP );

	/* Set baudrate */
	USART1->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) | ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	/* Enable transmission and receiver */
	USART3->CR1 |= ( USART_CR1_RE | USART_CR1_TE );

	/* Interrupt enable */
	USART3->CR1 |= USART_CR1_IDLEIE;

	USART3->CR1 |= ( USART_CR1_UE );
}

void uart3_init( const uint32_t baud_rate )
{
	#ifdef HSE_CLOCK_SPEED
		uint32_t usartdiv = ( SystemCoreClock / ( baud_rate * 2 ) );
	#else
		uint32_t usartdiv = ( SystemCoreClock / baud_rate );
	#endif

	/* Enable USART3 clock */
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	/* Clear CNF & MODE bits PA10 */
	GPIOB->CRH &= ~( GPIO_CRH_CNF11 | GPIO_CRH_MODE11 );

	/* Set PB11 to input floating */
	GPIOB->CRH |= ( 0x01 << GPIO_CRH_CNF11_Pos );

	USART3->CR1 = 0x00U;

	USART3->CR1 &= ~( USART_CR1_M );
	USART3->CR2 &= ~( USART_CR2_STOP );

	USART3->CR3 |= USART_CR3_DMAR;

	/* Set manually baud rate */
	USART3->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) | ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	USART3->CR1 |= USART_CR1_RE;

	/* Interrupt enable */
	USART3->CR1 |= USART_CR1_IDLEIE;

	USART3->CR1 |= ( USART_CR1_UE );
}