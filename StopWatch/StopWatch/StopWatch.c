/*
 * PROJECT_AVR.c
 *
 *  Created on: May 8, 2024
 *      Author: ASUS
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define false 0   	//define false =0
#define true 1		//define true =1
#define NUMBER_OF_COMPARE_PER_SECOND 20  //compare value which complete 1 second

/*FUNCTIONS PROTOTYPE*/
void seven_seg_configure (void); 	//7-SEGMENTS INSTALLATION
void time_calculations (void);		// TIME CALCULATION

void Timer1(void); 			//TIMER1_COMPA
void INT0_Init(void);		//INT0 SETUP
void INT1_Init(void);		//INT0 SETUP
void INT2_Init(void);		//INT0 SETUP


//unsigned char num=0;  /* NUMBER TO BE DISPLAYED */
unsigned char  ticks = 0;  /* TICKS COUNTED BY ISR TIMER1_COMPA */
unsigned char  flag = false  ; //set flag =0 first

unsigned char dig_arr[]={0 , 1 , 2 , 3 ,\
		 4 , 5 , 6, 7 , 8 , 9}; /*ARRAY TO LOOP FROM 0-9*/


unsigned char sec_units = 0, sec_tenth = 0, min_units = 0;\
min_tenth =0 , hr_units= 0, hr_tenth = 0;


unsigned char sec = 0, min=0,hr=0; //COUNTING SEC & MIN & HOURS


void Timer1(void)	/* TIMER1 SETUP*/
{
	TCNT1 = 0X00;		/* Set timer1 initial count to zero */

	OCR1A = 50000;		/* 50,000 MULTIPLY BY 20 = 1x10^6 usec = 1 sec */

	TCCR1A = TCCR1A | (1<<FOC1A) ; /*FORCE OUTPUT COMPARE A  */

	TCCR1B = (1<<WGM12) | (1<<CS11) ;	//8 PRESCALE CS11=1

	TIMSK = TIMSK  | (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

}

ISR(TIMER1_COMPA_vect) //timer1 compare mode intintialzation
{
	ticks++;  //increment till reach compare value
	if ( ticks  == NUMBER_OF_COMPARE_PER_SECOND)
	{
		flag =true;// increment 7-segment every 1 complete second
		ticks = false; //restart ticks again to reach 20;
	}
		/*
		 *  8 Mhz (uc) divided by (8) prescale = 1 Mhz
		 *  Assume Output Compare OCR1A = 50,000 clock cycle
		 * 	Therefore, 10^6 Mhz / 50,000 cycle = 20 interrupt
		 */
}

void INT0_Init(void) //TIMER0 SETUP
{


	DDRD = DDRD & ~(1<<PD2);	/* input pin*/
	PORTD = PORTD |(1<<PD2); 	// internal pull up ON


	MCUCR |= (1<<ISC01); // Enable External interrupt Request 0
	MCUCR &=~(1<<ISC00);	// falling edge of INT0

	GIFR |=(1<<INTF0);	//ENABLE INTERRUPT FLAG REGISTER

	GICR  |= (1<<INT0); 	//CONTROL REGISTER-ENABLE INTERRUPT REQUEST

}

ISR (INT0_vect) //TIMER0 ISR
{

	PORTC =0;   //restart time
	sec = 0;//restart time
	min=0;	//restart time
	hr=0;	//restart time
	sec_units = 0;	//restart time
	sec_tenth = 0; //restart time
	min_units = 0;	//restart time
	min_tenth =0 ;	//restart time
	hr_units= 0;	//restart time
	hr_tenth = 0;	//restart time
}

void INT1_Init(void){


	DDRD = DDRD & ~(1<<PD3); //INPUT PIN

	MCUCR  = MCUCR | (1<<ISC10)|(1<<ISC11);

	GIFR  = GIFR | (1<<INTF1); //ENABLE INTERRUPT FLAG REGISTER

	GICR  = GICR | (1<<INT1); //Enable External interrupt 1
}

ISR(INT1_vect)
{

	TIMSK = TIMSK & ~(1<<OCIE1A); /* SET TIMER1 INTERRUPT =0 */

}

void INT2_Init(void)
{

	DDRB = DDRB & ~(1<<PB2);
	PORTB = PORTB |  (1<<PB2); //Activate the internal pull up


	MCUCSR &= ~(1<<ISC2); //Enable External interrupt Request 2
	GIFR |=(1<<INTF2);  //ENABLE FLAG


	GICR  |= (1<<INT2); //Enable External interrupt

}

ISR (INT2_vect)
{

	TIMSK = TIMSK | (1<<OCIE1A);  /* RETURN TIMER1 TO 1 */

}

int main (void)
{

	SREG = SREG | (1<<7);             // Enable global interrupts.

	 Timer1();						//TIMER1 CONFIGURURAION
	 seven_seg_configure();			// LCD CONFIGURATION
	 time_calculations ();			//function to calculate time

	 INT0_Init();  //INTERRUPT0 SETUP
	 INT1_Init();	//INTERRUPT1 SETUP
	 INT2_Init();	//INTERRUPT2 SETUP

	 while (1)
	{
		 /* WHEN DISPLAYING ANY NUMBER START BY DISPLAYING ITS TENTH FIRES
		  * EXAMPLE DISPLAYING: 23 SEC , DISPLAY 2 FIRST THEN DISPLAY 3*/

		 	 	PORTA &=~(1<<PA5)&~(1<<PA3)&~(1<<PA2)&~(1<<PA1)&~(1<<PA4);
		 		PORTA = PORTA |(1<<PA0);
		 		PORTC = dig_arr [sec_tenth];
		 		_delay_ms(1); //DELAY

		 		PORTA &=~(1<<PA4)&~(1<<PA3)&~(1<<PA2)&~(1<<PA0)&~(1<<PA5);
		 		PORTA |=(1<<PA1);
		 		PORTC = dig_arr [sec_units];
		 		_delay_ms(1);

		 		PORTA &=~(1<<PA4)&~(1<<PA3)&~(1<<PA5)&~(1<<PA1)&~(1<<PA0);
		 		PORTA |=(1<<PA2);
		 		PORTC = dig_arr [min_tenth];
		 		_delay_ms(1);

		 		PORTA &=~(1<<PA4)&~(1<<PA5)&~(1<<PA2)&~(1<<PA1)&~(1<<PA0);
		 		PORTA |=(1<<PA3);
		 		PORTC = dig_arr [min_units];
		 		_delay_ms(1);

		 		PORTA &=~(1<<PA0)&~(1<<PA3)&~(1<<PA2)&~(1<<PA1)&~(1<<PA5);
		 		PORTA |=(1<<PA4);
		 		PORTC = dig_arr [hr_tenth];
		 		_delay_ms(1);

		 		PORTA &=~(1<<PA4)&~(1<<PA3)&~(1<<PA2)&~(1<<PA1)&~(1<<PA0);
		 		PORTA |=(1<<PA5);
		 		PORTC = dig_arr [hr_units];
		 		_delay_ms(1);

		 	if ( flag == true) /*IF 1 SECOND PASSED, 1 000 000 Us passed*/
		 	{
		 		 time_calculations ();
			 	flag= false; // reset flag (equal to zero) to restart count again
		 	}
	}

}

void seven_seg_configure (void)
{
	DDRC = DDRC | 0X0F;			/*4 pins of 7447 */
	PORTC = PORTC & 0xF0;

	DDRA = DDRA | 0X3F;         /*  SET FIRTS 6 PINS OF A OUTPUT OF 6-(7-SEGMENT)*/
	PORTA = PORTA | 0X3F;       /*SET FIRST 6 PINS IN PORTA AS OUTPUT */

}

void time_calculations (void)
{
	sec++;

		if(sec > 59)
		{
			sec = 0;
			min++;

		}
		if (min > 59)
		{
			min = 0;
			hr++;
		}
		if (hr > 24)
		{

			hr = 0;
		}

		sec_tenth = (sec/10);
		sec_units = sec - (sec_tenth*10) ;

		min_tenth = min/10;
		min_units = min - (min_tenth*10) ;

		hr_tenth = hr/10;
		hr_units= hr - (hr_tenth*10) ;

}



