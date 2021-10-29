#ifndef COMMANDS_H
#define COMMANDS_H

#include <shell.h>

void getCurrentDayTime(int argc, char ** argv);
void getCPUFeatures(int argc, char ** argv);
void getInfoReg(int argc, char ** argv);
void divZero(int argc, char ** argv);
void opCode(int argc, char ** argv);
void getRoots(int argc, char ** argv);
void clear(int argc, char ** argv);
void exit(int argc, char ** argv);
void getMem(int argc, char ** argv);
void _opcodeExp(void);
void getCPUVendor(int argc, char ** argv);
void logo(int argc, char ** argv);
void changeFtColour(int argc, char ** argv);
void changeBgColour(int argc, char ** argv);
void _quadratic(double * a, double * b, double * c, double * root1, double * root2);
void semStatusWrapper(int argc, char ** argv);
void memStatusWrapper(int argc, char ** argv);
void processStatusWrapper(int argc, char ** argv);
void setPriorityWrapper(int argc, char ** argv);
void killProcessWrapper(int argc, char ** argv);
void blockProcessWrapper(int argc, char ** argv);
void testPriorityWrapper(int argc, char ** argv);
void testSyncWrapper(int argc, char ** argv);
void testNoSyncWrapper(int argc, char ** argv);
void testProcessesWrapper(int argc, char ** argv);
void testMemoryWrapper(int argc, char ** argv);
void unblockProcessWrapper(int argc, char ** argv);
void cat(int argc, char ** argv);
void loop(int argc, char ** argv);
void pipeStatusWrapper(int argc, char ** argv);
void filter(int argc, char **argv);
void wc(int argc, char ** argv);
int checkArgcWrapper(int argc, int argumentsPermitted);
#endif