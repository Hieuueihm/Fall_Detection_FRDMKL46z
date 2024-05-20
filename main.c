#include "led.h"
#include "switch.h"
#include "systick.h"
#include "uart.h"
#include "clock.h"
#include "lcd.h"


uint8_t main_status = 0; // 0 off - 1 on





int main(void){
	
	mcg_clock_config();
	systick_init();
	switch_init();
	uart_init();
	led_init(RED_LED);
	led_init(GREEN_LED);
	
	lcd_init();

	while(1){
	if(main_status == 0){
			led_off(RED_LED);
			led_off(GREEN_LED);
			lcd_display_digit(NUM_1);
			delay_ms(100);
			
		}else if(main_status == 1){
			led_toggle(GREEN_LED);
		lcd_display_digit(NUM_0);

			delay_ms(1000);
		}
	 
	}
}


void PORTC_PORTD_IRQHandler(){
		if((PTC -> PDIR & (1 << 3)) == 0){
			main_status = main_status == 0 ? 1 : 0;
		}
		// clear interrupt
		PORTC->PCR[3] |= (1 << 24);
}

/*
	
	

*/