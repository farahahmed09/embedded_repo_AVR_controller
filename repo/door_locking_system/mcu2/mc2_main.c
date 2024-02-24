/*
 * mc2_main.c
 *
 *  Created on: Nov 1, 2022
 *      Author: farah ahmed
 */


#include "uart.h"
#include "motor.h"
#include "buzzer.h"
#include "external_eeprom.h"
#include "gpio.h"
#include "std_types.h"
#include "twi.h"
#include "TIMER1.h"
#include <avr/io.h>

#define ERROR                  0x00
#define MC1_READY              0X01
#define MC2_READY              0X02
#define DONE                   0X03
#define FAILURE                0X04
#define MATCHED                0x05
#define UNLOCK_THEN_LOCK       0X06
#define CHECK_SAVE             0X07
#define CHECK                  0x08





/*******************************************************************************
/ *                                global variables                                  *
/ *******************************************************************************/
uint8 recieved_pass1[5];/* array holds first pass */
uint8 recieved_pass2[5];/* array holds second pass */
uint8 saved_pass[5];/* array holds the saved password in eeprom */
uint8 test_arr[5]; /*array to receive any pass to check if matched with the saved one */
uint8 check_byte;
uint8 action;/* holds the received action needed to be done from mc1 */
uint8 FINISHED=0; /* flag */
unsigned short tick=0; /* to count how many times an interrupt occurs from the timer */
unsigned char error_flag=0;
uint8 count=0; /* used to check how many times [count_15_sec] function has been entered */



/* initialization for timer module */
Timer1_ConfigType timer_config = {0 , normal_mode ,fcpu_8};



/*******************************************************************************
/ *                      Functions Prototypes                                   *
/ *******************************************************************************/
void MC2_recievePass(uint8 *recieved_pass1);
/*
 * function that receives the entered password from mc1
 * and store it in an array
 */
uint8 check_matching(uint8*arr1 , uint8 *arr2);
/*
 * function takes 2 passwords and check if they are both matched or not
 *
 * function return matched or failure
 */
void count_15_sec(void);
/*
 * called back function by timer to count
 * 15 secs
 */
void count_3_sec(void);
/*
 * called back function by timer to count
 * 3 secs
 */
void count_1_min(void);
/*
 * called back function by timer to count
 * 1 min
 *
 */
void error_happened(void);
/*
 * indicates an error happened
 * it starts buzzer to give alert
 * and waits till timer finishes counting
 */
void write_eeprom(uint8 *pass);
/*
 * function takes the password if the user entered it twice correctly
 * and save it in eeprom and also store it in an array -> saved_pass[5]
 */
void read_eeprom(uint8 *saved_pass);
/*
 * read the pass stored  in memory
 * and save it in an array " saved_pass[]"
 */




int main()
{

	/*******************************************************************************
	 *                                 modules initializations                                 *
	 *******************************************************************************/

	SREG=(1<<7);
	DCMOTOR_init();
	Buzzer_init();
	UART_ConfigType uart_config = { mode_8_bits_data ,enable_odd_parity,double_stop_bit , rate_9600 };
	UART_init(&uart_config);
	TWI_ConfigType twi_config ={  my_address , fast_mode };
	TWI_init(&twi_config);



	while(1)
	{
		/* always waits till mc1 sends a byte to start communication*/
		while(UART_recieveByte()!=MC1_READY)
		{

		}
		UART_sendByte(MC2_READY);
		/*receive the action needed to be done from mc1 */
		action=UART_recieveByte();
		switch(action)
		{
		case CHECK_SAVE:
			/* receive 2 passwords and check on them if they are matched
			 * and if they are matched , save this password in the external
			 * eeprom memory
			 *
			 * then send a byte to mc1 telling if the action
			 * is done or not
			 *
			 * if they are not matched failure msg is sent to mc2
			 * and this passed password won't be saved in memory
			 */
			MC2_recievePass(recieved_pass1);
			MC2_recievePass(recieved_pass2);
			check_byte=check_matching(recieved_pass1,recieved_pass2);
			if(check_byte==MATCHED)
			{
				write_eeprom(saved_pass);
				UART_sendByte(MC2_READY);
				while(UART_recieveByte()!= MC1_READY)
				{
					/*wait for mc1 to be ready */
				}
				UART_sendByte(DONE);/*indicates that the two passwords are matched and saved
				in the memory */
			}
			else
			{
				UART_sendByte(MC2_READY);
				while(UART_recieveByte()!= MC1_READY)
				{

				}
				UART_sendByte(FAILURE);/*indicates that the two passwords sent are not matched
				and nothing is saved in the memory */

			}
			break;
		case CHECK:
			/*
			 * the action check_save above is already done and system has
			 * a password stored in memory
			 *
			 * here ,receive password from mc1 and compare it to the saved one
			 *
			 * sends to mc1 byte if matched or not
			 */
			MC2_recievePass(test_arr);
			read_eeprom(saved_pass);
			if(check_matching(saved_pass,test_arr)==MATCHED)
			{
				UART_sendByte(MC2_READY);

				while(UART_recieveByte()!= MC1_READY)
				{

				}
				UART_sendByte(MATCHED);/*indicates that the sent password matches the saved one */
			}
			else
			{

				UART_sendByte(MC2_READY);

				while(UART_recieveByte()!= MC1_READY)
				{
					/*wait for mc1 to be ready */
				}
				UART_sendByte(FAILURE);/*indicates that the sent password does not
				matches the saved one */

			}
			break;

		case UNLOCK_THEN_LOCK  :
			/*
			 * operate the dcmotor and start the timer to count
			 * for unlocking and locking the door
			 *
			 * then send a byte to the  mc1 telling if the action is
			 * done or not
			 */
			DCMOTOR_rotate(CW ,100);
			Timer1_init(&timer_config);
			Timer1_setCallBack(count_15_sec);


			while(!FINISHED)
			{
				/* wait for timer to finish counting 15 secs at first to unlock the door
				 * then holds for 3 secs
				 * then counts again 15 secs to lock the door
				 */
			}
			UART_sendByte(MC2_READY);
			while(UART_recieveByte()!= MC1_READY)
			{
				/*wait for mc1 to be ready */
			}
			UART_sendByte(DONE);/*indicates that the door has been unlocked then locked */

			break;
		case ERROR :
			/*
			 * error happened as the user entered wrong password
			 * more than 3 times ,so here starts timer to count
			 * and start the buzzer to give alert
			 */
			Timer1_init(&timer_config);
			Timer1_setCallBack(count_1_min);
			error_happened();
			break;
		}

	}
}
/*******************************************************************************
/ *                      functions definitions                                  *
/ *******************************************************************************/

void MC2_recievePass(uint8 *recieved_pass)
{
	uint8 i;
	for(i=0 ; i<5 ; i++)
	{

		while(UART_recieveByte()!=MC1_READY)
		{

		}
		UART_sendByte(MC2_READY);
		recieved_pass[i]=UART_recieveByte();

	}

}
uint8 check_matching(uint8*arr1 , uint8 *arr2)
{

	uint8 i;
	for(i=0 ; i<5 ; i++)
	{
		if(arr1[i]==arr2[i])
		{
			if(i==4)
			{
				for(uint8 k =0 ; k<5 ; k++)
				{
					saved_pass[k]=arr1[i];
				}
				break;
			}
		}
		else

			return FAILURE;

	}
	return MATCHED;

}
void count_15_sec(void)
{
	tick++;
	if(tick==228)
	{
		if(count==0)
		{

			DCMOTOR_rotate(CW ,0);
			Timer1_setCallBack(count_3_sec);
			tick=0;
			count++;
		}
		else
		{
			tick=0;
			DCMOTOR_rotate(OFF ,0);
			Timer1_deInit();
			FINISHED=1;
		}


	}

}
void count_3_sec(void)
{
	tick++;
	if(tick==45)
	{

		DCMOTOR_rotate(Anti_CW , 100);
		Timer1_setCallBack(count_15_sec);
		tick=0;
		UART_sendByte(MC2_READY);
		while(UART_recieveByte()!= MC1_READY)
		{

		}
		UART_sendByte(DONE);


	}

}
void count_1_min(void)
{
	tick++;
	if(tick==915)
	{
		error_flag=1;
		Timer1_deInit();
		Buzzer_off();
		tick=0;
	}


}
void error_happened(void)
{
	Buzzer_on();
	while(!error_flag)
	{

	}

}
void write_eeprom(uint8 *pass)
{
	uint8 j;
	for( j =0 ; j<5 ; j++)
	{
		EEPROM_writeByte(j,pass[j]);
	}

}
void read_eeprom(uint8 *saved_pass)
{
	/*get the saved password from eeprom and store it in
	 * array
	 */
	uint8 i;
	for(i=0;i<5;i++)
	{
		EEPROM_readByte(i,&saved_pass[i]);
	}
}
