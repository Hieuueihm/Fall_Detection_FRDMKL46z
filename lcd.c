#include "lcd.h"
#include "led.h"

#define _LCDBLINKRATE     (3)
#define _LCDRVEN          (1)  //
#define _LCDRVTRIM        (8)           // CPSEL = 1     0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf
#define _LCDCPSEL         (1)          //  charge pump select 0 or 1 
#define _LCDLOADADJUST    (3)           // CPSEL = 1     0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf
#define _LCDALTDIV        (0)           // CPSEL = 1     0 -- 8000 pf 1 -- 6000 pf  2 -- 4000 pf  3 -- 2000 pf
#define _LCDALRCLKSOURCE  (0)         // 0 -- External clock       1 --  Alternate clock

#define _LCDCLKPSL        (0)         //  Clock divider to generate the LCD Waveforms 
#define _LCDSUPPLY        (1) 
#define _LCDHREF          (0)          // 0 or 1 
#define _LCDCLKSOURCE     (1)         // 0 -- External clock       1 --  Alternate clock
#define _LCDLCK           (1)         //Any number between 0 and 7 
#define _LCDBLINKRATE     (3) 

uint8_t waveForm = 0;

static void lpo_source_init(){
		SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK; 

			if ((SIM->SOPT1 & SIM_SOPT1_OSC32KSEL_MASK) == 0) {
					SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(3); 
			}

	}
#define SLCD_WAVEFORM_CONFIG_NUM 16

typedef enum _slcd_phase_type
{
    kSLCD_PhaseAActivate = 0x01U,  /*!< LCD waveform phase A activates. */
    kSLCD_PhaseBActivate = 0x02U,  /*!< LCD waveform phase B activates. */
    kSLCD_PhaseCActivate = 0x04U,  /*!< LCD waveform phase C activates. */
    kSLCD_PhaseDActivate = 0x08U,  /*!< LCD waveform phase D activates. */
} slcd_phase_type_t;
void lcd_set_backplane_phase(uint32_t pinIndex, uint32_t phase) {
      LCD->WF8B[pinIndex] = phase;

}

void lcd_set_frontplane_segments(uint32_t pinIndex, uint32_t phase) {
      LCD->WF8B[pinIndex] = phase;

}


void lcd_init(){
		
		lpo_source_init();
		SIM->SCGC5 |= SIM_SCGC5_SLCD_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	
			
		LCD->GCR = 0; 
		LCD->AR = 0;
	  LCD->GCR =  (   LCD_GCR_RVEN_MASK*_LCDRVEN  
                   | LCD_GCR_RVTRIM(_LCDRVTRIM)    //0-15
                   | LCD_GCR_CPSEL_MASK*_LCDCPSEL 
                   | LCD_GCR_LADJ(_LCDLOADADJUST)     //0-3*/
                   | LCD_GCR_VSUPPLY_MASK*_LCDSUPPLY  //0-1*/
                   |!LCD_GCR_FDCIEN_MASK
                   | LCD_GCR_ALTDIV(_LCDALTDIV)  //0-3
                   |!LCD_GCR_LCDDOZE_MASK  
                   |!LCD_GCR_LCDSTP_MASK
                   |!LCD_GCR_LCDEN_MASK    //WILL BE ENABLE ON SUBSEQUENT STEP
                   | LCD_GCR_SOURCE_MASK*_LCDCLKSOURCE
                   | LCD_GCR_ALTSOURCE_MASK*_LCDALRCLKSOURCE  
                   | LCD_GCR_LCLK(_LCDLCK)   //0-7
                   | LCD_GCR_DUTY(3)   //0-7
                 );   
		
		LCD->PEN[0] =  0x000e0d80U;
		LCD->PEN[1] =0x00300160U;
	
		LCD->BPEN[0] = 0x000c0000U;
		LCD->BPEN[1] = 0x00100100U;
	
	
		LCD->AR  = LCD_AR_BRATE(_LCDBLINKRATE);
		
		LCD->GCR |=	LCD_GCR_LCDEN_MASK;    // LCD Driver Enable

	
	
	}

void lcd_display_digit(uint8_t NUM) {
			LCD->AR &= ~(1 << 5);

		if(NUM == NUM_0){
			waveForm = (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate );
			
			lcd_set_frontplane_segments(37, 0);
			lcd_set_frontplane_segments(17, 0); // LCD_P17


			lcd_set_backplane_phase(40, kSLCD_PhaseAActivate); // LCD_P40
			lcd_set_backplane_phase(52, kSLCD_PhaseBActivate); // LCD_P52
			lcd_set_backplane_phase(19, kSLCD_PhaseCActivate); // LCD_P19
			lcd_set_backplane_phase(18, kSLCD_PhaseDActivate); // LCD_P18

			// Set frontplane segments
			lcd_set_frontplane_segments(37, kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseDActivate); // LCD_P37
			lcd_set_frontplane_segments(17, kSLCD_PhaseDActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate); // LCD_P17
   
		}else if(NUM == NUM_1){
			waveForm = (kSLCD_PhaseAActivate | kSLCD_PhaseBActivate | kSLCD_PhaseCActivate | kSLCD_PhaseDActivate );
			lcd_set_frontplane_segments(37, 0);
			lcd_set_frontplane_segments(17, 0);

			lcd_set_backplane_phase(40, kSLCD_PhaseAActivate); // LCD_P40
			lcd_set_backplane_phase(52, kSLCD_PhaseBActivate); // LCD_P52
			lcd_set_backplane_phase(19, kSLCD_PhaseCActivate); // LCD_P19
			lcd_set_backplane_phase(18, kSLCD_PhaseDActivate); // LCD_P18

			// Set frontplane segments
			lcd_set_frontplane_segments(37, kSLCD_PhaseBActivate | kSLCD_PhaseDActivate); // LCD_P37
			
		}
	  
		

}

void lcd_clear(){
	

		LCD->AR |= (1 << 5);

	
}
