
#include "servo_motor.h"
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>	/* Include Delay header file */
#include <avr/io.h>
#include"gpio.h"

void Timer2_Fast_PWM_Init(unsigned char duty_cycle)
{
    /* Set initial values for Timer2 */
    TCNT2 = 0;            // Set Timer2 initial count to zero
    OCR2 = duty_cycle;    // Set the compare value for duty cycle

    /* Configure Timer2 control register TCCR2 */
    /* Fast PWM mode, non-inverting (Clear OC2 on compare match) */
    TCCR2 |= (1 << WGM20) | (1 << WGM21) | (1 << COM21);

    /* Configure the prescaler for Timer2 (64 prescaler) */
    TCCR2 |= (1 << CS22);

    /* Set the OC2 pin (PD7) as an output */
    GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
}


void rotate_Servo(uint8 degree)
{
	Timer2_Fast_PWM_Init(degree);
}
