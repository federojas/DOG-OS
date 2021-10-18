#ifndef PRINTS_H
#define PRINTS_H

#include <colors.h>
#include <stdint.h>
#include <stdarg.h>

void printIntDec(uint64_t num);
void printIntHex(uint64_t num);
void printIntBin(uint64_t num);
int strlen(char *str);
void printLine();
void strcpy(char dest[], const char source[]);
int log2(uint32_t number);
int pow(int x, unsigned int y);
int printf(char const *fmt, ...);
static int scan(char const *fmt, va_list arg);
void putChar(char c);
char* intToStr(int value, char* buffer, int base);

#endif