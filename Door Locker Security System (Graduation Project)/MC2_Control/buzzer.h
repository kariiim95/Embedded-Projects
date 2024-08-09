/*
 * buzzer.h
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"

#define BUZZER_PORT_ID	PORTD_ID
#define BUZZER_PIN_ID	PIN4_ID


void BUZZER_init(void);


void BUZZER_ONN (void);

void BUZZER_OFF (void);


#endif /* BUZZER_H_ */
