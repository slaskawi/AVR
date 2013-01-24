#include "global.h"

/**
 * This file is responsible for LCD
 * @author Michal Wachowicz
 */

/**
 * LCD Defines
 */
#define LCD  PORTB
#define E  	 3
#define RS   2

/**
 * Text wrapping and polish signs
 */
#define LCD_WRAP
#define LCD_WIN1250

#define SET_E   LCD |= _BV(E)
#define CLR_E   LCD &= ~_BV(E)

#define SET_RS  LCD |= _BV(RS)
#define CLR_RS  LCD &= ~_BV(RS)

/**
 * Init LCD
 */
void LCD_init(void)
{
	//Ports configuration
	DDRB = 0xFF;	
	PORTB = 0xFF;
		
	//wait for stable voltage	
	waitms(15);
	//E = 0
	CLR_E;
	//RS = 0
	CLR_RS;
	char i;
	//3 x send instructions set
	for(i = 0; i < 3; i++)
	{
		SET_E;
		LCD &= 0x3F;
		CLR_E;
		waitms(5);
	}
	SET_E;
	LCD &= 0x2E;
	CLR_E;	
	waitms(1);
	
	//4 bits interface, 2 lines, 5x7 sign
	LCD_write_command(0x28);
	waitms(20);
	//turn on LCD, pointer and blinking
	LCD_write_command(0x08);
	waitms(20);
	//clear LCD
	LCD_write_command(0x01);
	waitms(20);
	//no shifting right
	LCD_write_command(0x06);
	waitms(20);
	//turn on LCD, no pointer, no blinking
	LCD_write_command(0x0C);
	
}


/**
 * Write byte to LCD
 * @param
 * 		byte to be written
 */
void write_to_lcd(char x)
{
	SET_E;
	//first half of byte
	LCD = ((LCD & 0x0F) | (x & 0xF0));
	//write it to LCD on E falling edge
	CLR_E;
	SET_E;
	//last half of byte
	LCD = ((LCD & 0x0F) | ((x & 0x0F) << 4));
	CLR_E;
	waitms(1);
}


/**
 * Write command to LCD
 * @param 
 * 		command
 */
void LCD_write_command(char x)
{
	CLR_RS;
	write_to_lcd(x);
}

/**
 * Write data to LCD
 * @param 
 * 		data
 */
void LCD_putc(char x)
{
	SET_RS;
	write_to_lcd(x);
}

/**
 * Put u08 to LCD
 * @param 
 * 		value
 */
void LCD_puthexU08(u08 value)
{
	char s[3];
	byte2hex(value,s);
	LCD_puts(s);
}

/**
 * Put u16 to LCD
 * @param 
 * 		value
 */
void LCD_puthexU16(u16 value)
{
	char s[5];
	word2hex(value,s);
	LCD_puts(s);
}

/**
 * Write String to LCD
 * @param
 * 		String
 */
void LCD_puts(char * s)
{
	int i = 0;
	//while != null
	while(*s)
	{
		//write pointer value
		LCD_putc(*s);
		//increment pointer
		s++;
		i++;
		if (i == 16) LCD_gotoxy (1,2);
	}
}

/**
 * Write value of given radix to LCD
 * @param value
 * 		value
 * @param radix
 * 		radix
 */
void LCD_putint(int value, u08 radix) 
{
	//buffer
	char string[18];
	//conversion
	itoa(value, string, radix);
	//LCD write
	LCD_puts(string);
}


/**
 * Sets cursor to x,y (numeration starts of 1)
 * @param x
 * @param y
 */ 
void LCD_gotoxy (char x, char y) 
{ 
    switch (y) 
    { 
        case 1: 
            x += (0x80-1); 
            break; 
        case 2: 
            x += (0xC0-1); 
            break; 
        case 3: 
            x += (0x94-1); 
            break; 
        case 4: 
            x += (0xD4-1); 
            break; 
    } 
    LCD_write_command(x); 
}

/**
 * Clears LCD
 */
void LCD_clear(void)
{
	LCD_write_command(0x01);	
	waitms(5);			
	LCD_gotoxy(1,1);	
}

/**
 * Display progress bar
 * @param pos_y
 * 		position
 * @param delay
 * 		Delay
 */
void LCD_progressbar(u08 pos_y, u16 delay)
{
	u08 i;
	
	for(i=1; i<=16; i++)
	{
		LCD_gotoxy(i,pos_y);
		LCD_putc(255);
		if (delay > 255)
		delayms(delay);
		else
		waitms(delay);		
	}
}
