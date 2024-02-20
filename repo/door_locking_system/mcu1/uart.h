/*
 * uart.h
 *
 *  Created on: Oct 30, 2022
 *      Author: hp
 */



#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                      Definitions                                            *
 *******************************************************************************/
typedef enum
{
	rate_1200=1200, rate_2400=2400,rate_4800=4800 , rate_9600=9600 , rate_14400=14400 , rate_19200=19200 , rate_38400=38400, rate_57600=57600

}UART_BaudRate;
typedef enum
{
	disable_parity=0 , enable_even_parity=2 , enable_odd_parity=3

}UART_Parity;
typedef enum
{
	single_stop_bit , double_stop_bit
}UART_StopBit;
typedef enum
{
	mode_5_bits_data ,mode_6_bits_data,mode_7_bits_data,mode_8_bits_data,none1,none2,none3,mode_9_bits_data
}UART_BitData;
typedef struct
{
	UART_BitData bit_data;
	UART_Parity parity;
	UART_StopBit stop_bit;
	UART_BaudRate baud_rate;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */


