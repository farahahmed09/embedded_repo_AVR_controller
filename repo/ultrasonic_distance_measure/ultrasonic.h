/*
 * ultrasonic.h
 *
 *  Created on: Oct 12, 2022
 *      Author: farah ahmed
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "std_types.h"


#define TRIGGER_PORT_ID  PORTB_ID
#define TRIGGER_PIN_ID    PIN5_ID







/**************************************************************************************************
 *                            functions prototype
 ***************************************************************************************************/
/*
 *  - initialize icu by calling its init function.
 *  - pass the address of the function to be called back by icu.
 *  - set trigger pin to be output pin which is used to generate the initial
 *     pulse that start the sensor.
 */
void Ultrasonic_init(void);

/*
 * send the initial pulse through the trigger pin.
 *
 */
void Ultrasonic_trigger(void);

/*
 * - call the ultrasonic_trigger funtion to send the initial pulse.
 * - start measurements by the icu.
 * - return the calculated distance .
 *
 */
uint16 Ultrasonic_readDistance(void);

/*
 * - this is the called back function by the icu driver
 * - the function calculates the high time (pulse) that is returned
 *    by the ultrasonic sensor through the echo pin .
 *
 */
void Ultrasonic_edgeProcessing(void);

#endif /* ULTRASONIC_H_ */
