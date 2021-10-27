#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <processManagerQueue.h>

#define FOREGROUND 1
#define BACKGROUND 0

void initializeProcessManager();
int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground, uint16_t * fd);
int killProcess(uint64_t pid);
int blockProcess(uint64_t pid);
int readyProcess(uint64_t pid);
void _callTimerTick();
void * processManager(void * sp);
void printProcessStatus();
void yield();
int setState(uint64_t pid, t_state newState);
int getProcessPID();
void setPriority(uint64_t pid, int newPriority);
void printCurrentProcess();
void killCurrentFGProcess();

#endif