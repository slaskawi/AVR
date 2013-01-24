#include "global.h"

/**
 * This file is responsible for conversions for different data types
 * @author Michal Wachowicz
 */

char nibble2hex(u08 val)  
{
	char s;
	s='0'+(val&0xf);
	if (s>'9') s+='A'-'9'-1;
	return s;
}

/**
 * Converts word to hex
 */
void word2hex(u16 val, char *s)  
{
	s[0]=nibble2hex(val>>12);
	s[1]=nibble2hex(val>>8);
	s[2]=nibble2hex(val>>4);
	s[3]=nibble2hex(val);
	s[4]=0;
}

/**
 * Converts BCD to binary representation
 */
u08 bcd2bin(u08 bcd)                    
{
	#ifdef OPTIMIZE_SPEED
	return (10*(bcd>>4)|(bcd&0x0f));
	#else
	u08 Temp = bcd & 0x0F; 
	while (bcd>=0x10) 
	{	 
		Temp += 10; 
		bcd -= 0x10; 
	} 
	return Temp; 
	#endif
}

/**
 * Converts binary number to BCD
 */
u08 bin2bcd(u08 bin)                    
{
    #ifdef OPTIMIZE_SPEED
	return (((bin/10)<<4)|(bin%10));
	#else
	
	u08 Temp = 0; 
	while(bin>9) 
	{		 
		Temp += 0x10; 
		bin-=10; 
	} 
	
	return Temp+bin; 
	#endif
 
}

/*
 * Converts byte to decimal representation
 */
void byte2dec(u08 val, char *s)  
{
	s[0]='0'+(val/10);
	s[1]='0'+(val%10);
	s[2]=0;
}

/**
 * Converts byte to hexadecimal representation
 */
void byte2hex(u08 val, char *s)  
{
	s[0]=nibble2hex(val>>4);
	s[1]=nibble2hex(val);
	s[2]=0; 
}

/**
 * Converts string to int representation
 */
u08 str_to_int( char *s )
{
	u08 value;
	
	if (s[0] == 0) 
		return s[1];

	return 0;
}
