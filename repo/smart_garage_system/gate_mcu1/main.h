/*
 * main.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Farah
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>
#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"
#include "keypad.h"
#include "lcd.h"
#include "motor.h"
#include "pir_sensor.h"
#include "pwm_timer0.h"
#include "uart.h"



/******************************************************************************************
 * 								pre defined parameters
 *****************************************************************************************/
#define ID_CHAR 5           // Max characters for user ID + null terminator
#define NUM_OF_USERS 5      // Total predefined users
#define NOT_ASSIGNED -1     // Represents no assigned slot
#define NUM_OF_SLOTS 2      // Total number of slots
#define ASSIGNED_TO_S1  0x11
#define ASSIGNED_TO_S2  0x15

/* states */
typedef enum
{
    IDLE,
    ACTION,
    ENTER_ID,
    CHECK_ID,
	CHOOSE_SLOT,
    OPEN_THE_GATE,
    CLOSE_THE_GATE,
    ERROR,
}enum_states;

/* USERS AND IDs */
typedef struct {
    uint8 userID[ID_CHAR];   // User ID (string)
    uint16 user_slot_num;      // Assigned slot number (-1 if no slot assigned)
}str_users;
str_users arrUsers[NUM_OF_USERS]; /* instance of pre defined users */

/* instance of the pir used */
pirConfig pir3={on ,SENSOR_3_INPUT_PORT_ID , SENSOR_3_INPUT_PIN_ID };


/* LCD OPTIONS */
#define ENTRY     0X22
#define EXIT      0x21
#define AVAILABLE_SLOTS   0x23

/* UART HANDSHAKE  */



/************************************************************************************************
 * 									functions prototype
 ************************************************************************************************/
void SystemFlow(enum_states state);
void InitSystem(void);
void EnterID(uint8 * arr);
sint8 checkUserID(uint8 *enteredID);
void chooseSlot(uint8 user_index);
void Display(void);
void EntryOrExit(void);
void displayAvailableSlotsOnLCD(void);

#endif /* MAIN_H_ */
