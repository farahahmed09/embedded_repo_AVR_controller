/*
 * main.c
 *
 *  Created on: Oct 12, 2022
 *      Author: farah ahmed
 */
#include "lcd.h"
#include "ultrasonic.h"
#include "icu.h"
#include <avr/io.h> /*to use SREG register */



Icu_ConfigType configuration = {F_CPU_8 , RISING };


int main(void)
{


	SREG|=(1<<7); /* interrupts enable */
	uint16 calculated_distance=0;
	Ultrasonic_init();
	LCD_init();
	LCD_displayString("Distance=     cm");

	while(1)
	{
		calculated_distance= Ultrasonic_readDistance();
		LCD_moveCursor(0,10);
		LCD_intgerToString(calculated_distance);
		LCD_displayCharacter(' ');

	}

}

