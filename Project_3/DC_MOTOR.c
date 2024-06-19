/*
 * DC_MOTOR.c
 *
 *  Created on: Jun 10, 2024
 *      Author: ASUS
 */
#include "gpio.h"
#include "DC_MOTOR.h"
#include "common_macros.h" /* For GET_BIT Macro */

void DcMotor_Init (void)
{

	GPIO_setupPinDirection(MOTOR0_PORT_ID,MOTOR0_PIN_ID ,PIN_OUTPUT );
	GPIO_writePin(MOTOR0_PORT_ID, MOTOR0_PIN_ID, LOGIC_LOW);

	GPIO_setupPinDirection(MOTOR1_PORT_ID , MOTOR1_PIN_ID,PIN_OUTPUT );
	GPIO_writePin(MOTOR1_PORT_ID, MOTOR1_PIN_ID,LOGIC_LOW);

}


void DcMotor_Rotate (DcMotor_State state , uint8 speed)
{

	if (state == STOP)
	{
		GPIO_writePin(MOTOR0_PORT_ID, MOTOR0_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR1_PORT_ID, MOTOR1_PIN_ID,LOGIC_LOW);
		PWM_Timer0_Init(speed);

	}
	else if (state == CW)
	{
		GPIO_writePin(MOTOR0_PORT_ID, MOTOR0_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(MOTOR1_PORT_ID, MOTOR1_PIN_ID,LOGIC_LOW  );
		PWM_Timer0_Init(speed);

	}

}
