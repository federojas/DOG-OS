#ifndef COMMANDS_H
#define COMMANDS_H

#include <shell.h>

void help(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getCurrentDayTime(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getCPUFeatures(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getInfoReg(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void divZero(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void opCode(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getRoots(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void clear(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void exit(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getMem(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void changeUser(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE], char userName[USER_SIZE]);
void _opcodeExp(void);
void getCPUVendor(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void logo(void);
void changeColour(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE], int bg);
void _quadratic(double * a, double * b, double * c, double * root1, double * root2);
#endif