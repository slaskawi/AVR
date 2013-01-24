/**
 * This is the main entry point of program
 * @author Michal Wachowicz
 */

/**
 * Program includes
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h> 
#include <stdlib.h>
#include <avr/signal.h>
#include "global.h"
#include "uart.c"
#include "lcd.c"
#include "ds18b20.c"
#include "delay.c"
#include "conversion.c"
#include "i2c.h"
#include "pcf8583.c"
#include "rc5.c"
#include "remote_choice.c"
#include "timer_menu.c"
#include "options_menu.c"
#include "PC_connection.c"
#include "PC_menu.c"
#include "temp.c"
#include "alarm.c"



/**
 * Main program loop
 */
int main(void)
{        
	DDRD = 0xff;
	PORTD = 0xf0;	
	PORTD |= _BV(3);
	
	UART_init(16000000, 19200);			
	LCD_init();
	PCF8583_init();
		
	PCF8583_set_time(12,00,0,0);
	PCF8583_set_date(24,1,2013);
	
	//set the alarm off
	PCF8583_get_status();
	PCF8583_status|=4;
	PCF8583_write(0,PCF8583_status);
	
	cbi(PORTD, 6);
	
	u16 code;
	
	LCD_clear();
	
	get_temperatureSilent();
	power_on();	
	while(1)
	{		
		if(m_requiresSettings == SETTING_ALL)
		{
			updateTime();
			updateTemperature();
			dayNightDecision();	
		}
		
		
			power_on();	
			
			code = decode_rc5();
			if ( code != 0 )
			{
				remote_switch(code);
				while ( UART_unread_data() ) UART_getc();
			}			
		
		//if uart recieved something
		if ( (PC_get_msg() == PC_CONNECT) & bit_is_clear(PORTD, 6) )
		{
			PC_connecting_show();
			do
			{		
				show_PC_menu();		
				code = PC_get_msg();				
				PC_switch( code );				
			}
			while( code != PC_DISCONNECT );
			PC_disconnecting_show();
		}		
	}	
	return 0;
}
