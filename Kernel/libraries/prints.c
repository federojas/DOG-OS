#include <videoDriver.h>
#include <prints.h>
#include <keyboardDriver.h>
#include <syscalls.h>

void printLine() {
	newLine();
}

int strlen(char *str) {
      int size = 0;
      for (int i = 0; str[i] != 0; i++) {
            size++;
      }
      return size;
}

//FUNCION DE CATEDRA
static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void printIntDec(uint64_t num) {
   char buffer[10];
   uintToBase(num, buffer, 10);
   sys_write(buffer, strlen(buffer), BLACK, WHITE);
}

void printIntHex(uint64_t num) {
    char buffer[10];
    uintToBase(num, buffer, 16);
    sys_write(buffer, strlen(buffer), BLACK, WHITE);
}

void printIntBin(uint64_t num) {
    char buffer[10];
    uintToBase(num, buffer, 2);
    sys_write(buffer, strlen(buffer), BLACK, WHITE);
}

