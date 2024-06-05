#ifndef I2C_H
#define I2C_H

#include "MKL46Z4.h"
#include "systick.h"
#include "uart.h"

/*  direction=write : start+device_write;cmdbuff;xBuff; */
/*  direction=recive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */

void i2c_init(void);
unsigned char i2c_read_single_byte(unsigned char address, unsigned char reg_address);
void i2c_read_multiple_byte(unsigned char address, unsigned char reg_adddress, int n, unsigned char data[]);
void i2c_write_single_byte(unsigned char address, unsigned char reg_address, unsigned char data);
void i2c_write_multiple_byte(unsigned char address, unsigned char reg_address, int n, unsigned char data[]);

#endif