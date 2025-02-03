/******************************************************************************
 *
 * Module: PIR SENSOR
 *
 * File Name: pir_sensor.c
 *
 * Description: Source file for the PIR sensor driver
 *
 * Author: Nada Hamed
 *
 *******************************************************************************/
#include "pir_sensor.h"

void PIR_init(pirConfig *str)
{
	if(str->state==off)
	{

	}
	else if(str->state==on)
	{
		GPIO_setupPinDirection(str->pir_input_port,str->pir_input_pin,PIN_INPUT);
		/* waits 3sec till sensor stabilize*/
		_delay_ms(3000);
	}
	else
	{

	}


}

uint8 detect_motion(pirConfig *str)
{

	return GPIO_readPin(str->pir_input_port , str->pir_input_pin);
}

