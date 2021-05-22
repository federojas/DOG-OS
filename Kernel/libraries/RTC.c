#include <RTC.h>
#include <naiveConsole.h>
#include <stdint.h>

void printCurrentTime()
{
	ncPrint("Fecha de hoy: "); //DESPUES REEMPLAZAR POR NUESTRO PRINT CON DRIVER DE VIDEO
	uint8_t x = _getRTCInfo(0x07);
	uint8_t result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result); //DESPUES REEMPLAZAR POR NUESTRO PRINT CON DRIVER DE VIDEO
	ncPrint("/");

	x = _getRTCInfo(0x08);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result);
	ncPrint("/");

	x = _getRTCInfo(0x09);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result);
	ncNewline();

	ncPrint("Tiempo actual: ");
	x = _getRTCInfo(0x04);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrintDec(result);
	ncPrint(":");

	x = _getRTCInfo(0x02);
	result = ((x / 16) * 10) + (x & 0xf);
	if(result < 10){
		ncPrintDec(0);
	}
	ncPrintDec(result);
	ncPrint(":"); 
	
	x = _getRTCInfo(0x00);
	result = ((x / 16) * 10) + (x & 0xf);
		if(result < 10){
		ncPrintDec(0);
	}
	ncPrintDec(result); 
	ncNewline();		
}