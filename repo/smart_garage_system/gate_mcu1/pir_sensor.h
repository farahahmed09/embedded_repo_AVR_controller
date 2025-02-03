 /******************************************************************************
 *
 * Module: PIR SENSOR
 *
 * File Name: pir_sensor.h
 *
 * Description: Header file for the PIR sensor driver
 *
 * Author: Nada Hamed
 *
 *******************************************************************************/

#ifndef PIR_SENSOR_H_
#define PIR_SENSOR_H_

#include "std_types.h"
#include "gpio.h"
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* PIR sensors port configurations */
/* first instance */
#define SENSOR_1_INPUT_PORT_ID               (uint8*)PORTB_ID
#define SENSOR_1_INPUT_PIN_ID                (uint8*)PIN1_ID
/* second instance */
#define SENSOR_2_INPUT_PORT_ID               (uint8*)PORTB_ID
#define SENSOR_2_INPUT_PIN_ID                (uint8*)PIN2_ID
/* third instance */
#define SENSOR_3_INPUT_PORT_ID               (uint8*)PORTB_ID
#define SENSOR_3_INPUT_PIN_ID                (uint8*)PIN3_ID

typedef enum
{
	off,on,
}pir_state;
typedef struct
{
	pir_state state ;
	uint8 pir_input_port;
	uint8 pir_input_pin;
}pirConfig;
extern pirConfig pir1;
extern pirConfig pir2;
extern pirConfig pir3;

/* Keypad button logic configurations */
#define MOTION_DETECTED                 LOGIC_HIGH
#define MOTION_NOT_DETECTED             LOGIC_LOW

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * initialize two PIR sensors.
 */
void PIR_init(pirConfig *str);

/*
 * Description :get readings of sensor returns 1 if detected motion
 * returns 0 if there's no motion .
 */
uint8 detect_motion(pirConfig *str);



#endif /* PIR_SENSOR_H_ */
