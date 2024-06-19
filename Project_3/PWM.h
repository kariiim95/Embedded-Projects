/*
 * PWM.h
 *
 *  Created on: Jun 11, 2024
 *      Author: ASUS
 */

#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>
#include "std_types.h"


#define MOTORCOMPARE_PORT_ID 	PORTB_ID
#define MOTORCOMPARE_PIN_ID 	PIN3_ID

void PWM_Timer0_Init(uint8 set_duty_cycle);

#endif /* PWM_H_ */
