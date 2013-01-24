/**
 * PCF handles
 */
 
#include "global.h"

volatile u08 PCF8583_status;
volatile u08 PCF8583_alarm;


u08 PCF8583_read(u08 address)
{
	u08 a;
	a=(PCF8583_A0<<1)|0xa0;
	I2C_start();
	I2C_write(a);
	I2C_write(address);
	I2C_start();
	I2C_write(a|1);
	a=I2C_read(1);
	I2C_stop();
	return a;
}



u08 PCF8583_read_bcd(u08 address)
{
	return bcd2bin(PCF8583_read(address));
}



u08 PCF8583_get_status(void)
{
	PCF8583_status=PCF8583_read(0);
	PCF8583_alarm=(PCF8583_status&2);
	return PCF8583_status;
}



void PCF8583_alarm_off(void)
{
	//DDRD = 0xff;
	//sbi(PORTD, 2);
	
	PCF8583_get_status();
	PCF8583_status&=0xfb;
	PCF8583_write(0,PCF8583_status);
	
	
}



void PCF8583_alarm_on(void)
{
	PCF8583_get_status();
	PCF8583_status|=4;
	PCF8583_write(0,PCF8583_status);
	
	PORTD &=~(_BV(5));	
	DDRD = 0xf0;
	sbi(PORTD, 2);
	INT_init();
}



void PCF8583_get_alarm_date(u08 *day,u08 *month)
{
	*day=bcd2bin(PCF8583_read(0xd)&0x3f);
	*month=bcd2bin(PCF8583_read(0xe)&0x1f);
}



void PCF8583_get_alarm_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec)
{
	*hsec=PCF8583_read_bcd(0x9);
	*sec=PCF8583_read_bcd(0xa);
	*min=PCF8583_read_bcd(0xb);
	*hour=PCF8583_read_bcd(0xc);
}



void PCF8583_get_date(u08 *day,u08 *month,u16 *year)
{
	u08 dy;
	u16 y1;
	PCF8583_hold_on();
	dy=PCF8583_read(5);
	*month=bcd2bin(PCF8583_read(6)&0x1f);
	PCF8583_hold_off();
	*day=bcd2bin(dy&0x3f);
	dy>>=6;
	y1=PCF8583_read(16)|((u16) PCF8583_read(17)<<8);
	if (((u08) y1&3)!=dy) PCF8583_write_word(16,++y1);
	*year=y1;
}



void PCF8583_get_time(u08 *hour,u08 *min,u08 *sec,u08 *hsec)
{
	PCF8583_hold_on();
	*hsec=PCF8583_read_bcd(1);
	*sec=PCF8583_read_bcd(2);
	*min=PCF8583_read_bcd(3);
	*hour=PCF8583_read_bcd(4);
	PCF8583_hold_off();
}



void PCF8583_hold_off(void)
{
  PCF8583_get_status();
  PCF8583_status&=0xbf;
  PCF8583_write(0,PCF8583_status);
}

	

void PCF8583_hold_on(void)
{
	PCF8583_get_status();
	PCF8583_status|=0x40;
	PCF8583_write(0,PCF8583_status);
}



void PCF8583_init(void)
{
	PCF8583_status=0;
	PCF8583_alarm=0;
	PCF8583_write(0,0);
	PCF8583_write(4,PCF8583_read(4)&0x3f);
	PCF8583_write(8,0x90);
}



void PCF8583_set_alarm_date(u08 day,u08 month)
{
	PCF8583_write_date(0xd,day,0);
	PCF8583_write_bcd(0xe,month);
}



void PCF8583_set_alarm_time(u08 hour,u08 min,u08 sec,u08 hsec)
{
	PCF8583_write_bcd(0x9,hsec);
	PCF8583_write_bcd(0xa,sec);
	PCF8583_write_bcd(0xb,min);
	PCF8583_write_bcd(0xc,hour);
	
	PCF8583_alarm_on();
}



void PCF8583_set_date(u08 day,u08 month,u16 year)
{
	PCF8583_write_word(16,year);
	PCF8583_stop();
	PCF8583_write_date(5,day,year);
	PCF8583_write_bcd(6,month);
	PCF8583_start();
}



void PCF8583_set_time(u08 hour,u08 min,u08 sec,u08 hsec)
{
//  if (hour>23) hour=0;
//  if (min>59) min=0;
//  if (sec>59) sec=0;
//  if (hsec>100) hsec=0;
	PCF8583_stop();
	PCF8583_write_bcd(1,hsec);
	PCF8583_write_bcd(2,sec);
	PCF8583_write_bcd(3,min);
	PCF8583_write_bcd(4,hour);
	PCF8583_start();
}



void PCF8583_start(void)
{
  PCF8583_get_status();
  PCF8583_status&=0x7f;
  PCF8583_write(0,PCF8583_status);
}



void PCF8583_stop(void)
{
  PCF8583_get_status();
  PCF8583_status|=0x80;
  PCF8583_write(0,PCF8583_status);
}



void PCF8583_write(u08 address,u08 data)
{
  I2C_start();
  I2C_write((PCF8583_A0<<1)|0xa0);
  I2C_write(address);
  I2C_write(data);
  I2C_stop();
}



void PCF8583_write_bcd(u08 address,u08 data)
{
  PCF8583_write(address,bin2bcd(data));
}



void PCF8583_write_date(u08 address,u08 day,u16 year)
{
  PCF8583_write(address,bin2bcd(day)|(((u08) year&3)<<6));
}



void PCF8583_write_word(u08 address,u16 data)
{
  PCF8583_write(address,(u08) data&0xff);
  PCF8583_write(++address,(u08)(data>>8));
}
