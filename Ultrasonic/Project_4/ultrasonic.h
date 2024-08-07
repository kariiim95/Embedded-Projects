 /******************************************************************************
 *
 * Module:
 *
 * File Name: ultrasonic.h
 *
 * Description:
 *
 * Author: ASUS
 *
 *******************************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define TRIGGER_PORT_ID PORTB_ID
#define TRIGGER_PIN_ID  PIN5_ID

#define ECHO_PORT_ID PORTD_ID
#define ECHO_PIN_ID  PIN6_ID
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initializing the ICU driver, setup the ICU call back function
 * and setup the direction for the trigger pin as output pin.
 */
void Ultrasonic_init(void);

/*
 * Description :
 * Function responsible for sending the Trigger pulse to the Ultrasonic Sensor.
 */
void Ultrasonic_Trigger(void);

/*
 * Description :
 * Function responsible for sending the trigger pulse by using Ultrasonic_Trigger() function
 * and start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void);

/*
 * Description :
 * This is the call back function called by the ICU driver, It is used to calculate
 * the high time (pulse time) generated by the Ultrasonic Sensor.
 *
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
