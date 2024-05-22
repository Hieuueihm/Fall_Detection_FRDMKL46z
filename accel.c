#include "accel.h"
#include "stdio.h"


void accel_init(){
	// who am i
	unsigned char device_id;
	unsigned char reg_value;
	device_id = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, WHO_AM_I_REG);

	while(device_id != ACCEL_WHO_AM_I){
		uart_send_msg("don't found ACCEL ~");
	}
	// config ACCEL;
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2, 0x40); // sofeware reset
	reg_value = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2) & 0x40;
	while(reg_value){
		reg_value = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2) & 0x40;

	}
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG1, 0x18);			// ODR = 100Hz, standby
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, XYZ_DATA_CFG_REG, 0x00); // default 2g -> 4096 and high pass filter disable
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2, 0x02);			// High Resolution mode
	
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_CFG_REG, 0xD8);		
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FT_MT_THS_REG, 0x30);		// Threshold Setting Value for the Freefall detection of  0.2g (4 * 0.063)
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_COUNT_REG, 0x0A);   	

	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG4, 0x04);			// Enable Motion/Freefall Interrupt
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG5, 0x04);			// Freefall interrupt routed to INT1 - PTC6

	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG1, 0x19);			// ODR = 100Hz, Active mode	
	

	
	
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

    // Configure PORTA pin 1 as GPIO
    PORTC->PCR[6] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[6] |= PORT_PCR_MUX(1);

    // Configure PORTA pin 1 interrupt on falling edge
    PORTC->PCR[6] &= ~PORT_PCR_IRQC_MASK;   // Clear IRQC field
    PORTC->PCR[6] |= PORT_PCR_IRQC(0b1010); // Set IRQC to falling edge interrupt

   

	

	

	
	


}