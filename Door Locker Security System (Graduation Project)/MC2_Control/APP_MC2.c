/*
 * APP_MC2.c
 *
 *  Created on: Jul 10, 2024
 *      Author: ASUS
 */

#include "uart.h"
#include "std_types.h"
#include <avr/io.h> /* To use the SREG register */
#include <util/delay.h> /* For the delay functions */

#include "external_eeprom.h"
#include "twi.h"

#include "buzzer.h"
#include "motor.h"

#include "Timer0.h"
#include "timer_1.h"

#define MC2_READY 0x10

/*
 * TWO ENTERED PASS AND ONE SAVED IN EEPROM
 */
uint8 Pass1_Arry[5], Pass2_Arry[5], Pass_EEPROM[5];
uint8 count =0;

void Get_Pass_AND_Check (void);



// To count how many seconds have elapsed
uint8  volatile seconds = 0;


 void Timer_1_CallBack(void);
 void Timer_1_delay(uint32 delay);

 //Initialization structure for TIMER1
 Timer1_ConfigType Timer1_ConfigStruct = {0,31250,F_CPU_256,CTC_OCR};


typedef enum
{
	failed_to_save,succeeded_to_save,wrong_password,right_password
}errors;

errors status; //STATUS OF MC2


typedef enum
{
	Check_this_password,save_new_pass,Open,Buzzer_ONN,Buzzer_OFF,Holding,Close  // commands sent by HMI_ecu to give orders to Control_ecu
}orders_from_HMI_ECU;


/*
 * take two pass and check if same, if not return failed, if same save it in EEPROM
 * */
errors write_in_memory( uint8 *pass,uint8 * repass)
{
	uint8 i;
	for ( i=0; i<5; i++)
	{
		if( pass[i]!=  repass[i])
		{
			_delay_ms(50); // this delay is used between each two operations done on the eeprom
			return failed_to_save;
		}
	}
	for(uint8 i =0;i<5;i++)
	{
		EEPROM_writeByte(1+i, Pass1_Arry[i]); // 1+i is the address of the location in the eeprom that i'm doing to save in it the 5-byte pass								// each byte is a number
		_delay_ms(10); // this delay is used between each two operations done on the eeprom
	}
		return succeeded_to_save;
}


/*
 * CHECH ENTED PASS WITH SAVED PASS IN EEPROM
 * if same return right pass else return wrong
 */
errors verfiy_password_from_memory(uint8 pass[])
{

	for (uint8 i = 0;i<5;i++)
	{
		EEPROM_readByte(1+i, Pass_EEPROM+i);//saving the pass in eeprom in the arr
											  //pass_in_memory is the arr name and +i to increment the ptr as the function takes a ptr
		_delay_ms(10); // this delay is used between each two operations done on the eeprom
	}

	for(uint8 i =0;i<5;i++)
	{
		if(pass[i]!=Pass_EEPROM[i])
			return wrong_password;
	}
	return right_password;
}

/*
 * receives two pass and check if differ return failed, if same return same
 */
void Get_Pass_AND_Check (void)
{
	while(status==failed_to_save)
	{

		for (uint8 i=0; i<5;i++)
	 		{
	 			Pass1_Arry[i]=UART_receive_byte();
	 		}
	 		for (uint8 i=0; i<5;i++)
	 		{
	 			Pass2_Arry[i]=UART_receive_byte();
	 		}
	 		status = write_in_memory(Pass1_Arry, Pass2_Arry);
	 		UART_send_byte(status);
	}
	 if (status == succeeded_to_save)
	 {

	 	uint8 i;
		 for( i =0;i<5;i++)
	 	{
			 BUZZER_ONN ();
	 		EEPROM_writeByte(5+i, Pass1_Arry[i]); // 1+i is the address of the location in the eeprom that i'm doing to save in it the 5-byte pass								// each byte is a number
	 		_delay_ms(20); // this delay is used between each two operations done on the eeprom
	 	}
	 		BUZZER_OFF();
	 }

}


void main (void)
{

	uint8 choice;

	UART_config config_uart ={eight,disabled,one_bit,9600};
	UART_init(& config_uart);

	TWI_ConfigType config_twi ={0b00000010,400000}; // this is address 1 since address is represented in 7 bits and lsb is for r/w
	TWI_init( & config_twi); // config of twi (i2c) to communicate with eeprom

	MOTOR_init();
	BUZZER_init();

	SREG |= (1<<7);

	Get_Pass_AND_Check();  // receives two pass and check if differ keep asking for same pass,
							//if same write it in memory


	while(1)
	{
		//RECEIVE FROM MC1 - ALWAYS READY -RECEIVING
		choice = UART_receive_byte();

		switch(choice)
		{
		case Check_this_password: //INCASE CHECK PASS WITH MEMOMRY

			for(uint8 i=0;i<5;i++)
			{
				Pass1_Arry[i]= UART_receive_byte();
			}

			status = verfiy_password_from_memory(Pass1_Arry); //SEND GLOBAL PASS TO EPROM
			UART_send_byte(status);
			break;

		case save_new_pass: //when choose to change pass then begin to send two passwords. the same bulk at the beginning of the main
			do{
				for (uint8 i=0; i<5;i++)
					Pass1_Arry[i]=UART_receive_byte();
				for (uint8 i=0; i<5;i++)
					Pass2_Arry[i]=UART_receive_byte();
				status = write_in_memory(Pass1_Arry, Pass2_Arry); //RETURN FAILED OR SECCUSE
				UART_send_byte(status);
			 }while(status == failed_to_save);
				break;

		case Open:  ///RECIVED TO OPEN DOOR
			Motor_Rotate(Clockwise, 100);
			break;

		case Holding:	//RECIEVE TO HOLD DOOR
			Motor_Rotate(Stop, 0);
			break;

		case Close:	//RECIEVE TO CLOSE DOOR
			Motor_Rotate(Anti_Clockwise, 100);
			Timer_1_delay(2);
			Motor_Rotate(Stop, 0);
			break;


		case Buzzer_ONN:  //TURN BUZZER ONN
			BUZZER_ONN();
			break;

		case Buzzer_OFF: //TURN OFF
			BUZZER_OFF();
			break;
		}
	}

}





/*
 * SAME AS MC1
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


