/*
 * Internal_EEPROM.h
 *
 *  Created on: Sep 2, 2023
 *      Author: mustafa
 */

#ifndef INTERNAL_EEPROM_H_
#define INTERNAL_EEPROM_H_


#include "std_types.h"

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr);


#endif /* INTERNAL_EEPROM_H_ */
