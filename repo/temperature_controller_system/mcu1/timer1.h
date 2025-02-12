/*
 * timer.h
 *
 *  Created on: Nov 3, 2022
 *
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"
#include "gpio.h"


/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
typedef uint8  Timer1_Prescaler;
typedef enum
{
	OVF_Mode,CTC_Mode
}Timer1_Mode;
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef struct
{
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));

#endif /* TIMER1_H_ */
