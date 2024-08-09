/*
 * APP_MC1.c
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 *   I am using 1 Mhz in Eclipce and proteus as Requested from Eng. Mohamed tarek,
 *   because proteus crackes alot, timer1 was implemented as 8 Mhz, so time will increase by 8
 */

#include"lcd.h"
#include"keypad.h"
#include "uart.h"
#include "timer_1.h"
#include "std_types.h"
#include <avr/io.h> /* To use the SREG register */
#include <util/delay.h> /* For the delay functions */



/*
 * Global Variables
 */
 uint8 Pass1_Arry[5];	//Array to hold first pass
 uint8 Pass2_Arry[5];	//Array to hold second pass
 uint8 count=0; 	//counter for timer1

 uint8  volatile seconds = 0;

// Orders sent from Micro2 to Micro1
 typedef enum
 {
 	failed_to_save,succeeded_to_save,wrong_password,right_password
 }errors;

 errors status;


 // Orders sent from  Micro1 to Micro2
 typedef enum
 {
 	Check_this_password,save_new_pass,Open,Buzzer_ONN,Buzzer_OFF,Holding,Close  // commands sent by HMI_ecu to give orders to Control_ecu
 }orders_from_HMI_ECU;




/****************************
 * 	Functions Prototype
 ***************************/

 void Timer_1_CallBack(void);
 void Timer_1_delay(uint32 delay);
 //Initialization structure for TIMER1
 Timer1_ConfigType Timer1_ConfigStruct = {0,31250,F_CPU_256,CTC_OCR};



//Function that takes pass1 and saved it in global array
void Pass1_Check_Display(void)
{
	uint8 i=0;//loop for 5 digits only

	LCD_displayString("pls enter pass:");
	LCD_moveCursor(1,0);
	while(i !=5)
	{
			for (i=0;i<5;i++)
			{
				//accept char from 0 to 9 only
				while(!(KEYPAD_getPressedKey() <= 9) && (KEYPAD_getPressedKey() >= 0)) {}


					Pass1_Arry[i]=KEYPAD_getPressedKey();
					_delay_ms(500);
					LCD_displayCharacter('*');
					_delay_ms(100);
			}

	}
	_delay_ms(250);


	while ( 13 != KEYPAD_getPressedKey()  ) {} ///Use ON as Enter Key asci is 13
	_delay_ms(250);

}

//Function that takes pass2 and saved it in global array SAME AS before FUNCTION
void Pass2_Check_Display(void)
{
	uint8 i=0;//loop for 5 digits only

	LCD_displayString("pls re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass:");

	while(i !=5)
	{
			for (i=0;i<5;i++)
			{
				//accept char from 0 to 9 only
				while(!(KEYPAD_getPressedKey() <= 9) && (KEYPAD_getPressedKey() >= 0)) {}

				Pass2_Arry[i]=KEYPAD_getPressedKey();
				_delay_ms(500);
				LCD_displayCharacter('*');
				_delay_ms(100);
			}

	}
		_delay_ms(250);

	while ( 13 != KEYPAD_getPressedKey()  ) {} ///Use ON as Enter Key
	_delay_ms(250);

}

///FUNCTION TAKES TWO GOLBAL ARRAYS AND SEND THEM TO MC2 TO CHECK SIMILARITY OR NOT
errors Check_created_password(uint8 Pass1_Arry[], uint8 Pass2_Arry[])
{

	LCD_displayString("Sending...");	//TEST CASE
	_delay_ms(250);

	for(uint8 i =0;i<5;i++)	//SENDING FIRST PASS
	{
		UART_send_byte(Pass1_Arry[i]);
		_delay_ms(100);
	}
	for(uint8 i =0;i<5;i++) //SENDING SECOND PASS
	{
			UART_send_byte(Pass2_Arry[i]);
			_delay_ms(100);
	}
	LCD_displayString("Sent");	//TEST CASE
		_delay_ms(250);
		errors status = UART_receive_byte(); // SENT TO EEPROM AND RETURN SAME PASS OR NOT
	return status;

}

/*
 * function to print the main menu
 * */
uint8 Main_options()
{
	LCD_clearScreen();
	LCD_displayString("+ : Open Door");
	LCD_displayStringRowColumn(1, 0, "- : Change Pass");
	return KEYPAD_getPressedKey();
}



/*
 *  function to verify the given password from the keypad with the password saved in the EEPROM
 * */
errors verify_password(uint8 Pass1_Arry[])
{

	UART_send_byte(Check_this_password); //order the MC2 to check it with the password saved in the EEPROM
	for(uint8 i =0;i<5;i++)
	{
		UART_send_byte(Pass1_Arry[i]);
	}
	errors status = UART_receive_byte(); //waiting to confirm that the password is correct or not
	return status;
}



/*
 * function to open the door
 * */
void Open_door(uint8 Pass1_Arry[])
{
	uint8 false_pass_count=0;
	uint8 i=0;

	while(false_pass_count<3)//to count false password max three times
	{
		LCD_clearScreen();
		//_delay_ms(1000);
		LCD_displayString("pls enter Pass: ");
		LCD_moveCursor(1, 0);

		while(i !=5)
			{
				for (i=0;i<5;i++)
				{
					while(!(KEYPAD_getPressedKey() <= 9) && (KEYPAD_getPressedKey() >= 0)) {}

						Pass1_Arry[i]=KEYPAD_getPressedKey();
						_delay_ms(500);
						LCD_displayCharacter('*');
						_delay_ms(100);
				}
			}
			i=0;
		while ( 13 != KEYPAD_getPressedKey()  ) {}
		_delay_ms(250);

		errors status = verify_password(Pass1_Arry); //waiting the MC2 to verify and give an ack or confirm

		if (status == right_password )
		{

			LCD_clearScreen();
			LCD_displayString("Unlocking Door");
			UART_send_byte(Open);	//telling the MC2 to open
			Timer_1_delay(2);  		/*Incase of 8Mhz = Timer_1_delay(15);
									  Incase of 1Mhz 15sec/8=1.75  ~2 sec*/

			LCD_clearScreen();
			LCD_displayString("HOLDING...");
			UART_send_byte(Holding);	//telling the MC2 to open
			Timer_1_delay(1);			/*Incase of 8Mhz = Timer_1_delay(3);
									  	  Incase of 1Mhz 3sec/8= 0.3 sec ~1 sec */


			LCD_clearScreen();
			LCD_displayString("Door is locking");
			UART_send_byte(Close);//telling the MC2 to close

			Timer_1_delay(2);  		/*Incase of 8Mhz = Timer_1_delay(15);
									  Incase of 1Mhz 15sec/8=1.75 ~ 2 sec*/
			LCD_clearScreen();
			break;
		}
		else
		{
			LCD_clearScreen();
			LCD_displayString("Wrong");	//TEST CASE
			_delay_ms(1000);
			LCD_clearScreen();
			false_pass_count++; // increment it tell reaching 3
		}

	}

	if(false_pass_count==3)
	{

		LCD_clearScreen();
		LCD_displayString("ERROR");//TEST CASE
		UART_send_byte(Buzzer_ONN); // SEND TO MC2 TO TURN BUZZER ON
		Timer_1_delay(1);			/*Incase of 8Mhz = Timer_1_delay(3);
								  	  Incase of 1Mhz 3sec/8= 0.3 sec ~1 sec */
		LCD_clearScreen();
		UART_send_byte(Buzzer_OFF); // hear i controlled the timing of the buzzer by the HMI_ecu not the Control_ecu unlike the motor

	}


}


//FUCTION TAKES OLD PASS AND CHECKS WITH MEMORY IF SAME, OVERRIDE THE OLD PASS AND SAVE NEW
void Change_password(uint8 pass[])
{
	uint8 false_pass_count=0;
	uint8 i;

	while(false_pass_count<3)//ENTER PASS MAX 3 TIMES WRONG
	{
		_delay_ms(500);
		LCD_clearScreen();
		LCD_displayString("Enter OLD Pass:");
		LCD_moveCursor(1, 0);

		while(i !=5)//SAME ANS TWO FUNCTIONS ABOVE
			{
				for (i=0;i<5;i++)
				{
				while(!(KEYPAD_getPressedKey() <= 9) && (KEYPAD_getPressedKey() >= 0)) {}

					Pass1_Arry[i]=KEYPAD_getPressedKey();
					_delay_ms(500);
					LCD_displayCharacter('*');
				_delay_ms(100);
				}
			}
					i=0;	//TO ALLOW 3 TRIALS FOR PASSWORLD
				while ( 13 != KEYPAD_getPressedKey()  ) {}
				_delay_ms(250);

		//SENDIND ENTER PASS TO MC2 AND RETURN RIGHT OR WORNG PASS
		errors status = verify_password(pass);
		if (status == right_password )//IF CORRECT OLD PASS ENTERED
		{
			do{
				LCD_clearScreen();
				_delay_ms(500);

				Pass1_Check_Display(); // ENTER NEW PASS 1
				LCD_clearScreen();
				Pass2_Check_Display();	 // ENTER NEW PASS 2

				UART_send_byte(save_new_pass);//
				status = Check_created_password(Pass1_Arry, Pass2_Arry); //CHECK IF THE SAME FIRST

				//IF TWO PASS NOT THE SAME
				if (status == failed_to_save)
				{
					LCD_clearScreen();
					LCD_displayString("Not Matched");//TEST CASE
					_delay_ms(1500);
					LCD_clearScreen();
				}
				else if (status == save_new_pass) // IF TWO ARE SAME
				{
					LCD_clearScreen();
					LCD_displayString("NEW PASS SAVED");//TEST CASE
					_delay_ms(1500);

				}

			}while(status== failed_to_save); //LOOP 3 TIMES
			break;
		}
		else
		{

			false_pass_count++; // INCREAMENT WRONG TRAILS TILL 3

			LCD_clearScreen();
			LCD_displayString("WRONG"); //TEST CASE
			_delay_ms(1000);
			LCD_clearScreen();


		}
	}
	if(false_pass_count==3) //IF THREE TIMES WRONG PASS ENTERED
	{
		LCD_clearScreen();
		LCD_displayString("ERROR");
		UART_send_byte(Buzzer_ONN); // SEND TO MC2
		Timer_1_delay(1);			/*Incase of 8Mhz = Timer_1_delay(3);
								  	  Incase of 1Mhz 3sec/8= 0.3 sec ~1 sec */
		UART_send_byte(Buzzer_OFF);  // SEND TO MC2

	}
}



int main (void)
{

	LCD_init();

	LCD_displayString("1 Mhz for protes");//according to ENG, MT use 1 Mhz (proteus Craches)
	LCD_displayStringRowColumn(1,0,"USE ON/C AS ENTR"); // USE ON\C FOR ENTER ONLY
	_delay_ms(4000);
	LCD_clearScreen();


	UART_config config_uart ={eight,disabled,one_bit,9600};
	UART_init(& config_uart);

	errors status; //FOR COMMUNICATION WITH MIRCO


	//Initialization structure for TIMER1
	//Timer1_ConfigType Timer1_ConfigStruct = {0,31250,F_CPU_256,CTC_OCR};

	SREG |= (1<<7);


	Pass1_Check_Display(); // TAKE PASS1 IN GLOBAL ARRAY
	LCD_clearScreen();
	Pass2_Check_Display(); // TAKE PASS2 IN GLOBAL ARRAY
	LCD_clearScreen();

	//CHECK IS TWO PASS ARE SAME,IF NOT SUCK THERE
	status = Check_created_password(Pass1_Arry, Pass2_Arry);


			while (status == failed_to_save) //KEEP LOOPING UNTILL YOU ENTER SAME SAME
			{
				LCD_moveCursor(1,0);
				LCD_displayString("Not Matched");
				_delay_ms(1000);

				LCD_clearScreen();
				Pass1_Check_Display(); // TAKE PASS1 IN GLOBAL ARRAY
				LCD_clearScreen();
				Pass2_Check_Display(); // TAKE PASS2 IN GLOBAL ARRAY
				LCD_clearScreen();

				//SEND TO EPROM TO RETURN FAILED TO SAVE OR SUCCES IF SAME PASS
				//IF SAME BREAK WHILE
				status = Check_created_password(Pass1_Arry, Pass2_Arry);
			}

			if (status == succeeded_to_save) // MC2- EPROM RETURNS THAT THE ARE SAME
			{
				LCD_moveCursor(1,0);
				LCD_displayString("Matched");
				_delay_ms(2000);
			}


	while(1)
	{

		uint8 choice;

		choice = Main_options(); 	// MAIN MENU  ALWAYS READY & RETURNS + OR -

		switch (choice)
		{
			case '+':
				_delay_ms(1000);
				LCD_clearScreen();
				LCD_displayString("Enterd +");
				_delay_ms(500);
				LCD_clearScreen();
				Open_door(Pass1_Arry); //FUCNTION FOR OPEN DOORS
			break;

			case '-':
				LCD_clearScreen();
				LCD_displayString("Enterd -");
				_delay_ms(500);
				LCD_clearScreen();
			Change_password(Pass1_Arry); //FUNCTION TO TAKE TWO PASS AND SAVE NEW ONE
			break;
		}

	}

}



/*
 * : Delay the code execution for a specific time interval
 */
void Timer_1_delay(uint32 delay)
{

	//Initialize timer1 with the configuration structure
	Timer_1_init(&Timer1_ConfigStruct);

	//Set the callback function for timer1
	Timer_1_setCallBack(&Timer_1_CallBack);

	//Stay in the loop as long as the required number of delay seconds hasn't elapsed
	while(seconds != delay);

	//Deinitialize timer1
	Timer_1_deInit();

	//Reset the seconds variable
	seconds = 0;
}


/*
 * Description: increment the seconds variable each time the function is called
 */
void Timer_1_CallBack(void)
{
	seconds++;
}

