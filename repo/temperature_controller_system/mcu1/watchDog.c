
#include"watchDog.h"
#include "avr/io.h" /* To use the UART Registers */
#include <util/delay.h>

//	function to enable Watchdog timer.
void WDT_ON(void)
{
	// Watchdog timer enables with timeout period 16.3 millisecond.
	WDTCR = (1<<WDE);
}

// 	function to disable Watchdog timer.
void WDT_OFF(void)
{
	// Set the WDTOE & WDE bits in the same operation
	WDTCR = (1<<WDTOE)|(1<<WDE);
	// Wait 4 cycles before clear the WDE bit
	_delay_us(4);
	WDTCR = 0x00;
}

