#include "dma.h"

/**
 * NOTE: After ISR must be set 1 to IFCR each interrupt register (TEI, HTI, TCI)
 */
void dma1_init( uint8_t *buffer, const uint32_t buffer_size)
{
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel3->CCR = 0x00U; /* Clear control register */

	DMA1_Channel3->CPAR  = ( uint32_t )&USART3->DR; /* Peripheral address */
	DMA1_Channel3->CMAR  = ( uint32_t )buffer;  /* Memory Address */
	DMA1_Channel3->CNDTR = buffer_size;
	/**
	 * Set peripheral and buffer size.
	 */
	DMA1_Channel3->CCR &= ~( DMA_CCR_MEM2MEM );
	DMA1_Channel3->CCR &= ~( DMA_CCR_MSIZE ); /* Data buffer store each charecter 1 Byte 8 Bit */
	DMA1_Channel3->CCR &= ~( DMA_CCR_PSIZE ); /* Pripheral size is 8 Bit or 1 Byte */
	DMA1_Channel3->CCR &= ~( DMA_CCR_PINC );
	DMA1_Channel3->CCR |= DMA_CCR_CIRC;               /* Circular mode enable */
	DMA1_Channel3->CCR &= ~( DMA_CCR_DIR );           /* Read from peripheral */
	DMA1_Channel3->CCR |= DMA_CCR_MINC;               /* Set data memory 8 bit */
	DMA1_Channel3->CCR |= ( 0x03 << DMA_CCR_PL_Pos ); /* Priority level set very high */
	DMA1_Channel3->CCR |= DMA_CCR_EN;                 /* Enable channel */
}

// void DMA1_Channel3_IRQHandler( void )
// {
// 	/* Transfer error occurred */
// 	if( ( DMA1->ISR & DMA_ISR_TEIF5 ) )
// 	{
// 		DMA1->IFCR |= DMA_IFCR_CTEIF5;
// 	}

// 	/* Transfer complete ( Don't need for circular mode )*/
// 	// if( ( DMA1->ISR & DMA_ISR_TCIF5 ) )
// 	// {
// 	// 	DMA1->IFCR |= DMA_IFCR_CTCIF5;
// 	// }

// 	/* Half of the bytes transfer */
// 	if( ( DMA1->ISR & DMA_ISR_HTIF5 ) )
// 	{
// 		DMA1->IFCR |= DMA_IFCR_CHTIF5;
// 	}
// }