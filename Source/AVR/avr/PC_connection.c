/**
 * PC command handles
 * @author Michal Wachowicz
 */
 
u08 command[16];

/**
 * Gets PC command index
 * @return 
 * 		PC command index
 */
u08 PC_get_msg(void)
{
	while ( UART_unread_data() )
	{

		UART_gets(command);
		
		
		if ( !(strncmp (command, "connect",7)) ) 		
			return 1;
		else
		if ( !(strncmp (command, "disconnect",10)) ) 	
			return 2;
		else		
		if(!(strncmp (command, "datetime",8)) )
			return 15;
		else
		if ( !(strncmp (command, "settings",8)) ) 		
			return 16;
		else
		if ( !(strncmp (command, "settemp",7)) ) 		
			return 17;
		else
		if ( !(strncmp (command, "ttsetup",7)) ) 		
			return 18;
		else
		if ( !(strncmp (command, "timer",5)) ) 		
			return 4;
		else
		if ( !(strncmp (command, "date",4)) ) 			
			return 5;
		else
		if ( !(strncmp (command, "temp",4)) ) 			
			return 6;		
		else
		if ( !(strncmp (command, "onwork",4)) ) 			
			return 19;		
		else
		if ( !(strncmp (command, "offwork",4)) ) 			
			return 20;		
		else
		if ( !(strncmp (command, "uart",4)) ) 			
			return 7;
		else
		if ( !(strncmp (command, "exit",4)) ) 			
			return 8;
		else
		if ( !(strncmp (command, "timeset",7)) ) 		
			return 9;
		else
		if ( !(strncmp (command, "dataset",7)) ) 		
			return 10;
		else
		if ( !(strncmp (command, "on",2)) ) 			
			return 11;
		else
		if ( !(strncmp (command, "off",3)) ) 			
			return 12;
		else
		if ( !(strncmp (command, "console",8)) ) 		
			return 14;
		else		
		
		return 0;
	}
	
	return 0;
}

/**
 * Sends message to PC
 * @param 
 * 		message/command
 */
u08 PC_send_msg(char *command)
{
	UART_puts(command);
}

/**
 * Shows connecting dialog
 */
void PC_connecting_show(void)
{
	LCD_clear();
	LCD_puts("Connecting...");
	LCD_progressbar(2,250);
	LCD_clear();
	LCD_puts("Connected...");
}

/**
 * Shows disconnecting dialog
 */
void PC_disconnecting_show(void)
{
	LCD_clear();
	LCD_puts("Disconnecting...");
	LCD_progressbar(2,250);
	PORTD |=_BV(4);
	LCD_clear();
	LCD_puts("Disconnected...");
	delayms(1000);
	LCD_clear();
	LCD_puts("      Ready     ");
}

void show_PC_menu()
{
	LCD_gotoxy(1,1);
	LCD_puts("    Waiting!    ");
}

/**
 * PC command switch - decides what to do
 * @param
 * 		PC code
 */
PC_switch (u08 PC_code)
{
	switch ( PC_code )
	{		
		
		
		case PC_DATE_TIME:
		{
			PC_DateTime_Show();					
			LCD_clear();
			break;
		}
			
		case PC_TEMPERATURE:
		{					
			PC_temperature_show();					
			LCD_clear();
			break;
		}
		
		case PC_SETTINGS:
		{
			LCD_clear();
			LCD_puts("Sending settings");
			UART_putint(m_nightHour,10);
			UART_puts("!");
			UART_putint(m_nightLast,10);
			UART_puts("!");
			UART_putint(m_dayTemp,10);
			UART_puts("!");
			UART_putint(m_nightTemp,10);
			UART_puts("!");
			UART_putint(m_offsetTemp,10);
			UART_puts("!");
			UART_putint(m_work,10);
			UART_puts("!");
			break;
		}
		
		case PC_TEMP_TIME_SET_UP:
		{
			PC_temp_time_set();
			break;
		}
		
		case PC_TIME_SET:
		{
			PC_time_set();
			break;
		}
		
		case PC_DATE_SET:
		{
			PC_date_set();
			break;
		}
		
		case PC_TEMP_SET_UP:
		{
			PC_temp_set();
			break;
		}
		
		case PC_WORK_ON:
		{
			work_on();
			LCD_clear();
			LCD_puts("    Waiting!    ");
			break;
		}
		
		case PC_WORK_OFF:
		{
			work_off();
			LCD_clear();
			LCD_puts("    Waiting!    ");
			break;
		}
				
					
		default: 
		{					
			break;
		}	
	}
}	
