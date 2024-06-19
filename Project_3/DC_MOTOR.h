/*
 * DC_MOTOR.h
 *
 *  Created on: Jun 10, 2024
 *      Author: ASUS
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"


#define MOTOR0_PORT_ID 	PORTB_ID
#define MOTOR0_PIN_ID 	PIN0_ID

#define MOTOR1_PORT_ID 	PORTB_ID
#define MOTOR1_PIN_ID 	PIN1_ID


typedef enum
{
	STOP,CW
}DcMotor_State;


void DcMotor_Init (void);



void DcMotor_Rotate (DcMotor_State state , uint8 speed);










#endif /* DC_MOTOR_H_ */
