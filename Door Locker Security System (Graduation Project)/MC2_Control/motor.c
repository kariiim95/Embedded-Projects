/*
 * motor.c
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */


#include"motor.h"
#include "gpio.h"
#include"std_types.h"
#include "Timer0.h"


void MOTOR_init(void)
{
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);

	GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN1_PIN_ID , LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_IN2_PIN_ID, LOGIC_LOW);
}

void Motor_Rotate(Motor_State state,uint8 speed)  //speed from 0 to 100

{
switch(state)
	{
	case Anti_Clockwise:

		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_HIGH);
		break;
	case Clockwise:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_HIGH);
		break;
	case Stop:
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN2_PIN_ID, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID, MOTOR_IN1_PIN_ID, LOGIC_LOW);
		break;
	}

	PWM_Timer0_Start(speed);

}
