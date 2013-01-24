/**
 * This file is responsible for I2C operations
 * @author Sebastian Laskawiec and Michal Wachowicz
 */

/**
 * I2C port Configuration
 */
#define I2C_PORT_O	I2C_PORT
#define	I2C_PORT_D	DDR(I2C_PORT)
#define I2C_PORT_I	PIN(I2C_PORT)


#define I2C_SDA_WR()	sbi(I2C_PORT_D,I2C_SDA)
#define I2C_SDA_RD()	cbi(I2C_PORT_D,I2C_SDA)
#define I2C_SCL_WR()	sbi(I2C_PORT_D,I2C_SCL)
#define I2C_SCL_RD()	cbi(I2C_PORT_D,I2C_SCL)


#define I2C_SDA_H()	sbi(I2C_PORT_O,I2C_SDA)
#define I2C_SDA_L()	cbi(I2C_PORT_O,I2C_SDA)
#define I2C_SCL_H()	sbi(I2C_PORT_O,I2C_SCL)
#define I2C_SCL_L()	cbi(I2C_PORT_O,I2C_SCL)


/**
 * Reads 8 bits from I2C
 * @param 
 * 		ACK
 * @return
 * 		Read byte
 */ 
u08 I2C_read(u08 ack)
{
	u08 i;
	u08 b = 0;
	//SDA = read
	I2C_SDA_RD();
	//SDA = 1;
	I2C_SDA_H();
	waitms(1);
	
	//read 8 bits
	for (i=0; i<8; i++)
	{
		//SCL = 1
		I2C_SCL_H();
		waitms(1);
		//1 bit left
		b <<= 1;
		//if SDA == 1 add read bit
		if (bit_is_set(I2C_PORT_I,I2C_SDA)) 
			b |= 1;
		//SCL = 0;
		I2C_SCL_L();
		waitms(1);
	}
  	//SDA - write
	I2C_SDA_WR();
	//check ACK
	if (ack == 0)
		I2C_SDA_L();
	
	else
		I2C_SDA_H();
  
	waitms(1);
	I2C_SCL_H();
	waitms(1);
	I2C_SCL_L();
	waitms(1);
	I2C_SDA_L();
	waitms(1);
	
	return(b);
}

/**
 * Prepare for transmission
 */
void I2C_start(void)
{
	//port directions
	I2C_SCL_H();
	I2C_SDA_H();
	I2C_SDA_WR();
	I2C_SCL_WR();
	waitms(1);
	I2C_SDA_L();
	waitms(1);
	I2C_SCL_L();
	waitms(1);
}

/**
 * Stop transmission
 */
void I2C_stop(void)
{
	//SDA - write
	I2C_SDA_WR();
	I2C_SCL_H();
	waitms(1);
	I2C_SDA_H();
	waitms(1);
}

/**
 * Write to I2C
 * @param b
 * 		Byte to be written
 * @return
 */
u08 I2C_write(u08 b)
{
	u08 i;
	//SDA - write
	I2C_SDA_WR();
	//write 8 bits
	for (i=0; i<8; i++)
	{
		if (b & 0x80)
			I2C_SDA_H();
			
		else
			I2C_SDA_L();
			
		waitms(1);
		I2C_SCL_H();
		waitms(1);
		I2C_SCL_L();
		//roll 1 left
		b <<= 1;
	}
	
	//SDA - read
	I2C_SDA_RD();
	//Pull up SDA
	I2C_SDA_H();
	waitms(1);
	//SCL = 1;
	I2C_SCL_H();
	waitms(1);
	i=0xFF;
	
	do
	{
		//if we have affirmation
		if (bit_is_clear(I2C_PORT_I,I2C_SDA))
			break;
			
		waitms(1);
	}
	while(--i>0);
	
	//SCL = 0
	I2C_SCL_L();
	waitms(1);
	return(i);
}


