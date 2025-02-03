
#include"pwm_timer0.h"
#include"gpio.h"
#include <avr/io.h>




/*******************************************************************************
 *                      Functions Definition                                  *
 *******************************************************************************/


void PWM_timer0_start(uint8 duty_cycle)
{


	TCNT0 = 0; //Set Timer Initial value


	/*store the duty in the OCR0 register to give a match */
	OCR0= ((duty_cycle)*2.55);


	/* configure direction of OC0 pin to be output */
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);


	/* configure Timer0 to work with fast PWM non inverting mode*/
	 TCCR0=(1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS01);

}
