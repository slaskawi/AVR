#include <avr/interrupt.h>
#include <avr/signal.h>        

/**
 * This file was designed to handle alarm events.
 * We tried to use it but now it is done in other way
 * @author Sebastian Laskawiec
 */


SIGNAL (SIG_INTERRUPT0) 
{
	//if ( bit_is_clear(PIND, 5) )
	//{
		do
		{
			sbi(PORTB, 0);
			waitms(250);
			cbi(PORTB, 0);
			waitms(250);
		}
		while ( !(bit_is_clear(PINA, 7)) );
	//}
}



void INT_init(void)
{
	//interrupts on falling edge
	MCUCR = _BV(ISC01)|_BV(ISC11);
	GICR = 0xC0;
	//interrupts on
	sei();
  
}
