#ifndef LED_H
#define LED_H

#include "MKL46Z4.h"

#define GREEN_LED 0
#define RED_LED 1

void led_init(uint8_t led);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);

#endif
