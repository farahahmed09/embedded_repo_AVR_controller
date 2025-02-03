/*
 * buzzer.h
 *
 *  Created on: Oct 29, 2022
 *      Author: farah ahmed
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                      definitions                                           *
 *******************************************************************************/


#define BUZZER_PORT_ID  PORTD_ID
#define BUZZER_PIN_ID   PIN7_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void Buzzer_init(void);
void Buzzer_on(void);
void Buzzer_off(void);

#endif /* BUZZER_H_ */
