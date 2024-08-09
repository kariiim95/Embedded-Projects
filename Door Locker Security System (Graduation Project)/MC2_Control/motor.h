/*
 * motor.h
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include"std_types.h"

#define	MOTOR_PORT_ID 		PORTC_ID

#define	MOTOR_IN1_PIN_ID	PIN2_ID
#define	MOTOR_IN2_PIN_ID	PIN3_ID

#define	MOTOR_EN1_PORT_ID	PORTB_ID
#define	MOTOR_EN1_PIN_ID	PIN3_ID


typedef enum
{
	Clockwise,Anti_Clockwise,Stop
}Motor_State;


void MOTOR_init(void);

void Motor_Rotate(Motor_State state,uint8 speed);


#endif /* MOTOR_H_ */
