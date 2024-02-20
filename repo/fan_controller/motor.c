

#include"motor.h"
#include"pwm_timer0.h"
#include"gpio.h"


/*******************************************************************************
 *                      Functions Definition                                  *
 *******************************************************************************/

void DCMOTOR_init(void)
{
	/*set 2 pins of motor to be output*/
	GPIO_setupPinDirection(DC_PORT_ID,DC_PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DC_PORT_ID,DC_PIN1_ID,PIN_OUTPUT);

	/* stop the motor at the beginning*/
	GPIO_writePin(DC_PORT_ID,DC_PIN0_ID,LOGIC_LOW);
	GPIO_writePin(DC_PORT_ID,DC_PIN1_ID,LOGIC_LOW);



}
void DCMOTOR_rotate( DCMOTOR_state state , uint8 speed)
{

	if(state==0)
	{
		/* state when motor is off */
		PWM_timer0_start(speed);
		GPIO_writePin(DC_PORT_ID,DC_PIN0_ID,LOGIC_LOW);
		GPIO_writePin(DC_PORT_ID,DC_PIN1_ID,LOGIC_LOW);
	}
	else if (state==1)
	{
		/* state when motor rotate clockwise */
		PWM_timer0_start(speed);
		GPIO_writePin(DC_PORT_ID,DC_PIN0_ID,LOGIC_LOW);
		GPIO_writePin(DC_PORT_ID,DC_PIN1_ID,LOGIC_HIGH);


	}
	else if (state==2)
	{

		/*state when motor rotate counter clockwise */
		PWM_timer0_start(speed);
		GPIO_writePin(DC_PORT_ID,DC_PIN0_ID,LOGIC_HIGH);
		GPIO_writePin(DC_PORT_ID,DC_PIN1_ID,LOGIC_LOW);
	}






}

