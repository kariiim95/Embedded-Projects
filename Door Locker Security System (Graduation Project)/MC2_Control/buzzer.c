/*
 * buzzer.c
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */

#include"buzzer.h"
#include "gpio.h"

void BUZZER_init(void)
{

	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID , PIN_OUTPUT);

	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);

}


void BUZZER_ONN (void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}


void BUZZER_OFF (void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
