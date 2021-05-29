#ifndef videoDriver_H
#define videoDriver_H
#include <screens.h>
#include <colors.h>

//PRE TP MODO TEXTO
// void printChar(char character, int colour);
// void printString(const char * string);

void putPixel(int x, int y, int colour);
void printChar(char c, t_color fontColour,t_color bgColour, int stop);
void initializeVideo();
void clearScreen();
void newLine();
void deleteChar();

#endif