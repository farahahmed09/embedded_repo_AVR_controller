/*
 * ultrasonic.c
 *
 *  Created on: Oct 12, 2022
 *      Author: farah ahmed
 */

#include "ultrasonic.h"
#include "gpio.h"
#include "icu.h"
#include <util/delay.h>


uint8 edge=0 ;
uint32 high_time=0;

/****************************************************************************************************
                                functions definitions
 **************************************************************************************************/
void Ultrasonic_init(void)
{

	Icu_init(&configuration);
	Icu_setCallBack(Ultrasonic_edgeProcessing);
	GPIO_setupPinDirection(TRIGGER_PORT_ID , TRIGGER_PIN_ID , PIN_OUTPUT);
	GPIO_writePin(TRIGGER_PORT_ID , TRIGGER_PIN_ID , LOGIC_LOW);



}
void Ultrasonic_trigger(void)
{

	GPIO_writePin(TRIGGER_PORT_ID , TRIGGER_PIN_ID , LOGIC_HIGH);
	_delay_us(11);
	GPIO_writePin(TRIGGER_PORT_ID , TRIGGER_PIN_ID , LOGIC_LOW);

}

uint16 Ultrasonic_readDistance(void)
{

	Ultrasonic_trigger();
	while((edge==0 )|| (edge==1) || (edge==2))
	{
		if(edge==2)
		{
			edge=0;
			break;
		}
	}
	/*
	 * division by factor 0.983 to remove the error percentage in calculations.
	 */
	return (((high_time)*0.017)/0.983);



}
void Ultrasonic_edgeProcessing(void)
{

	edge++;
	if(edge==1)
	{
		Icu_clearTimerValue();
		Icu_setEdgeDetectionType(FALLING);

	}
	else if(edge==2)
	{
		high_time = Icu_getInputCaptureValue();
		Icu_clearTimerValue();
		Icu_setEdgeDetectionType(RISING );
	}


}
