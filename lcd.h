#ifndef LCD_H
#define LCD_H

#include "MKL46Z4.h"
#include "clock.h"

#define NUM_1 1
#define NUM_0 0

void lcd_init(void);
void lcd_display_digit(uint8_t NUM) ;

void lcd_clear(void);

#endif