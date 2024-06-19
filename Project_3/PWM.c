/*
 * PWM.c
 *
 *  Created on: Jun 11, 2024
 *      Author: ASUS
 */

#include "PWM.h"

 /* Description:
 * Generate a PWM signal with frequency 500Hz
 * Timer0 will be used with pre-scaler F_CPU/8
 * F_PWM=(F_CPU)/(256*N) = (10^6)/(256*8) = 500Hz
 * Duty Cycle can be changed by updating the value
 * in The Compare Register
 */
#include "gpio.h"

void PWM_Timer0_Init(uint8 set_duty_cycle)
{
		TCNT0 = 0; //Set Timer Initial value

		OCR0  = set_duty_cycle; // Set Compare Value


		GPIO_setupPinDirection(MOTORCOMPARE_PORT_ID , MOTORCOMPARE_PIN_ID,PIN_OUTPUT );


		/* Configure timer control register
		 * 1. Fast PWM mode FOC0=0
		 * 2. Fast PWM Mode WGM01=1 & WGM00=1
		 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
		 */
		TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);


}
