#include "switch.h"

#define SWITCH_1_PIN 3
#define SWITCH_2_PIN 12

#define SIM_SCGC5_PORTC_EN (1 << 11)
#define PCR_GPIO (1 << 8)
#define PS_PU_EN (0x1u)
#define PE_EN (0x2u)

void switch_init()
{
	// enable port C
	// enable pin x gpio
	// pull enable and pull select -> pull up
	// INTERRUPT IN FALLING EDGE

	SIM->SCGC5 |= SIM_SCGC5_PORTC_EN;

	PORTC->PCR[SWITCH_1_PIN] |= PCR_GPIO;
	PORTC->PCR[SWITCH_1_PIN] |= PS_PU_EN;
	PORTC->PCR[SWITCH_1_PIN] |= PE_EN;
	PORTC->PCR[SWITCH_1_PIN] |= PORT_PCR_IRQC(0xA); // falling edge 

	PTC->PDDR &= ~((uint32_t)(1 << SWITCH_1_PIN)); // clear pddr -> input mode

	PORTC->PCR[SWITCH_2_PIN] |= PCR_GPIO;
	PORTC->PCR[SWITCH_2_PIN] |= PS_PU_EN;
	PORTC->PCR[SWITCH_2_PIN] |= PE_EN;

	PORTC->PCR[SWITCH_2_PIN] |= PORT_PCR_IRQC(0xA);

	PTC->PDDR &= ~((uint32_t)(1 << SWITCH_2_PIN));

	NVIC_ClearPendingIRQ(PORTC_PORTD_IRQn);
	NVIC_SetPriority(PORTC_PORTD_IRQn, 0); 
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}
