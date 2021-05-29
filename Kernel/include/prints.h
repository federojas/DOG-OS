#ifndef PRINTS_H
#define PRINTS_H

#include <colors.h>
#include <stdint.h>

void printString(char * str);
void printStringColor(char * str, t_color bgColor, t_color ftColor );
void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor);
void printIntDec(uint64_t num);
void printIntHex(uint64_t num);
void printIntBin(uint64_t num);
void putChar(char c);
void printLine();

#endif