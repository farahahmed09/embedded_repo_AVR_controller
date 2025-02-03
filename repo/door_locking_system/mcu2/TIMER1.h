/*
 * TIMER1.h
 *
 *  Created on: Oct 24, 2022
 *      Author: farah ahmed
 */
#include "std_types.h"
#ifndef TIMER1_H_
#define TIMER1_H_


/*******************************************************************************
 *                                 Definitions                                 *
 *******************************************************************************/


typedef enum
{
	no_clock,no_prescaling,fcpu_8,fcpu_64,fcpu_256,fcpu_1024,external_falling_clock,external_rising_clock

}Timer1_Prescaler;
typedef enum
{
	normal_mode , compare_mode
}Timer1_Mode;

typedef struct
{
	uint16 initial_value;
	Timer1_Mode mode;
    Timer1_Prescaler prescaler;
    uint16 compare_value;


}Timer1_ConfigType;


/*******************************************************************************
 *                      Functions prototypes                                 *
 *******************************************************************************/

void Timer1_init(const Timer1_ConfigType * Config_Ptr);
void Timer1_deInit(void);
void Timer1_setCallBack(void(*a_ptr)(void));
#endif /* TIMER1_H_ */
