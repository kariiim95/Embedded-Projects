/*
 * TWI.c
 *
 *  Created on: Oct 30, 2022
 *      Author: abdoa
 */
#include "twi.h"
#include "avr/io.h"
#include "common_macros.h"


void TWI_init(TWI_ConfigType * config)
{

	TWAR= config->address;
	TWBR = ((F_CPU/config->bit_rate)-16)/2;
	TWSR = 0x00;
	TWCR =(1<<TWEN);

}

void TWI_start(void){

	TWCR =(1<<TWEN)|(1<<TWSTA)|(1<<TWINT);

	while(BIT_IS_CLEAR(TWCR,TWINT));

}


void TWI_stop(void){

	TWCR =(1<<TWEN)|(1<<TWSTO)|(1<<TWINT);

	//while(BIT_IS_CLEAR(TWCR,TWINT)); i don't have to put it

}

void TWI_writeByte(uint8 data){
	TWDR = data;
	TWCR =(1<<TWEN)|(1<<TWINT);
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readByteWithACK(void){

	TWCR =(1<<TWEN)|(1<<TWEA)|(1<<TWINT);
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}

uint8 TWI_readByteWithNACK(void){
	TWCR =(1<<TWEN)|(1<<TWINT);
	while(BIT_IS_CLEAR(TWCR,TWINT));
	return TWDR;
}

uint8 TWI_getStatus(void){

	return TWSR & 0XF8;
}
