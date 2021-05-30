#include <videoDriver.h>
#include <prints.h>
#include <keyboardDriver.h>
void printLine() {
	newLine();
}

void printString(char * str) {
    printStringColor(str, BLACK,WHITE);
}


static int strlen(char *str) {
      int size = 0;
      for (int i = 0; str[i] != 0; i++) {
            size++;
      }
      return size;
}

void printStringColor(char * str, t_color bgColor, t_color ftColor) {
    sys_write(str, strlen(str), bgColor, ftColor);
}

void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor) {
    if (str == 0 ||  len <= 0 || bgColor < 0 || ftColor < 0) 
        return ;
    
    for (int i = 0; str[i] != 0 && i < len; i++) 
        printChar(str[i], ftColor, bgColor, 1);     
}


void putChar(char c){
    printChar(c,WHITE,BLACK,1);
}

void sys_read(uint64_t buffer, uint64_t lenght){
	if(lenght==0)
		return;
		char *buff=(char* ) buffer;//casteo para evitar el warning

	dumpBuffer(buff,lenght);
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
    printString(buffer);
}

void printIntHex(uint64_t num) {
    char buffer[10];
    uintToBase(num, buffer, 16);
    printString(buffer);
}

void printIntBin(uint64_t num) {
    char buffer[10];
    uintToBase(num, buffer, 2);
    printString(buffer);
}

