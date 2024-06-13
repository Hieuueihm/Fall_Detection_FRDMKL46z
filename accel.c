#include "accel.h"
#include "stdio.h"

void accel_init()
{
	// who am i
	unsigned char device_id;
	unsigned char reg_value;
	device_id = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, WHO_AM_I_REG);

	while (device_id != ACCEL_WHO_AM_I)
	{
		uart_send_msg("don't found ACCEL ~");
		device_id = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, WHO_AM_I_REG);
	}

	// config ACCEL;

	// reset && wait reset enable
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2, 0x40); // sofeware reset
	reg_value = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2) & 0x40;
	while (reg_value)
	{
		reg_value = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2) & 0x40;
	}

	// 0b0010000
	// 0b00011010
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG1, 0x1A);		 // ODR = 100, standby
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, XYZ_DATA_CFG_REG, 0x02); // default 8g ->
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG2, 0x00);		 // Normal Mode

	// 0b00111000
	// enable freefall

	accel_config(FREE_FALL, 6, 3);

	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG4, 0x04); // Enable Motion/Freefall Interrupt
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG5, 0x04); // Freefall interrupt routed to INT1 - PTC5
	// start
	// 0b00100001
	i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, CTRL_REG1, 0x1B);

	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	// Configure PORTA pin 1 as GPIO
	PORTC->PCR[5] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[5] |= PORT_PCR_MUX(1);

	// Configure PORTA pin 1 interrupt on falling edge
	PORTC->PCR[5] &= ~PORT_PCR_IRQC_MASK;	// Clear IRQC field
	PORTC->PCR[5] |= PORT_PCR_IRQC(0b1010); // Set IRQC to falling edge interrupt

	NVIC_SetPriority(PORTC_PORTD_IRQn, 0);
	NVIC_EnableIRQ(PORTC_PORTD_IRQn);
}

void accel_config(uint8_t MODE, uint8_t THRESHOLD, uint8_t DEBOUNCE)
{
	switch (MODE)
	{
	case FREE_FALL:

		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_CFG_REG, 0x38);
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_THS_REG, THRESHOLD);
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_COUNT_REG, DEBOUNCE);
		break;
	case MOTION_DETECTION:
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_CFG_REG, 0x78);
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_THS_REG, THRESHOLD);
		i2c_write_single_byte(ACCEL_DEVICE_ADDRESS, FF_MT_COUNT_REG, DEBOUNCE);
		break;
	}
}