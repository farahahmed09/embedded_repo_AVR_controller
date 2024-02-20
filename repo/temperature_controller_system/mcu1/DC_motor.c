
#include "DC_motor.h"
#include"PWM_timer.h"


/*Description
➢ The Function responsible for setup the direction for the two
motor pins through the GPIO driver.
➢ Stop at the DC-Motor at the beginning through the GPIO driver.*/


void DcMotor_Init(void)
{
	/* configure pin PC0 and PC1 as output pins */
	GPIO_setupPinDirection(DCMotor_PORT_ID,DCMotor_PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(DCMotor_PORT_ID,DCMotor_PIN1_ID,PIN_OUTPUT);

	/* Motor is stop at the beginning */
	GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN0_ID,LOGIC_LOW);
	GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN1_ID,LOGIC_LOW);
}

/*Description:
➢ The function responsible for rotate the DC Motor CW/ or A-CW or
stop the motor based on the state input state value.
➢ Send the required duty cycle to the PWM driver based on the
required speed value.*/

void DcMotor_Rotate(uint8 speed)
{
	// 0<=speed<=100
	PWM_Timer0_Start(speed);
	if(speed==0)
	{
		GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN0_ID,LOGIC_LOW);
		GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN1_ID,LOGIC_LOW);
	}
	else
	{
		// Rotate Clockwise
	    GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN0_ID,LOGIC_HIGH);
     	GPIO_writePin(DCMotor_PORT_ID,DCMotor_PIN1_ID,LOGIC_LOW);
	}
}

