/******************************************************************************
 *
 * Module:
 *
 * File Name: ultrasonic.c
 *
 * Description:
 *
 * Author: ASUS
 *
 *******************************************************************************/

#include <util/delay.h>
#include "gpio.h"
#include "icu.h"
#include "ultrasonic.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
volatile uint8 g_flag = 0;      /* This flag is set when the high time is calculated */
volatile uint8 g_edgeCount = 0; /* Variable to count the edges */
volatile uint16 g_timeHigh = 0; /* This is the time in which the signal travels from HC-SR04 to object and returns to the module HC-SR-04 again */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description :
 * Function responsible for initializing the ICU driver, setup the ICU call back function
 * and setup the direction for the trigger pin as output pin.
 */
void Ultrasonic_init(void) {


	GPIO_setupPinDirection(TRIGGER_PORT_ID, TRIGGER_PIN_ID, PIN_OUTPUT); /* The TRIGGER_PIN  */
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);

	GPIO_setupPinDirection(ECHO_PORT_ID, ECHO_PIN_ID, PIN_INPUT); /* The ECHO_PIN  */
	GPIO_writePin(ECHO_PORT_ID, ECHO_PIN_ID, LOGIC_LOW);

	/* Configure the ICU Driver with F_CPU/8 and wait for the RAISING edge at the beginning */
	ICU_ConfigType ICU_Configurations = { F_CPU_8, RAISING };
	ICU_init(&ICU_Configurations);

	/* Ultrasonic_edgeProcessing() is the CallBack Function called by the ICU Driver */
	ICU_setCallBack(Ultrasonic_edgeProcessing);


}
/*
 * Description :
 * Function responsible for sending the Trigger pulse to the Ultrasonic Sensor.
 */
void Ultrasonic_Trigger(void) {
	/* Send at least 10  microseconds pulse to the Ultrasonic Sensor through the TRIGGER_PIN */
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_HIGH);
	_delay_us(12);
	GPIO_writePin(TRIGGER_PORT_ID, TRIGGER_PIN_ID, LOGIC_LOW);
}
/*
 * Description :
 * Function responsible for sending the trigger pulse by using Ultrasonic_Trigger() function
 * and start the measurements by the ICU from this moment.
 */
uint16 Ultrasonic_readDistance(void) {
	uint16 distnace;
	Ultrasonic_Trigger(); /* Send Rrigger pulse */
	while (g_flag != 1); /* Wait till calculate the high time */

	/* we have selected an internal 8MHz oscillator frequency for ATmega32
	 * with Prescaler F_CPU/8 for timer frequency
	 * then time to execute 1 instruction is 1 us*/
	distnace = (g_timeHigh / 58.8);


	return distnace;
}
/*
 * Description :
 * This is the call back function called by the ICU driver, It is used to calculate
 * the high time
 */
void Ultrasonic_edgeProcessing(void) {
	g_edgeCount++;
	g_flag = 0; /* Clear the g_flag */
	if (g_edgeCount == 1) /* Fisrt Edge (RISING) */
	{
		ICU_clearTimerValue(); /* Reset the timer counter */
		ICU_setEdgeDetectionType(FALLING);
	} else if (g_edgeCount == 2) /* Second Edge (FALLING) */
	{
		g_timeHigh = ICU_getInputCaptureValue(); /* Store the High time value */
		ICU_setEdgeDetectionType(RAISING);
		g_edgeCount = 0;
		g_flag = 1; /* Set g_flag to execute the distance measurement in the Ultrasonic_readDistance Function */
	}
}
