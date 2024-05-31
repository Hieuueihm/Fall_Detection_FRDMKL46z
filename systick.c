#include "systick.h"

volatile uint32_t  counter = 0;
int32_t volatile msTicks = 0; // Interval counter in ms
void SysTick_Handler (void) { // SysTick interrupt Handler
	counter++;
	msTicks++; // Increment counter
}
void systick_init(){
	SysTick->LOAD = SystemCoreClock / 1000; 
	SysTick->VAL = 0; /* Load the SysTick Counter Value */

	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_ENABLE_Msk;
	
	
	
	NVIC_ClearPendingIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, (1 << 4) - 1);
	NVIC_EnableIRQ(SysTick_IRQn);
	

		
}


void delay_ms(uint32_t tick){
	while (msTicks < tick); // Wait 500ms
	msTicks = 0; // Reset counter
}
