
#ifndef MOTOR_H_
#define MOTOR_H_
#include"std_types.h"




/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
#define DC_PORT_ID PORTA_ID
#define DC_INPUT_PIN1_ID PIN6_ID
#define DC_INPUT_PIN2_ID  PIN7_ID
typedef enum
{
	OFF,CW,Anti_CW
}DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                *
 *******************************************************************************/
/*
 * function description :
 * initialize the dc motor outputs pins
 */
void DCMOTOR_init(void);

/*
 * function description :
 * adjust a pwm signal on OC0 pin with duty cycle with respect to the temperature and controls
 * the direction of rotation of the motor
 */
void DCMOTOR_rotate( DcMotor_State state , uint8 speed);


#endif /* MOTOR_H_ */
