/*
 * Timer0.h
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */
#ifndef GPIO_TIMER0_H_
#define GPIO_TIMER0_H_

#include "std_types.h"


void PWM_Timer0_init(void);

void PWM_Timer0_Start(uint8 duty_cycle);


#endif /* GPIO_TIMER0_H_ */
