#ifndef PRINTS_H
#define PRINTS_H

#include <colors.h>
#include <stdint.h>

void sys_write(char * str, uint8_t len, t_color bgColor, t_color ftColor);
void printIntDec(uint64_t num);
void printIntHex(uint64_t num);
void printIntBin(uint64_t num);
uint64_t sys_read(uint64_t buffer, uint64_t lenght);
int strlen(char *str);
void printLine();
#endif