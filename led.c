#include "LED.h"

#define RED_LED_PIN (1 << 29)
#define GREEN_LED_PIN (1 << 5)

#define SIM_SCGC5_PORTE_EN (1 << 13)
#define SIM_SCGC5_PORTD_EN (1 << 12)
#define PCR_GPIO (1 << 8)

void led_init(uint8_t led)
{
	if (led == GREEN_LED)
	{
		SIM->SCGC5 |= SIM_SCGC5_PORTD_EN; // This enables clock to port D
		PORTD->PCR[5] |= PCR_GPIO;		  // This sets the Mux control of PTD5 to 001, or GPIO
		PTD->PDDR |= GREEN_LED_PIN;		  // This sets PTD5 as an output.
		PTD->PSOR |= GREEN_LED_PIN;   // clear led
	}
	else if (led == RED_LED)
	{
		SIM->SCGC5 |= SIM_SCGC5_PORTE_EN; // This enables clock to port E
		PORTE->PCR[29] |= PCR_GPIO;		  // SET GPIO FOR PTD26
		PTE->PDDR |= RED_LED_PIN;
		PTE->PSOR |= RED_LED_PIN;
	}
}
void led_on(uint8_t led)
{
	if (led == GREEN_LED)
	{
		PTD->PCOR |= GREEN_LED_PIN;
	}
	else if (led == RED_LED)
	{
		PTE->PCOR |= RED_LED_PIN;
	}
}
void led_off(uint8_t led)
{
	if (led == GREEN_LED)
	{
		PTD->PSOR |= GREEN_LED_PIN;
	}
	else if (led == RED_LED)
	{
		PTE->PSOR |= RED_LED_PIN;
	}
}
void led_toggle(uint8_t led)
{
	if (led == GREEN_LED)
	{
		PTD->PTOR |= GREEN_LED_PIN;
	}
	else if (led == RED_LED)
	{
		PTE->PTOR |= RED_LED_PIN;
	}
}
