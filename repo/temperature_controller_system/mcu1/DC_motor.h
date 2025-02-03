

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"
#include "gpio.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DCMotor_PORT_ID                 PORTA_ID
#define DCMotor_PIN0_ID                 PIN0_ID
#define DCMotor_PIN1_ID                 PIN1_ID


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	Stop,Clockwise,Anticlockwise
}DcMotor_State;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void DcMotor_Init(void);
void DcMotor_Rotate(uint8 speed);


#endif /* DC_MOTOR_H_ */
