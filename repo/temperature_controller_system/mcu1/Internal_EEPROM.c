/*
 * Internal_EEPROM.c
 *
 *  Created on: Sep 2, 2023
 *      Author: mustafa
 */

#include "Internal_EEPROM.h"
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "avr/io.h" /* To use the IO Ports Registers */

void EEPROM_writeByte(uint16 u16addr,uint8 u8data)
{
	/* Wait for completion of previous write */
//	while(EECR & (1<<EEWE))
//	;
	/* Set up address and data registers */
	EEAR = u16addr;
	EEDR = u8data;
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start eeprom write by setting EEWE */
	EECR |= (1<<EEWE);
}


uint8 EEPROM_readByte(uint16 u16addr)
{
	/* Wait for completion of previous write */
//	while(EECR & (1<<EEWE))
//	;
	/* Set up address register */
	EEAR = u16addr;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from data register */
	return EEDR;
}

