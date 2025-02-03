/*
 * mc1_main.c
 *
 *  Created on: Nov 1, 2022
 *      Author: farah ahmed
 */

#include "uart.h"
#include "lcd.h"
#include "gpio.h"
#include "common_macros.h"
#include "keypad.h"
#include "TIMER1.h"
#include <util/delay.h>
#include <avr/io.h>

#define ERROR                 0x00
#define MC1_READY             0X01
#define MC2_READY             0X02
#define DONE                  0X03
#define FAILURE               0X04
#define MATCHED               0x05
#define UNLOCK_THEN_LOCK      0X06
#define CHECK_SAVE            0X07
#define CHECK                 0x08


/*******************************************************************************
 *                   global variables definitions and declarations            *
 *******************************************************************************/

uint8 a_firstpass[5]; /*Holds the first entered password*/
uint8 a_secondpass[5]; /*Holds the second entered password*/
uint8 login_pass[5];  /* pass needed to login to the system */
uint8 error_count=0;  /*counts how many times the user entered wrong pass */
uint8 error_flag=0;   /* flag for interrupts of timer */
unsigned short tick=0;  /* count how many times an interrupt occurs */
uint8 check_for_matching; /* to check on a received byte from mc2 */


/* timer module initializations */
TIMER1_ConfigType timer_config = {0 , normal_mode ,fcpu_8};


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
uint8 change_pass(void);
/*
 *  -> function to change the saved password in the system
 *  ->also called at the beginning of the program for the first creation
 *    of password
 *  -> it returns matched if : the first entry of password
 *  matches the second entry for ensuring
 *  ->it returns failure if : the first entry of password does not matches
 *  the second entry , so it asks the user to enter again the password twice
 *   for ensuring
 *
 */
void create_password(uint8 *a_firstPass,uint8 *a_secondPass);
/*
 *  ->function if the user wanted to create a password
 *  ->it takes the password and the re-entered password from the user
 *  and store them in an array
 *
 */
void take_pass(uint8 * arr);
/*
 * function that take the password from the user and store it in
 *  an array
 */
void MC1_sendPass(uint8 *pass);
/*
 * function responsible for sending the password which the user has
 * entered to the mc2 to check on it or to login to the system
 */
uint8 main_options(void);
/*
 * ->function that display the main options for the system
 * and wait till the user choose an option
 *
 * -> the function returns the chosen option through keypad
 *
 */
void open_door(void);
/*
 * function is called when the user entered the correct password for
 * login and choose option '+' from the main_options
 */
uint8 login(void);
/*
 * ->function responsible to take the password needed to login into the system
 * and send it to the mc2 by uart through function MC1_sendPass()
 * for the mc2 to check if its matched with the saved one in the system
 *
 * ->the function returns matched or failure
 */
void error_happened(void);
/*
 * ->function is called when the user enter a wrong password for login
 *   more than 3 times
 *
 *   ->the functions displays error msg on the lcd screen
 */
void count_one_min(void);
/*
 * the called back function when the timer gives an interrupt to
 * the mc1
 */





/**************************************************************************************/


int main()
{

	/*******************************************************************************
	 *                                 modules initializations                      *
	 *******************************************************************************/

	SREG=(1<<7);
	LCD_init();
	UART_ConfigType uart_config = { mode_8_bits_data ,enable_odd_parity,double_stop_bit , rate_9600 };
	UART_init(&uart_config);

	while(change_pass()!=DONE)
	{
		/*
		 * wait till the first two entry of passwords
		 * give a match
		 */

	}
	while(1)
	{
		switch(main_options())
		{
		case'+':
			error_count=0;
			while(login()!=MATCHED)
			{
				error_count++;
				if(error_count==3)
				{
					/* the user has entered wrong password
					 * more than 3 timer so an error must be displayed
					 * on lcd screen and an error byte must be send
					 * to the contro_ECU
					 */
					UART_sendByte(MC1_READY);
					while(UART_recieveByte()!= MC2_READY)
					{
						/* wait for mc2 to be ready */
					}

					UART_sendByte(ERROR);
					/*start the timer to count */
					Timer1_init(&timer_config);
					Timer1_setCallBack(count_one_min);

					error_happened();
					break;
				}

			}
			if(error_count!=3)
				/*
				 * if the login password matches the saved one
				 * and the user does not entered a wrong pass more than
				 * 3 times
				 *
				 * system should unlock the door
				 */
				open_door();
			break;
		case'-':
			error_count=0;
			while(login()!=MATCHED)
			{
				error_count++;
				if(error_count==3)
				{
					/* the user has entered wrong password
					 * more than 3 timer so an error must be displayed
					 * on lcs screen and an error msg must be send to
					 * the contro_ECU
					 */
					UART_sendByte(MC1_READY);
					while(UART_recieveByte()!= MC2_READY)
					{
						/* wait for mc2 to be ready */
					}

					UART_sendByte(ERROR);
					/*start the timer to count */
					Timer1_init(&timer_config);
					Timer1_setCallBack(count_one_min);
					error_happened();
					break;
				}
			}

			if(error_count!=3)
				/*
				 * if the login password matches the saved one
				 * and the user does not entered a wrong pass more than
				 * 3 times
				 *
				 * system should allow the user to change the password
				 * then after finishing the main options screen must be
				 * displayed
				 */

				change_pass();
			break;
		}
	}
}
/*******************************************************************************
/ *                      functions definitions                                  *
/ *******************************************************************************/

uint8 change_pass(void)
{
	/*
	 * function to create a password through function create_password()
	 *
	 *
	 * return done if the two passwords are matched and the
	 * control_ecu saved the password in the eeprom
	 */
	LCD_clearScreen();
	create_password(a_firstpass,a_secondpass);
	UART_sendByte(MC1_READY);
	while(UART_recieveByte()!= MC2_READY)
	{
		/*wait till mc2 is ready */
	}
	UART_sendByte(CHECK_SAVE);/*send the action needed to be done */
	MC1_sendPass(a_firstpass);
	MC1_sendPass(a_secondpass);
	while(UART_recieveByte()!=MC2_READY)
	{

	}
	UART_sendByte(MC1_READY);
	return UART_recieveByte();

}


void create_password(uint8 *a_firstPass,uint8 *a_secondPass)
{
	LCD_displayStringRowColumn(0,0,"plz enter pass");
	LCD_moveCursor(1,0);
	take_pass(a_firstPass);/* take first pass entered from user and store it in array */
	LCD_displayStringRowColumn(0,0,"plz re-enter the");
	LCD_displayStringRowColumn(1,0,"same pass: ");
	/*take the re-entered pass from the user */
	take_pass(a_secondPass);

}
void take_pass(uint8 * arr)
{
	uint8 i;
	uint8 key;
	for(i=0 ; i<5;i++)
	{
		key=KEYPAD_getPressedKey();
		if(key>=0 && key<=9)
		{
			arr[i]=key;
			_delay_ms(400);
			LCD_displayCharacter('*');
		}
		else i--;
	}
	while(KEYPAD_getPressedKey()!=ENTER)
	{
		/*wait till user press enter button */
	}

}
void MC1_sendPass(uint8 *pass)
{

	/*
	 * send the password byte-byte to the
	 * control_ECU
	 */
	uint8 i;
	for(i=0 ; i<5 ; i++)
	{
		UART_sendByte(MC1_READY);
		while(UART_recieveByte()!= MC2_READY)
		{
			/*wait till mc2 is ready */
		}
		UART_sendByte(pass[i]);
	}
}
uint8 main_options(void)
{
	/*
	 * displays the options on screen
	 *
	 * return the pressed key option
	 */
	uint8 key;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"+ : open door");
	LCD_displayStringRowColumn(1,0,"- : change pass");
	key=KEYPAD_getPressedKey();

	return  key;

}
void open_door(void)
{
	UART_sendByte(MC1_READY);
	while(UART_recieveByte()!=MC2_READY)
	{
		/*wait till mc2 is ready */
	}
	/* send to the control_ECU
	 * the action needed to be done
	 */
	UART_sendByte(UNLOCK_THEN_LOCK);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"door's unlocking");
	while(UART_recieveByte()!=MC2_READY)
	{
		/*wait till mc2 is ready */
	}
	UART_sendByte(MC1_READY);
	if(UART_recieveByte()==DONE)
	{
		/*
		 * if the login pass matches the saved one in the
		 * system
		 */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"door's locking");
	}
	while(UART_recieveByte()!=MC2_READY)
	{

	}
	UART_sendByte(MC1_READY);
	if(UART_recieveByte()==DONE)
		return;
}
uint8 login(void)
{
	/*
	 * takes login pass from the user and send
	 * it to the control_ECU by uart to check on them if they
	 * are matched and return matched or failure
	 */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"plz enter pass");
	LCD_moveCursor(1,0);
	take_pass(login_pass);
	UART_sendByte(MC1_READY);
	while(UART_recieveByte()!= MC2_READY)
	{
		/*wait till mc2 is ready */
	}
	UART_sendByte(CHECK);/*send to mc2 the action needed to be done*/
	MC1_sendPass(login_pass);
	while(UART_recieveByte()!=MC2_READY)
	{

	}
	UART_sendByte(MC1_READY);
	return UART_recieveByte();
}
void error_happened(void)
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"ERROR");
	while(!error_flag)
	{
		/*wait till the timer finishes counting*/

	}
}
void count_one_min(void)
{
	tick++;
	if(tick==915)
	{
		tick=0;
		error_flag=1;
		Timer1_deInit(); /*turn off the timer */
	}

}
