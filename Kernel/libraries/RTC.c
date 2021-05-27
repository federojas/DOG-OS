#include <RTC.h>
#include <naiveConsole.h>
#include <stdint.h>

#define YEAR 20 //Debe escribir aca los digitos de su año (excepto los ultimos dos)
#define DAY_RTC_ID 0x07
#define MONTH_RTC_ID 0x08
#define YEAR_RTC_ID 0x09
#define HOUR_RTC_ID 0x04
#define MINUTE_RTC_ID 0x02
#define SECOND_RTC_ID 0x00

void printCurrentTime()
{
	ncPrint("Fecha de hoy: "); //DESPUES REEMPLAZAR POR NUESTRO PRINT CON DRIVER DE VIDEO
	uint8_t x = _getRTCInfo(DAY_RTC_ID);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result); //DESPUES REEMPLAZAR POR NUESTRO PRINT CON DRIVER DE VIDEO
	ncPrint("/");

	x = _getRTCInfo(MONTH_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result);
	ncPrint("/");

	x = _getRTCInfo(YEAR_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(YEAR);
	ncPrintDec(result);
	ncNewline();

	ncPrint("Hora: ");
	x = _getRTCInfo(HOUR_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result);
	ncPrint(":");

	x = _getRTCInfo(MINUTE_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
	if(result < 10){
		ncPrintDec(0);
	}
	ncPrintDec(result);
	ncPrint(":"); 

	x = _getRTCInfo(SECOND_RTC_ID);
	result = ((x / 16) * 10) + (x & 0xf);
		if(result < 10){
		ncPrintDec(0);
	}
	ncPrintDec(result); 
	ncNewline();		
}