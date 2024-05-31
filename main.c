#include "led.h"
#include "switch.h"
#include "systick.h"
#include "uart.h"
#include "clock.h"
#include "lcd.h"
#include "i2c.h"
#include "accel.h"
#include <stdio.h>

uint8_t main_status = 0; // 0 off - 1 on

#define STATE_0 0
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3
extern volatile uint32_t counter ;

int8_t convert_twos_complement(uint8_t value);
float convert_to_g(int8_t raw_value, float sensitivity);

uint8_t state = 1;
char buffer[50];
int main(void){
	
	mcg_clock_config();
	systick_init();
	switch_init();
	uart_init();
	
	led_init(RED_LED);
	led_init(GREEN_LED);
	
//led_on(RED_LED);
	
	lcd_init();
	i2c_init();

	accel_init();
	int8_t x, y, z;
		
		
	
		
	while(1){
		
			 x = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x01);
				y= i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x03);
				z = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x05);
		
		
		  x = convert_twos_complement(x);
     y = convert_twos_complement(y);
     z = convert_twos_complement(z);

    // Convert the raw data to g
    float sensitivity = 0.0625; // Sensitivity for ±8g range
    float x_g = convert_to_g(x, sensitivity);
    float y_g = convert_to_g(y, sensitivity);
    float z_g = convert_to_g(z, sensitivity);
		//x = convert_twos_complement(x);
   // y = convert_twos_complement(y);
   // z = convert_twos_complement(z);
		
		snprintf(buffer, sizeof(buffer), "[%.2f, %.2f, %.2f] - ", x_g, y_g, z_g);
		
		uart_send_msg(buffer);
		
		//delay_ms(100);
	

	if(main_status == 0){
			//led_off(RED_LED);
			//led_off(GREEN_LED);
			lcd_display_digit(NUM_1);
		//	delay_ms(100);
			
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
					PORTC->PCR[3] |= (1 << 24);

		}
		if((PTC->PDIR & (1 << 5)) == 0){
			switch(state){
				case STATE_0:
					break;
				case STATE_1:
						state = STATE_2;				
						led_on(RED_LED);
				
			// enable freefall

			// 0b01111xxx
				// set motion
			i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_CFG_REG, 0x78);		
	
				//  * 0.063 
				
			i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_THS_REG, 16);
			// 0b000000
			i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_COUNT_REG, 10); 
					break;
				case STATE_2:
					state = STATE_3;
						led_on(GREEN_LED);
					i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_CFG_REG, 0x38);		
	
		// 16 * 0.06 
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_THS_REG, 20);
		// 0b000000
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_COUNT_REG, 50);   	


					break;
				case STATE_3:
					led_off(RED_LED);
					break;
		


			}
			PORTC->PCR[5] |= (1 << 24);

		}



		// clear interrupt


		
}



/*
	
	

*/

int8_t convert_twos_complement(uint8_t value) {
    // If the highest bit (sign bit) is set, the number is negative
    if (value & 0x80) {
        return -((uint8_t)(~value) + 1);
    } else {
        return value;
    }
}

// Function to convert the raw value to g
float convert_to_g(int8_t raw_value, float sensitivity) {
    return raw_value * sensitivity;
}