#include "flash.h"

void FLASH_ConfigWaitState( uint32_t wate_state )
{
	FLASH->ACR &= ~( FLASH_ACR_HLFCYA | FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY );

	FLASH->ACR |= FLASH_ACR_PRFTBE;
	FLASH->ACR |= ( wate_state << FLASH_ACR_LATENCY_Pos );
}