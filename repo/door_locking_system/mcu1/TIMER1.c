/*
 * TIMER1.c
 *
 *  Created on: Oct 24, 2022
 *      Author: farah ahmed
 */
#include "TIMER1.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "std_types.h"

/*
 * global pointer to hold the address of the call back
 */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                            ISR Definitions                                  *
 *******************************************************************************/

ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application*/
		g_callBackPtr();
	}
}

ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		g_callBackPtr();
	}
}


/*******************************************************************************
 *                      Functions Definition                                  *
 *******************************************************************************/

void Timer1_init(const TIMER1_ConfigType * Config_Ptr)
{
	/*
	 * - set timer1 initial value
	 * - set the prescaler
	 */

	TCCR1A|=(1<<FOC1A)|(1<<FOC1B);
	TCCR1B=0;
	TCCR1B=(TCCR1B&0xF8)|(Config_Ptr->prescaler);
	/*
	 * if compare mode is chosen
	 * - set timer to compare mode
	 * - enable compare interrupt on channel A
	 * - set the compare value to get interrupt with each match
	 */
	if(Config_Ptr->mode)
	{
		TCNT1=0;
		TCCR1B|=(1<<WGM12);
		TIMSK=(1<<OCIE1A);
		OCR1A= Config_Ptr->compare_value;
	}
	/*
	 * if normal mode is chosen
	 *
	 * - enable overflow interrupt
	 */
	else
	{
		TCNT1=(Config_Ptr->initial_value);
		TIMSK=(1<<TOIE1);
	}

}

void Timer1_deInit(void)
{
	TCNT1=0;
	TCCR1B=0;
	TIMSK=0;
	TCCR1A=0;
	OCR1A=0;
}
void Timer1_setCallBack(void(*a_ptr)(void))
{
	/*
	 * save the address of the called function
	 */
	g_callBackPtr = a_ptr;

}

