#include <naiveConsole.h>
#include <stdint.h>

void printCurrentTime() {
	ncPrint("Tiempo actual: ");
	int x = getRTCInfo(0x04);
	int result = ((x / 16) * 10) + (x & 0xf) - 3;
	ncPrint("Hora: ");
	ncPrintDec(result);

	x = getRTCInfo(0x02);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrint(" Minuto: ");
	ncPrintDec(result);

	x = getRTCInfo(0x00);
	result = ((x / 16) * 10) + (x & 0xf);
	ncPrint(" Segundo: ");
	ncPrintDec(result);
	ncNewline();
}