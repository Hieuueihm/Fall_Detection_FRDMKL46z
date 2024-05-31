#ifndef SYSTICK_H
#define SYSTICK_H

#include "MKL46Z4.h"

void systick_init(void);
void delay_ms(uint32_t tick);
void timer_init(void);


#endif