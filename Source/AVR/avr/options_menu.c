/**
 * Options menu handles, functions and definitions
 * @author Michal Wachowicz
 */

/**
 * Shows option panel
 */
void options_show(void)
{
	LCD_clear();
	LCD_puts("    OPTIONS:    ");
	
	LCD_gotoxy(16,2);
	LCD_putc(62);	
}


/**
 * Shows work settings
 */
void work_settings_show(void)
{
	LCD_clear();
	LCD_puts("      WORK?     ");
	LCD_gotoxy(1,2);
	LCD_putc(60);	
	
	LCD_gotoxy(16,2);
	LCD_putc(62);
	if ( bit_is_clear(PIND, 5) )
		work_on();
	else
		work_off();

}

/**
 * Sets work on
 */
void work_on(void)
{
	LCD_gotoxy(5,2);
	LCD_puts("WORK  ON");
	LCD_putc(254);
	PORTD &=~(_BV(5));	
	m_work = 1;
}

/**
 * Sets work off
 */
void work_off(void)
{
	LCD_gotoxy(5,2);
	LCD_puts("WORK  OFF");
	PORTD |= _BV(5);
	m_work = 0;

}

/**
 * Shows PC connection message
 */
void PC_connection_show(void)
{
	LCD_clear();
	LCD_puts(" PC CONNECTION: ");
	LCD_gotoxy(1,2);
	LCD_putc(60);	
		
	if ( bit_is_clear(PIND, 6) )
		PC_connection_enable();
	else
		PC_connection_disable();
}

/**
 * Enables anility to connect to PC
 */
void PC_connection_enable(void)
{
	LCD_gotoxy(6,2);
	LCD_puts("ENABLE");
	LCD_putc(254);
	PORTD &=~(_BV(6));	
}

/**
 * Disables ability to connect to PC
 */
void PC_connection_disable(void)
{
	LCD_gotoxy(6,2);
	LCD_puts("DISABLE");
	PORTD |= _BV(6);
}
