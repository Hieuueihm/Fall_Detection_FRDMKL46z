#include "i2c.h"



#define I2C0_SCL 										24
#define I2C0_SDA 										25
#define I2C0_ALT_MUX 								5
#define I2C0_CLK_EN 								(1 << 6)
#define SIM_SCGC5_PORTE_EN 					(1 << 13)

#define I2C_TX_EN (1 << 4)
#define I2C_MST_EN (1 << 5)

#define I2C_START() I2C0->C1 |= I2C_TX_EN;\
                    I2C0->C1 |= I2C_MST_EN;

#define I2C_STOP() I2C0->C1 &= ~I2C_MST_EN;


#define I2C_WAIT()        while((I2C0->S & I2C_S_IICIF_MASK)==0) {} \
                           I2C0->S |= I2C_S_IICIF_MASK;

	
#define I2C_DISABLE_ACK()       I2C0->C1 |= I2C_C1_TXAK_MASK
#define I2C_ENABLE_ACK()        I2C0->C1 &= ~I2C_C1_TXAK_MASK
#define I2C_REPEATED_START()    I2C0->C1 |= I2C_C1_RSTA_MASK
#define I2C_RX_MODE()      			I2C0->C1 &= ~I2C_C1_TX_MASK
#define DELAY() 									{for (int i = 0; i < 100; i++){}}
void i2c_init(){
	
	SIM->SCGC4|=SIM_SCGC4_I2C0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_EN;
	
	PORTE->PCR[I2C0_SCL]|= PORT_PCR_MUX(I2C0_ALT_MUX);
	PORTE->PCR[I2C0_SDA]|= PORT_PCR_MUX(I2C0_ALT_MUX);
	
	PORTE->PCR[I2C0_SCL]|= PORT_PCR_PS_MASK |PORT_PCR_PE_MASK;
	PORTE->PCR[I2C0_SDA]|= PORT_PCR_PS_MASK |PORT_PCR_PE_MASK;
	
	I2C0->C1|= I2C_C1_IICEN_MASK;
}


unsigned char i2c_read_single_byte(unsigned char address, unsigned char reg_address){
	
/*  direction=write : start+device_write;cmdbuff;xBuff; */
/*  direction=recive : start+device_write;cmdbuff;repeatStart+device_read;xBuff; */
	unsigned char temp = 0;
	unsigned char data =0;
	I2C_START();
	I2C0->D = (address << 1) & (0xFE);   
	I2C_WAIT();
	I2C0->D = reg_address;
	I2C_WAIT();
	// Send the Repeated Start
	I2C_REPEATED_START();
		// addr + read
	I2C0->D = (address << 1) | (0x01);
			
	I2C_WAIT();
	
	I2C_RX_MODE();
	
	I2C_DISABLE_ACK();
	
	temp = I2C0->D;
	I2C_WAIT();
	I2C_STOP();
	data= I2C0->D;
	
	DELAY();
			
	
	return data;
}

void i2c_read_multiple_byte(unsigned char address, unsigned char reg_adddress, int n, unsigned char data[]){
		uint8_t i = 0;
		unsigned char temp = 0;
		I2C_START();
	// 11111110
		I2C0->D = (address << 1) & (0xFE); // (address << 1) | RW
		I2C_WAIT();
	
		I2C0->D = reg_adddress;
		
		I2C_WAIT();
		I2C0->D = (address << 1) | (0x01);
	
		I2C_WAIT();
		I2C_RX_MODE();
		I2C_ENABLE_ACK();
		
		temp = I2C0->D;
	
		I2C_WAIT();
		
		for(i =0; i < n - 2; i++){
			data[i] = I2C0->D;
			I2C_WAIT();
		}
		I2C_DISABLE_ACK();
		
		data[i++] = I2C0->D;
		I2C_WAIT();
		I2C_STOP();
		data[i] = I2C0->D;
		DELAY();

}

void i2c_write_single_byte(unsigned char address, unsigned char reg_address, unsigned char data){
		I2C_START();
		I2C0->D =  (address << 1 ) & (0xFE);
		I2C_WAIT();
		
		I2C0->D = reg_address;
		I2C_WAIT();
		
		I2C0->D = data;
		I2C_WAIT();
		I2C_STOP();
		DELAY();
}
void i2c_write_multiple_byte(unsigned char address, unsigned char reg_address, int n, unsigned char data[]){
	
	I2C_START();
		I2C0->D= (address << 1 ) & (0xFE);
	I2C_WAIT();
	
	I2C0->D=reg_address;
	I2C_WAIT();
	for(uint8_t i = 0;i < n;i++)
		{
		I2C0->D=data[i];
		I2C_WAIT();
		}
	I2C_STOP();
	DELAY();
}