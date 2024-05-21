#include "i2c.h"



#define I2C0_SCL 										24
#define I2C0_SDA 										25
#define I2C0_ALT_MUX 								5
#define I2C0_CLK_EN 								(1 << 6)
#define SIM_SCGC5_PORTE_EN 					(1 << 13)

#define I2C_TX_EN (1 << 4)
#define I2C_MST_EN (1 << 5)

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
	I2C0->C1 |=I2C_TX_EN;
	// start
	I2C0->C1 |=I2C_MST_EN;
	// send address -> write
	I2C0->D = (address << 1) & (0xFE);   
	//Wait for Transfer to complete 
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
	// ack from receiver
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}

	// Sending Register Address of Magnetometer that we want to read
	I2C0->D = reg_address;
	//Wait for Transfer to complete 
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	// Send the Repeated Start
	I2C0->C1 |= I2C_C1_RSTA_MASK;
	// addr + read
	I2C0->D = (address << 1) | (0x01);
			
	I2C0->C1 |= (I2C_C1_TXAK_MASK);


	//Wait for Transfer to complete i.e.
    while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
    //Clear IICIF Flag
	I2C0->S|= I2C_S_IICIF_MASK;
	//Waiting for Acknowledgement from slave
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	//Set the I2C in Receiver Mode
	I2C0->C1 &= (~I2C_C1_TX_MASK);
	//Read Dummy Data
	temp = I2C0->D;
	//Wait for Transfer to complete
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	//Clear IICIF Flag
	I2C0->S |= I2C_S_IICIF_MASK;
	//Read Data
	data= I2C0->D;
	//delay to ensure all processes are completed before next process starts
			
	I2C0->C1 &= ~(I2C_C1_TXAK_MASK);
		
	// stop
	I2C0->C1 &= (~I2C_C1_MST_MASK);
	//Return Data
	return data;
}

void i2c_read_multiple_byte(unsigned char address, unsigned char reg_adddress, int n, unsigned char data[]){
		unsigned char temp = 0;
		I2C0->C1 |= I2C_TX_EN;
	
		I2C0->C1 |=I2C_MST_EN;
		
		I2C0->D = (address << 1 ) & (0xFE);
		while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
		//Clear IICIF Flag
		I2C0->S|= I2C_S_IICIF_MASK;
		//Waiting for Acknowledgement from slave
		while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
			
		I2C0->D = reg_adddress;
		while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	
		I2C0->S|= I2C_S_IICIF_MASK;
		while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
		I2C0->C1|=I2C_C1_RSTA_MASK;
		
		I2C0->D= (address << 1 ) | (0x01);
		
			while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
    //Clear IICIF Flag
		I2C0->S|= I2C_S_IICIF_MASK;
		//Waiting for Acknowledgement from slave
		while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
			{}
		//Set the I2C in Receiver Mode
		I2C0->C1&=(~I2C_C1_TX_MASK);
		//Read Dummy  Data
		temp= I2C0->D;
				
		for(uint8_t i = 0; i < n; i++){
			if(i  < (n - 2)){
				 while((I2C0->S & I2C_S_IICIF_MASK) == 0)
						{}
				 I2C0->S|= I2C_S_IICIF_MASK;
				data[i] = I2C0->D;
			}else{
					I2C0->C1|=(I2C_C1_TXAK_MASK);
				while((I2C0->S & I2C_S_IICIF_MASK) == 0)
							{}
					data[i]=I2C0->D;
					i = i+1;
					//Wait for Transfer to complete 
					while((I2C0->S & I2C_S_IICIF_MASK) == 0)
						{}
					//Clear IICIF Flag
					I2C0->S|= I2C_S_IICIF_MASK;
					// Send the stop signal
					I2C0->C1&=(~I2C_C1_MST_MASK);
					// Clear Transmit Nack by setting TXAK to 0
					I2C0->C1&=(~I2C_C1_TXAK_MASK);
					// Finally read the last byte in the I2C data register
					data[i]=I2C0->D;
			}
		}

}

void i2c_write_single_byte(unsigned char address, unsigned char reg_address, unsigned char data){
		I2C0->C1 |= I2C_TX_EN;
	
		I2C0->C1 |=I2C_MST_EN;
	I2C0->D= (address << 1) & (0xFE);
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	I2C0->S|= I2C_S_IICIF_MASK;
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	I2C0->D=reg_address;
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	I2C0->S|= I2C_S_IICIF_MASK;
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	I2C0->D=data;
    while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	I2C0->S|= I2C_S_IICIF_MASK;
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
			{}
	I2C0->C1&=(~I2C_C1_MST_MASK);
	
}
void i2c_write_multiple_byte(unsigned char address, unsigned char reg_address, int n, unsigned char data[]){
		I2C0->C1 |= I2C_TX_EN;
	
		I2C0->C1 |=I2C_MST_EN;
	I2C0->D= (address << 1 ) & (0xFE);
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	I2C0->S|= I2C_S_IICIF_MASK;
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	I2C0->D=reg_address;
	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
		{}
	I2C0->S|= I2C_S_IICIF_MASK;
	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
		{}
	for(uint8_t i = 0;i < n;i++)
		{
		I2C0->D=data[i];
    	while((I2C0->S & I2C_S_IICIF_MASK) == 0)
			{}
    	I2C0->S|= I2C_S_IICIF_MASK;
    	while ((I2C0->S & I2C_S_RXAK_MASK) != 0)
			{}
		}
	I2C0->C1&=(~I2C_C1_MST_MASK);
}