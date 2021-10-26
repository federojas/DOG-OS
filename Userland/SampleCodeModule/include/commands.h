#ifndef COMMANDS_H
#define COMMANDS_H

#include <shell.h>

void getCurrentDayTime(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getCPUFeatures(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getInfoReg(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void divZero(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void opCode(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getRoots(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void clear(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void exit(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void getMem(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void _opcodeExp(void);
void getCPUVendor(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void logo(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void changeFtColour(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void changeBgColour(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void _quadratic(double * a, double * b, double * c, double * root1, double * root2);
void semStatusWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void memStatusWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void processStatusWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void setPriorityWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void killProcessWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void blockProcessWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void testPriorityWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void testSyncWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void testNoSyncWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void testProcessesWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void testMemoryWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void unblockProcessWrapper(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void cat(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
void loop(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]);
#endif