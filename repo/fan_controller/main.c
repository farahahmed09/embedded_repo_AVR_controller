/*
 * main.c
 *
 *  Created on: Oct 5, 2022
 *      Author: Farah Ahmed
 *
 *
 */


#include"lcd.h"
#include"lm35_sensor.h"
#include"motor.h"
#include<avr/io.h>
#include"adc.h"


int main(void)
{
	/* initialization for ADC driver */
	ADC_ConfigType configuration ={INTERNAL_VOLT,FCPU_div_8};
	ADC_init(&configuration);

	/* initialization for DC_Motor driver */
	DcMotor_State rotation_direction=CW;
	DCMOTOR_init();

	/*initialization for LCD_SCREEN driver */
	LCD_init();
	LCD_displayString("Fan is  ");
	LCD_moveCursor(1,0);
	LCD_displayString("Temp =     C");
	LCD_displayCharacter(' ');


	while(1)
	{
		uint8 tempreture = LM35_getTemperature();

		if(tempreture<30)
		{
			DCMOTOR_rotate(OFF , 0);
			LCD_moveCursor(0,7);
			LCD_displayString("OFF");
			LCD_displayCharacter(' ');
			LCD_moveCursor(1,7);
			LCD_intgerToString(tempreture);
			LCD_displayCharacter(' ');
		}
		else if(tempreture  >= 30 && tempreture  < 60)
		{
			DCMOTOR_rotate(rotation_direction, 25);
			LCD_moveCursor(0,7);
			LCD_displayString("ON");
			LCD_displayCharacter(' ');
			LCD_moveCursor(1,7);
			LCD_intgerToString(tempreture);
			LCD_displayCharacter(' ');
		}
		else if(tempreture  >= 60 && tempreture  < 90)

		{
			DCMOTOR_rotate(rotation_direction,50);
			LCD_moveCursor(0,7);
			LCD_displayString("ON");
			LCD_displayCharacter(' ');
			LCD_moveCursor(1,7);
			LCD_intgerToString(tempreture);
			LCD_displayCharacter(' ');
		}
		else if(tempreture >= 90 && tempreture  < 120)
		{
			DCMOTOR_rotate(rotation_direction, 75);
			LCD_moveCursor(0,7);
			LCD_displayString("ON");
			LCD_displayCharacter(' ');
			LCD_moveCursor(1,7);
			LCD_intgerToString(tempreture);
			LCD_displayCharacter(' ');
		}
		else if(tempreture >=120 && tempreture <150)

		{
			DCMOTOR_rotate(rotation_direction ,100);
			LCD_moveCursor(0,7);
			LCD_displayString("ON");
			LCD_displayCharacter(' ');
			LCD_moveCursor(1,7);
			LCD_intgerToString(tempreture);
			LCD_displayCharacter(' ');
		}
	}





}


