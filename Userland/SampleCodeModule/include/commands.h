#ifndef COMMANDS_H
#define COMMANDS_H

#include <shell.h>

void help(int argc, char* argv[]);
void getCurrentDayTime(int argc, char* argv[]);
void getCPUFeatures(int argc, char* argv[]);
void getInfoReg(int argc, char* argv[]);
void divZero(int argc, char* argv[]);
void opCode(int argc, char* argv[]);
// void getRoots(int argc, char* argv[], float a, float b, float c);
void clear(int argc, char* argv[]);
void exit(int argc, char* argv[]);
void getMem(int argc, char* argv[]);
void changeUser(int argc, char* argv[], char userName[USER_SIZE]);
void _opcodeExp(void);
void getCPUVendor(int argc, char* argv[]);
#endif