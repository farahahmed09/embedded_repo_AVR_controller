

#include"gpio.h"
#include"PWM_timer.h"
#include"DC_motor.h"
#include"servo_motor.h"
#include"uart.h"
#include"adc.h"
#include"timer1.h"
#include"buzzer.h"
#include"std_types.h"
#include <avr/io.h>
#include <util/delay.h>



#define MCU1_Ready 1
#define MCU2_Ready 2
/*******************************************************************************
 *                        Definitions & Variables                              *
 *******************************************************************************/
typedef enum
{
	normal_state,emergency_state,abnormal_state
}state_Type;


boolean stop_the_machine;
uint8 read_temp=0;
uint8 tick;
state_Type state=normal_state;  //initially
uint16 potentiometer_Value=0;

/* initialization for timer module */
Timer1_ConfigType timer_config = {  0 ,7813, 0x05,CTC_Mode};
/*
 * enter call back every one second
 * prescaler 1024
 * compare mode
 */

/********************************************************************************
 * 			             	functions prototype
 ********************************************************************************/
void light_red(void);
void light_green(void);
void light_yellow(void);
void program_flow(void);
void finish_seven_counts(void);




/********************************************************************************
 * 			             	functions definition
 ********************************************************************************/



void light_green(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);
}

void light_yellow(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_HIGH);
	GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_LOW);
}

void light_red(void)
{
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN6_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN7_ID, LOGIC_HIGH);
}



void finish_seven_counts(void)
{
	tick++;
}

void program_flow(void)
{


		while(UART_recieveByte()!=MCU1_Ready)
		{ }

		UART_sendByte(MCU2_Ready);
		read_temp=UART_recieveByte();

		if(read_temp<20)
		{
			light_green();

		}
		else if(read_temp<40)
		{
			light_yellow();

		}
		else if(read_temp<=50)
		{
			light_red();

			while(UART_recieveByte()!=MCU1_Ready)
			{ }

			UART_sendByte(MCU2_Ready);
			stop_the_machine=UART_recieveByte();
			if(stop_the_machine==TRUE)
				DcMotor_Rotate(0);

		}
		else
		{
			light_red();
			Buzzer_on();
			Timer1_init(&timer_config);
			tick=0;
			while (read_temp > 50)
			{
				while(UART_recieveByte()!=MCU1_Ready)
				{ }

				UART_sendByte(MCU2_Ready);
				read_temp=UART_recieveByte();
				if (tick >= 7)
				{
					rotate_Servo(187);
					DcMotor_Rotate(0);
					while(tick<13)
					{
					}
					break;
				}
			}
			tick= 0;
			Buzzer_off();
			Timer1_deInit();
			rotate_Servo(124);
		}
}

void normal_flow(void)
{

}
void emergency_flow(void)
{

}

void abnormal_flow(void)
{

}
int main()
{
	/* Enable Global Interrupt I-Bit */
	SREG |= (1 << 7);


	/* Initialize the DC Motor */
	DcMotor_Init();

	/* Initialize the Servo Motor */
	GPIO_setupPinDirection(PORTD_ID, PIN7_ID, PIN_OUTPUT);
	rotate_Servo(124);	/* Set Servo shaft at 0° position by 1 ms pulse */

	/* initialize Buzzer driver */
	Buzzer_init();

	/*adc driver*/
	ADC_ConfigType adc_config={ 0X03 , 0X03};
	ADC_init(&adc_config);


	/*uart driver*/
	UART_ConfigType uart_config ={0x03,0,0,9600};
	UART_init(&uart_config);

	Timer1_setCallBack(finish_seven_counts);



	/* initialize leds ports*/
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);//green
	GPIO_setupPinDirection(PORTB_ID, PIN6_ID, PIN_OUTPUT);//yellow
	GPIO_setupPinDirection(PORTB_ID, PIN7_ID, PIN_OUTPUT);//red

	while(1)
	{
		program_flow();
	}
}
