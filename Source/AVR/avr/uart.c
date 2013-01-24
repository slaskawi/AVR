#include "global.h"

/**
 * This file is responsible for communications
 * @author Michal Wachowicz
 */


char uart_buffer[UART_BUFFER_SIZE];
volatile unsigned char uart_buffer_pos_first = 0, uart_buffer_pos_last = 0;
volatile unsigned char uart_buffer_overflow = 0;


/**
 * UART initializations
 */
void UART_init(u32 CLK, u32 UART_SPEED)
{

	// Baud rate selection
	UBRRH = 0; 
	UBRRL = (CLK/(UART_SPEED*16)-1);
  
	
	// USART setup
	//UCSRA = 0x02;        // 0000 0010
                       // U2X enabled
	UCSRC = 0x86;        // 1000 0110
                       // Access UCSRC, Asyncronous 8N1
	UCSRB = 0x98;        // 1001 1000
                       // Receiver enabled, Transmitter enabled
                       // RX Complete interrupt enabled  
	sei();               // Enable interrupts globally
}


/**
 * Put char
 */
void UART_putc(char data)
{
    while ( !( UCSRA & 0x20 ) ); // Wait untill UART data register is empty
    // Transmit data
    UDR = data;
}


/**
 * Put string
 */
void UART_puts(char *data)
{
  int len, count;
  
  len = strlen(data);
  for (count = 0; count < len; count++) 
    UART_putc(*(data+count));
}


/**
 * put hexadecimal number
 */
void UART_puthexU08(u08 value)
{
	char s[3];
	byte2hex(value,s);
	UART_puts(s);
}


/**
 * put hexadecimal number
 */
void UART_puthexU16(u16 value)
{
	char s[5];
	word2hex(value,s);
	UART_puts(s);
}



/**
 * Put integer of given radix
 */
void UART_putint(int value, u08 radix)	 
{
	char string[17];					// bufor na wynik funkcji itoa
	itoa(value, string, radix);			// konwersja value na ASCII
	UART_puts(string);					// wyœlij string na port szeregowy
}



/**
 * Put string from EEPROM
 */
void UART_puts_EEPROM(u08 *s)		
{
	register u08 c;
	while ((c = eeprom_read_byte(s++)))	// dopóki nie napotkasz 0
	{
		UART_putc(c);					// wysy³aj znak
	}
}



/**
 * Put string from FLASH
 */
void UART_puts_Flash(const char *s)	
{
	register u08 c;
	while ((c = pgm_read_byte(s++)))		// dopóki nie napotkasz 0
	{
		UART_putc(c);						// wysy³aj znak
	}
}

/**
 * put octal value
 */
void UART_putU08(u08 value)
{
	char s[3];
	byte2dec(value,s);
	UART_puts(s);
}


/**
 * Get char
 */
char UART_getc(void)
{
	// Wait untill unread data in ring buffer 
	if (!uart_buffer_overflow) 
		while(uart_buffer_pos_first == uart_buffer_pos_last); 
	uart_buffer_overflow = 0; 
  
	// Increase first pointer 
	if (++uart_buffer_pos_first >= UART_BUFFER_SIZE) 
		uart_buffer_pos_first = 0; 
	
	// Get data from the buffer 
	return uart_buffer[uart_buffer_pos_first]; 
} 


/**
 * Gets string from UART
 */
void UART_gets (char *s)
{
	uint8_t i = 0;
	uint8_t ch = 0;
	do
	{
		WDR();						// restart licznika Watchdoga
		ch = UART_getc();			// pobieraj znak
		s[i] = ch;					// wstaw go do ³añcucha i inkrementuj
		i++;						// inkrementuj iloœæ znaków odebranych
	}
	while ( (ch != '!') );
	
	s[i-1]=0;						// wstaw 0 na koniec ³añcucha
}





/**
 * Returns != 0 if there is some unread information
 * in the buffer
 */
unsigned char UART_unread_data(void)
{ 
	if (uart_buffer_overflow) 
		return UART_BUFFER_SIZE; 
  
	if (uart_buffer_pos_last > uart_buffer_pos_first) 
		return uart_buffer_pos_last - uart_buffer_pos_first; 
	
	if (uart_buffer_pos_last < uart_buffer_pos_first) 
		return UART_BUFFER_SIZE - uart_buffer_pos_first + uart_buffer_pos_last;
		
	return 0; 
} 



SIGNAL(SIG_UART_RECV)
{ 
	// Increase last buffer 
	if (++uart_buffer_pos_last >= UART_BUFFER_SIZE) 
		uart_buffer_pos_last = 0; 
  
	if (uart_buffer_pos_first == uart_buffer_pos_last) 
		uart_buffer_overflow++; 
  
	// Put data to the buffer 
	uart_buffer[uart_buffer_pos_last] = UDR;
}
