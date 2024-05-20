#include "switch.h"


void switch_init(){
	// enable port C
	// enable pin x gpio 
	// pull enable and pull select
	SIM->SCGC5 |= (1 << 11);
	
	PORTC -> PCR[3] |= (1 << 8);
	PORTC-> PCR[3] |= (0x1u);
	PORTC->PCR[3] |= (0x2u);
	PORTC->PCR[3] |= PORT_PCR_IRQC(0xA);
		
	PTC->PDDR &= ~((uint32_t) (1 << 3));

	PORTC -> PCR[12] |= (1 << 8);
	PORTC-> PCR[12] |= (0x1u);
	PORTC->PCR[12] |= (0x2u);
		
	PORTC->PCR[12] |= PORT_PCR_IRQC(0xA);

	PTC->PDDR &= ~((uint32_t) (1 << 12));
	
	
	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);

}

