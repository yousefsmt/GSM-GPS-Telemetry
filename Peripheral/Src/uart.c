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

		uint32_t usartdiv = ( SystemCoreClock / baud_rate );

		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

		GPIOA->CRL &= ~( GPIO_CRL_CNF2 | GPIO_CRL_MODE2 );

		/* Enable alternative function output push-pull */
		GPIOA->CRL |= ( ( 0x02 << GPIO_CRL_CNF2_Pos ) | ( 0x01 << GPIO_CRL_MODE2_Pos ) );

		/* Enable USART2 */
		USART2->CR1 |= USART_CR1_UE;

		/* Set manually baud rate */
		USART2->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
						( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );
		/* Enable transmission */
		USART2->CR1 |= USART_CR1_TE;
	}
#endif /* DEBUG */

/**
 * Implement USART1 for receive NMEA data from NEO-6M, then prased it and send via SMS ( SIM800L )
 */
void uart1_init( const uint32_t baud_rate )
{
	/**
	 * neo-6m communication
	 * 1: Enable USART1 Clock
	 * 2: Enable GPIOA Pin 10 input floating
	 * 4: Set baud rate
	 * 5: Enable receiver
	 *
	 * TEST: For debug release must check DMA USAT1->DR to USART2->DR for transmit NMEA data to printf
	 * TEST: Test-1 parse message with DMA
	 * TEST: Test-2 parse message with ring buffer on stack
	 *
	 * TODO: After add RTOS, send NMEA parsed data via SIM800L SMS.
	 */
	uint32_t usartdiv = ( SystemCoreClock / baud_rate );

	/* Enable USART1 clock */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Clear CNF & MODE bits PA10 */
	GPIOA->CRH &= ~( GPIO_CRH_CNF10 | GPIO_CRH_MODE10 );

	/* Set PA10 to input floating */
	GPIOA->CRH |= ( 0x01 << GPIO_CRH_CNF10_Pos );

	USART1->CR1 = 0x00U;

	USART1->CR1 &= ~( USART_CR1_M );
	USART1->CR2 &= ~( USART_CR2_STOP );

	/**
	 * *** TEST-1 ***
	 * I want compare two methods for USART receive message, which one faster? which one efficient?
	 * 1: In ISR check IDEAL line then store 8 bit to circular buffer with DMA.
	 * 2: In ISR check IDEAL line then store 8 bit to implemented ring buffer in Common directory then parse it on main
	 */
	#ifdef TEST_DMA
		USART1->CR3 |= USART_CR3_DMAR;
	#endif /* TEST_DMA */

	/* Set manually baud rate */
	USART1->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
				    ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	USART1->CR1 |= USART_CR1_RE;

	/* Interrupt enable */
	USART1->CR1 |= USART_CR1_RXNEIE;

	USART1->CR1 |= USART_CR1_IDLEIE;

	USART1->CR1 |= USART_CR1_UE;
}

/**
 * Implement UART3 for SIM800L
 */
void uart3_init( const uint32_t baud_rate )
{
	/**
	 * SIM800L communication
	 * 1: Enable USART3 Clock
	 * 2: Enable GPIOA Pin 11 input floating
	 * 3: Enable GPIOA Pin 10 alternate function push-pull
	 * TEST: For debug release must check DMA USART3->DR to USART3->DR for transmit and receive AT commands
	 * TODO: After add RTOS, send NMEA parsed data via SIM800L SMS.
	 */
	uint32_t usartdiv = ( SystemCoreClock / baud_rate );

	/* Enable USART3 clock */
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	/* Clear CNF & MODE bits PA10 */
	GPIOB->CRH &= ~( GPIO_CRH_CNF10 | GPIO_CRH_MODE10 |
	                 GPIO_CRH_CNF11 | GPIO_CRH_MODE11  );

	/* Set PB11 to input floating */
	GPIOB->CRH |= ( 0x01 << GPIO_CRH_CNF11_Pos );

	/* Set PB10 to alternate function push-pull */
	GPIOB->CRH |= ( ( 0x01 << GPIO_CRH_MODE10_Pos ) | ( 0x02 << GPIO_CRH_CNF10_Pos ) );

	USART3->CR1 = 0x00U;

	USART3->CR1 &= ~( USART_CR1_M );
	USART3->CR2 &= ~( USART_CR2_STOP );

	#ifdef TEST_DMA
		USART3->CR3 |= USART_CR3_DMAR;
	#endif /* TEST_DMA */

	/* Set manually baud rate */
	USART3->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
				    ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	USART3->CR1 |= USART_CR1_RE;

	/* Interrupt enable */
	USART3->CR1 |= ( USART_CR1_RXNEIE | USART_CR1_IDLEIE | USART_CR1_TXEIE | USART_CR1_TCIE);

	USART3->CR1 |= ( USART_CR1_UE | USART_CR1_TE );

}