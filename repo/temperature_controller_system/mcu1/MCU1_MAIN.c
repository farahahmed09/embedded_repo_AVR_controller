#include"gpio.h"
#include"PWM_timer.h"
#include"DC_motor.h"
#include"Internal_EEPROM.h"
#include "adc.h"
#include "lm35_sensor.h"
#include"uart.h"
#include"timer1.h"
#include"watchDog.h"
#include"std_types.h"
#include <avr/io.h>
#include <util/delay.h>

/**************************************************************************************
 * 									defines
 ***********************************************************************************/
#define MCU1_Ready        0x01
#define MCU2_Ready        0x02

typedef enum
{
	normal_state,emergency_state,abnormal_state
}state_Type;


boolean stop_the_machine;
uint8 read_temp=0;;
uint8 tick;
state_Type state=normal_state;  //initially
uint8 rotation_speed=0;

/* initialization for timer module */
Timer1_ConfigType timer_config = {  0 ,7813, 0x05,CTC_Mode};
/*
 * enter call back every one second
 * prescaler 1024
 * compare mode
 */

/**************************************************************************************
 * 									functions prototype
 ***********************************************************************************/
void finish_seven_counts(void);
void program_flow(void);
void emergency_flow(void);
void abnormal_flow(void);


/**************************************************************************************
 * 									functions definition
 ***********************************************************************************/
void finish_seven_counts(void)
{
	tick++;
}

void program_flow(void)
{
	read_temp = LM35_getTemperature();
	UART_sendByte(MCU1_Ready);

	while(UART_recieveByte()!=MCU2_Ready)
	{

	}
	UART_sendByte(read_temp);

	if (state == normal_state)
	{
		if (read_temp < 20)
		{
			DcMotor_Rotate(0);
			state=normal_state;
		}
		else if (read_temp < 40 )
		{
			rotation_speed = (read_temp) ;
			DcMotor_Rotate(rotation_speed);
			state=normal_state;
		}
		else if (read_temp <= 50)
		{
			DcMotor_Rotate(100);
			if(!GPIO_readPin(PORTB_ID, PIN7_ID))
				stop_the_machine=TRUE;
			else
				stop_the_machine=FALSE;

			UART_sendByte(MCU1_Ready);

			while(UART_recieveByte()!=MCU2_Ready)
			{

			}
			UART_sendByte(stop_the_machine);
			state=normal_state;
		}
		else
		{

			state = emergency_state;
		}
	}
	else if (state == emergency_state)
	{
		emergency_flow();
	}
	else
	{
		//abnormal_state
		abnormal_flow();

	}

}
void emergency_flow(void)
{
	DcMotor_Rotate(100);
	Timer1_init(&timer_config );
	tick=0;
	while (read_temp > 50)
	{
		read_temp = LM35_getTemperature();

		UART_sendByte(MCU1_Ready);

		while(UART_recieveByte()!=MCU2_Ready)
		{

		}
		UART_sendByte(read_temp);
		if (tick >= 7)
		{
			state = abnormal_state;
			WDT_ON();
		}
	}
	tick = 0;
	Timer1_deInit();

}

void abnormal_flow(void)
{
	DcMotor_Rotate(100);
}

int main()
{
	/*global interrupts*/
	SREG |= (1 << 7);

	/*initialize of drivers*/

	Timer1_setCallBack(finish_seven_counts);

	/* Initialize the DC Motor */
	DcMotor_Init();

	/*adc driver*/
	ADC_ConfigType adc_config={ 0X03 , 0X03};
	ADC_init(&adc_config);


	/*uart driver*/
	UART_ConfigType uart_config ={0x03,0,0,9600};
	UART_init(&uart_config);


	while(1)
	{
		program_flow();
	}


}
