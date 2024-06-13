#include "lcd.h"
#include "led.h"

#define GCR_RVEN_EN (1 << 31)
#define GCR_CPSEL_EN (1 << 23)
#define GCR_VSUPLLY_EN (1 << 17)
#define GCR_SOURCE_ALT_EN (1 << 6)
#define AR_LCD_BLANK_DISPLAY_EN (1 << 5)
#define _LCDRVTRIM (8)	   // CPSEL = 1     0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf
#define _LCDALTDIV (0)	   // div 1
#define _LCDLOADADJUST (3) //

#define _LCDBLINKRATE (3)
#define _LCDRVEN (1)		 //
#define _LCDCPSEL (1)		 //  charge pump select 0 or 1
#define _LCDALRCLKSOURCE (0) // 0 -- External clock       1 --  Alternate clock

#define _LCDCLKPSL (0) //  Clock divider to generate the LCD Waveforms
#define _LCDSUPPLY (1)
#define _LCDHREF (0)	  // 0 or 1
#define _LCDCLKSOURCE (1) // 0 -- External clock       1 --  Alternate clock
#define _LCDLCK (4)		  // Any number between 0 and 7
#define _LCDBLINKRATE (3)

#define SLCD_WAVEFORM_CONFIG_NUM 16

typedef enum _slcd_phase_type
{
	kSLCD_PhaseAActivate = 0x01U, /*!< LCD waveform phase A activates. */
	kSLCD_PhaseBActivate = 0x02U, /*!< LCD waveform phase B activates. */
	kSLCD_PhaseCActivate = 0x04U, /*!< LCD waveform phase C activates. */
	kSLCD_PhaseDActivate = 0x08U, /*!< LCD waveform phase D activates. */
} slcd_phase_type_t;
void lcd_set_backplane_phase(uint32_t pinIndex, uint32_t phase)
{
	LCD->WF8B[pinIndex] = phase;
}

void lcd_set_frontplane_segments(uint32_t pinIndex, uint32_t phase)
{
	LCD->WF8B[pinIndex] = phase;
}

void lcd_init()
{

	SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	LCD->GCR = 0;
	LCD->AR = 0;

	// 1 / 4 duty cycle
	LCD->GCR = (
				GCR_SOURCE_ALT_EN |			 // System oscillator (OSC32KCLK) default 32.768 kHz
				LCD_GCR_LCLK(_LCDLCK) |		 // LCD FRAME RATE : 32 HZ
				LCD_GCR_DUTY(3) // 1 / 4 duty cycle
	);			 // 1 / 4 duty

	LCD->PEN[0] = 0x000e0d80U; /* LCD_P19/P18/P17/P11/P10/P8/P7. */
	LCD->PEN[1] = 0x00300160U; /* LCD_P53/P52/P40/P38/P37. */

	LCD->BPEN[0] = 0x000c0000U; /* LCD_P19/P18 --> b19/b18. */
	LCD->BPEN[1] = 0x00100100U; /* LCD_P52/P40 --> b20/b8. */

	LCD->GCR |= LCD_GCR_LCDEN_MASK; // LCD Driver Enable
}

void lcd_display_digit(uint8_t NUM)
{
	LCD->AR &= ~AR_LCD_BLANK_DISPLAY_EN;

	if (NUM == NUM_0)
	{

		lcd_set_frontplane_segments(37, 0);
		lcd_set_frontplane_segments(17, 0);

		lcd_set_backplane_phase(40, kSLCD_PhaseAActivate); // LCD_P40
		lcd_set_backplane_phase(52, kSLCD_PhaseBActivate); // LCD_P52
		lcd_set_backplane_phase(19, kSLCD_PhaseCActivate); // LCD_P19
		lcd_set_backplane_phase(18, kSLCD_PhaseDActivate); // LCD_P18

		// 0 0 0 0 0 0 0 1
		// 0 0 0 0 0 0 1 0
		// 0 0 0 0 0 1 0 0
		// 0 0 0 0 1 0 0 0
		// Set frontplane segments
		lcd_set_frontplane_segments(37, kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseDActivate); // LCD_P37
		lcd_set_frontplane_segments(17, kSLCD_PhaseDActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate); // LCD_P17

		// SLCD_SetFrontPlaneSegments(LCD, 7, waveForm);  /* SLCD P07 --- LCD_P7. */
		// SLCD_SetFrontPlaneSegments(LCD, 8, waveForm);  /* SLCD P08 --- LCD_P8. */
		// SLCD_SetFrontPlaneSegments(LCD, 53, waveForm); /* SLCD P09 --- LCD_P53. */
		// SLCD_SetFrontPlaneSegments(LCD, 38, waveForm); /* SLCD P10 --- LCD_P38. */
		// SLCD_SetFrontPlaneSegments(LCD, 10, waveForm); /* SLCD P11 --- LCD_P10. */
		// SLCD_SetFrontPlaneSegments(LCD, 11, waveForm); /* SLCD P12 --- LCD_P11. */
	}
	else if (NUM == NUM_1)
	{
		lcd_set_frontplane_segments(37, 0);
		lcd_set_frontplane_segments(17, 0);

		lcd_set_backplane_phase(40, kSLCD_PhaseAActivate); // LCD_P40
		// lcd_set_backplane_phase(52, kSLCD_PhaseBActivate); // LCD_P52
		// lcd_set_backplane_phase(19, kSLCD_PhaseCActivate); // LCD_P19
		lcd_set_backplane_phase(18, kSLCD_PhaseDActivate); // LCD_P18

		// Set frontplane segments
		lcd_set_frontplane_segments(37, kSLCD_PhaseBActivate | kSLCD_PhaseDActivate); // LCD_P37
	}
}

void lcd_clear()
{

	LCD->AR |= AR_LCD_BLANK_DISPLAY_EN;
}
