#ifndef KEYBOARDDRIVER_H
#define KEYBOARDDRIVER_H

#include <stdint.h>

void keyboardHandler();
void putCharInBuffer(char c);
uint64_t* getRegisters();
void dumpBuffer(char *dest, int size);
char removeCharFromBuffer();
#endif