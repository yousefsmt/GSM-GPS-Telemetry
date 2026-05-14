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

		GPIOA->CRL &= ~( GPIO_CRL_CNF2 | GPIO_CRL_MODE2 |
						GPIO_CRL_CNF3 | GPIO_CRL_MODE3  );

		/* Enable alternative function output push-pull */
		GPIOA->CRL |= ( ( 0x02 << GPIO_CRL_CNF2_Pos ) | ( 0x01 << GPIO_CRL_MODE2_Pos ) );

		/* Enable USART2 */
		USART2->CR1 |= USART_CR1_UE;

		/* Set manually baud rate */
		USART2->BRR |= ( ( ( usartdiv / 16 ) << USART_BRR_DIV_Mantissa_Pos ) |
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

/* USART1 for neo-6m */