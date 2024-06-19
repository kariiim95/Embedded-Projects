/*
 ================================================================================================
 Name        : temp_sensor_example.c
 Author      : Mohamed Tarek
 Description : Test the ADC driver designed with polling technique using LM35 Temperature Sensor
 Date        : 5/9/2015
 ================================================================================================
 */

#include "adc.h"
#include "lcd.h"
#include "lm35_sensor.h"
#include "PWM.h"
#include "DC_MOTOR.h"

int main(void)
{
	uint8 temp;
	ADC_ConfigType ADC_Configurations = {INTERNAL , DF8};
	LCD_init(); /* initialize LCD driver */
	ADC_init(&ADC_Configurations); /* initialize ADC driver */
	DcMotor_Init();

	LCD_moveCursor(0,4);
	LCD_displayString("FAN IS ");

	 LCD_moveCursor(1,4);
	/* Display this string "Temp =   C" only once on LCD at the first row */
	LCD_displayString("Temp =    C");

    while(1)
    {

    	temp = LM35_getTemperature();
		//DcMotor_Rotate(STOP,0);


		/* Display the temperature value every time at same position */
		//LCD_moveCursor(1,11);
		if(temp >=0 && temp <30)
		{
			LCD_moveCursor(1,11);
			LCD_intgerToString(temp);
			LCD_moveCursor(0,11);
			LCD_displayString("OFF");
			DcMotor_Rotate(STOP,0);

		}
		else
		{
			LCD_moveCursor(1,11);
			LCD_intgerToString(temp);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
			LCD_moveCursor(0,11);
			LCD_displayString("0NN");
		}


		if (temp >=0 && temp <30)
		{
			DcMotor_Rotate(STOP,0);
		}
		else if (temp >=30 && temp <60)
		{
			DcMotor_Rotate(CW,64);
		}
		else if (temp >=60 && temp <90)
		{
			DcMotor_Rotate(CW,128);
		}
		else if (temp >=90 && temp <120)
		{
			DcMotor_Rotate(CW,192);
		}
		else if (temp >=120 && temp <150)
		{
			DcMotor_Rotate(CW,255);
		}


    }
}
