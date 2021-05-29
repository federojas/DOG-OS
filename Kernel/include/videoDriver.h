#ifndef videoDriver_H
#define videoDriver_H
#include <screens.h>
#include <colors.h>

//PRE TP MODO TEXTO
// void printChar(char character, int colour);
// void printString(const char * string);

void putpixel(int x, int y, int colour);
void printchar(char c, t_color fontColour,t_color bgColour, int stop);
void initialize();
void clearScreen();
void newline();
void deleteChar();

#endif