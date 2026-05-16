#include "dma.h"


/**
 * NOTE: After ISR must be set 1 to IFCR each interrupt register (TEI, HTI, TCI)
 */
void dma_init( char *buffer, const uint32_t buffer_size)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel5->CCR = 0x00U; /* Clear control register */

	DMA1_Channel5->CPAR  = ( uint32_t )&USART1->DR; /* Peripheral address */
	DMA1_Channel5->CMAR  = ( uint32_t )buffer;  /* Memory Address */
	DMA1_Channel5->CNDTR = buffer_size;

	/**
	 * Enable interrupt for three types of DMA errors
	 */
	DMA1_Channel5->CCR |= DMA_CCR_TEIE; /* Enable transfer error */
	DMA1_Channel5->CCR |= DMA_CCR_HTIE; /* Enable half of the bytes */
	// DMA1_Channel5->CCR |= DMA_CCR_TCIE; /* Enable transfer complete */

	/**
	 * Set peripheral and buffer size.
	 */
	DMA1_Channel5->CCR &= ~( DMA_CCR_MSIZE ); /* Data buffer store each charecter 1 Byte 8 Bit */
	DMA1_Channel5->CCR &= ~( DMA_CCR_PSIZE ); /* Pripheral size is 8 Bit or 1 Byte */

	/**
	 * Configuration DMA procedure
	 */
	DMA1_Channel5->CCR &= ~( DMA_CCR_DIR );           /* Read from peripheral */
	DMA1_Channel5->CCR |= DMA_CCR_MINC;               /* Set data memory 8 bit */
	DMA1_Channel5->CCR |= ( 0x03 << DMA_CCR_PL_Pos ); /* Priority level set very high */
	DMA1_Channel5->CCR |= DMA_CCR_CIRC;               /* Circular mode enable */
	DMA1_Channel5->CCR |= DMA_CCR_EN;                 /* Enable channel */
}

void DMA1_Channel5_IRQHandler( void )
{
	/* Transfer error occurred */
	if( ( DMA1->ISR & DMA_ISR_TEIF5 ) )
	{
		DMA1->IFCR |= DMA_IFCR_CTEIF5;
	}

	/* Transfer complete */
	if( ( DMA1->ISR & DMA_ISR_TCIF5 ) )
	{
		DMA1->IFCR |= DMA_IFCR_CTCIF5;
	}

	/* Half of the bytes transfer */
	if( ( DMA1->ISR & DMA_ISR_HTIF5 ) )
	{
		DMA1->IFCR |= DMA_IFCR_CHTIF5;
	}
}