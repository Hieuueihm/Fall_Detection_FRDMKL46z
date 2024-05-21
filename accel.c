#include "accel.h"
#include "stdio.h"
#define ACC_DEVICE_ADDRESS                	0x1D
#define ACC_DEVICE_ID_REGISTER_ADDRESS     	0x0D
#define XYZ_CFG 						   							0x0E
#define ACC_CTRL_REG1                      	0x2A
#define ACC_OUT_X_MSB                      	0x01


void accel_init(){
	unsigned char ACC_DEVICE_ID;
	ACC_DEVICE_ID = i2c_read_single_byte(ACC_DEVICE_ADDRESS, ACC_DEVICE_ID_REGISTER_ADDRESS);

	while(ACC_DEVICE_ID != 0x1A){
		uart_send_msg("err");
	}


}