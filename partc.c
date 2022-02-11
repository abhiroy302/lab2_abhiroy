/*
 * GccApplication1.c
 *
 * Created: 2/9/2022 10:36:31 AM
 * Author : abhiroy
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void Initialize()
{
	cli();
	DDRB &= ~(1<<DDB0);
	DDRB |= (1<<DDB5);
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS10);
	TCCR1B &= ~(1<<ICES1);
	TIMSK1 |= (1<<ICIE1);
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	TCCR1B ^= (1<<ICES1);
	PORTB ^= (1<<PORTB5);
}

int main(void)
{
	Initialize();
    /* Replace with your application code */
    while (1) 
    {
    }
}

