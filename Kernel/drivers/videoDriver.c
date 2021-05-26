#include <videoDriver.h>
#include <stdint.h>

unsigned int defaulltBgColor = 0x0;
unsigned int defaultFontColor = 0xFFFFFF;

unsigned int SCREEN_WIDTH = 1024;
unsigned int SCREEN_HEIGHT = 768;
unsigned int PP_SIZE= 3; //size off pixel 

static uint8_t * currentVideo = (uint8_t*)0xB8000;

void printChar(char character, int colour) {
	*currentVideo = character;
	currentVideo += 1;
	*currentVideo = colour;
	currentVideo += 1;
}

void printString(const char * string) {
	int i;
	for(i=0; string[i] != 0; i++){
		printChar(string[i], 0xF2);
	}
}