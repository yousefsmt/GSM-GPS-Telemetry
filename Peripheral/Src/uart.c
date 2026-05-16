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
		 * 4:
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

	/**
	 * Implement putchar for printf syscall
	 * to send character instead standard I/O Linux to usart2
	 */
	int __io_putchar( int ch )
	{
		while ( ( USART2->SR & USART_SR_TXE ) == 0 ) {}
		USART2->DR = ch;
		return ch;
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
	 * TEST: For debug release must check DMA USAT1->DR to USART2->DR for transmit NMEA data to printf
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
	USART1->CR3 |= USART_CR3_DMAR;

	/* Set manually baud rate */
	USART1->BRR = ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
				    ( ( usartdiv % 16 ) << USART_BRR_DIV_Fraction_Pos ) );

	USART1->CR1 |= USART_CR1_RE;

	/* Interrupt enable */
	USART1->CR1 |= USART_CR1_RXNEIE;

	USART1->CR1 |= USART_CR1_IDLEIE;

	USART1->CR1 |= USART_CR1_UE;
}
uint32_t counter = 0x00;
void USART1_IRQHandler( void )
{
    if ( USART1->SR & USART_SR_RXNE )
    {
        uint8_t data = USART1->DR;

		( void )data;
    }

	if ( ( USART1->SR & USART_SR_IDLE ) )
	{
		counter = 0x01U;
	}
	else
	{
		counter = 0x00U;
	}
}