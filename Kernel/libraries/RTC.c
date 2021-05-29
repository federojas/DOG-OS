#include <RTC.h>
#include <stdint.h>
#include <prints.h>

#define YEAR 20 //Debe escribir aca los digitos de su año (excepto los ultimos dos)
#define DAY_RTC_ID 0x07
#define MONTH_RTC_ID 0x08
#define YEAR_RTC_ID 0x09
#define HOUR_RTC_ID 0x04
#define MINUTE_RTC_ID 0x02
#define SECOND_RTC_ID 0x00

// https://wiki.osdev.org/CMOS#Format_of_Bytes
void printCurrentTime()
{
	printString("Fecha de hoy: ");
	uint8_t x = _getRTCInfo(DAY_RTC_ID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	printIntDec(result);
	putChar('/');

	x = _getRTCInfo(MONTH_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	printIntDec(result);
	putChar('/');

	x = _getRTCInfo(YEAR_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	printIntDec(YEAR);
	printIntDec(result);
	newLine();

	printString("Hora: ");
	x = _getRTCInfo(HOUR_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	printIntDec(result);
	putChar(':');

	x = _getRTCInfo(MINUTE_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	if(result < 10){
		printIntDec(0);
	}
	printIntDec(result);
	putChar(':'); 

	x = _getRTCInfo(SECOND_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
		if(result < 10){
		printIntDec(0);
	}
	printIntDec(result); 
	newLine();		
}