/*
 * buzzer.c
 *
 *  Created on: Oct 29, 2022
 *      Author: farah ahmed
 */

#include "buzzer.h"
#include "gpio.h"
#include "std_types.h"
#include "common_macros.h"

 /*******************************************************************************
 *                      Functions Definition                               *
 *******************************************************************************/


void Buzzer_init(void)
{
	/*
	 * - set the direction of buzzer pin to be output
	 * - disable the buzzer at the beginning
	 */
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);


}
void Buzzer_on(void)
{
	/*
	 * enable the buzzer
	 */
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);

}
void Buzzer_off(void)
{
	/*
	 * disable the buzzer
	 */
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}
