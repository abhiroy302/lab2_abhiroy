#include <avr/io.h>
#include <avr/interrupt.h>
#define BAUD_RATE 9600
#define F_CPU 16000000UL
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
volatile int period = 0;
volatile int time = 0;
char String[25];
volatile int flag = 0;
volatile int overflow_count = 0;
volatile int final = 0;
volatile int num = 0;
volatile int count_dot = 0;
volatile int space_count = 0;
volatile int special_check = 0;
#include <util/delay.h>

void Initialize()
{
	cli();
	// Set Input Bit and Input Capture Bit
	DDRB &= ~(1<<DDB0);
	
	// Serial Monitor Stuff
	UART_init(BAUD_PRESCALER);
	
	// Prescaling Timer 1 by 1024
	TCCR1B |= (1<<CS12);
	TCCR1B |= (1<<CS10);
	TCCR1B &= ~(1<<CS11);
	
	// Prescaling Timer 0 by 1024
	TCCR0B |= (1<<CS02);
	TCCR0B |= (1<<CS00);
	TCCR0B &= ~(1<<CS01);
	
	// Enable Timer0 Overflow Interrupt
	TIMSK0 |= (1<<TOIE0);
	
	// Looking for Rising Edge in Timer 1 
	TCCR1B |= (1<<ICES1);
	
	// Clearing Interrupt Flag
	TIFR1 |= (1<<ICF1);
	
	//Enabling Input Capture Interrupt
	TIMSK1 |= (1<<ICIE1);
	
	sei();
}

ISR(TIMER1_CAPT_vect)
{
	overflow_count = 0;
	TCCR1B ^= (1<<ICES1);
	flag ++;
	period = ICR1 - time;
	time = ICR1;
}

ISR (TIMER0_OVF_vect)
{
	overflow_count ++;
}


int main(void)
{
	Initialize();
	
    /* Replace with your application code */
    while (1) 
    {
		// For DOT
		if (period > 468 && period < 3125 && flag % 2 == 0)
		{
			num ++;
			count_dot ++;
			final = final + num*10;
			special_check = special_check + 0*pow(2,num-1);
			period = 0;
			space_count = 0;
		}
		
		// For DASH
		else if (period > 3125 && period < 6250 && flag % 2 == 0)
		{
			num ++;
			final = final + num*100;
			special_check = special_check + 1*pow(2,num-1);
			period = 0;
			space_count = 0;
		}
		
		// For SPACE
		else if (overflow_count > 24)
		{
			space_count ++;
			num = 0;
			overflow_count = 0;
		}
		
		// Dot-Dash to Letters
		switch (final)
		{
			case 210:
			{
				if (space_count > 2)	
				{
					sprintf(String,"A \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 190:
			{
				if (space_count > 0)
				{
					sprintf(String,"B \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 460:
			{
				if (count_dot == 2 && space_count > 0)
				{
					sprintf(String,"C \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 3 && space_count > 0)
				{
					sprintf(String,"V \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 150:
			{
				if (space_count > 1)
				{
					sprintf(String,"D \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 10:
			{
				if (space_count > 4)
				{
					sprintf(String,"E \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 370:
			{
				if (count_dot == 2 && space_count > 0)
				{
					sprintf(String,"Z \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 3 && space_count > 0)
				{
					sprintf(String,"F \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 330:
			{
				if (count_dot == 1 && space_count > 1)
				{
					sprintf(String,"G \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 2 && space_count > 1)
				{
					sprintf(String,"U \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 100:
			{
				if (count_dot == 4 && space_count > 0)
				{
					sprintf(String,"H \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 0 && space_count > 3)
				{
					sprintf(String,"T \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 30:
			{
				if (space_count > 2)
				{
					sprintf(String,"I \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 910:
			{
				if (space_count > 0)
				{
					sprintf(String,"J \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 420:
			{
				if (count_dot == 1 && space_count > 1)
				{
					sprintf(String,"K \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 3 && space_count > 0)
				{
					sprintf(String,"7 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 280:
			{
				if (space_count > 0)
				{
					sprintf(String,"L \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 300:
			{
				if (space_count > 2)
				{
					sprintf(String,"M \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 120:
			{
				if (space_count > 2)
				{
					sprintf(String,"N \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 600:
			{
				if (count_dot == 0 && space_count > 1)
				{
					sprintf(String,"O \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 4 && space_count > 0)
				{
					sprintf(String,"4 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 550:
			{
				if (special_check == 6 && space_count > 0)
				{
					sprintf(String,"P \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (special_check == 9 && space_count > 0)
				{
					sprintf(String,"X \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 730:
			{
				if (space_count > 0)
				{
					sprintf(String,"Q \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 240:
			{
				if (count_dot == 2 && space_count > 1)
				{
					sprintf(String,"R \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				else if (count_dot == 4 && space_count > 0)
				{
					sprintf(String,"6 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 60:
			{
				if (space_count > 1)
				{
					sprintf(String,"S \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 510:
			{
				if (space_count > 1)
				{
					sprintf(String,"W \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 820:
			{
				if (space_count > 0)
				{
					sprintf(String,"Y \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 1410:
			{
				if (space_count > 0)
				{
					sprintf(String,"1 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 930:
			{
				if (space_count > 0)
				{
					sprintf(String,"2 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 960:
			{
				if (space_count > 0)
				{
					sprintf(String,"3 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 110:
			{
				if (space_count > 0)
				{
					sprintf(String,"5 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 690:
			{
				if (space_count > 0)
				{
					sprintf(String,"8 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 1050:
			{
				if (space_count > 0)
				{
					sprintf(String,"9 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			case 1500:
			{
				if (space_count > 0)
				{
					sprintf(String,"0 \n");
					UART_putstring(String);
					final = 0;
					count_dot = 0;
				}
				break;
			}
			
			default:
			{	
				break;
			}
		}
    }
}