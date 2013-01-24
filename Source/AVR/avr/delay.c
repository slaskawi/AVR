/**
 * This file is responsible for delaying loops
 * @author Michal Wachowicz
 */


/**
 * Delaying function
 * @param x
 * 		Time to wait in ms
 * 		255 >= x >= 0
 */
void waitms(u16 x)
{
	u08 a, b; 
	//do it for x times
	for( ; x > 0; --x) 
	//do it for only 10 times
		for(b = 10; b > 0; --b)
		//do it for 100 tiles
			for(a = 100; a > 0; --a)
			//assembler function nop
				__asm("nop"); 
}

/**
 * Delying function
 * @param x
 * 		Time to wait in ms
 */
void delayms (u16 x)
{
	int i = 0;
	for (i; i<4; i++)
	waitms(x);

}
