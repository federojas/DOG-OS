#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <processManagerQueue.h>

#define FOREGROUND 1
#define BACKGROUND 0

void initializeProcessManager();
int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground, uint16_t * fd);
uint64_t killProcess(uint64_t pid);
uint64_t blockProcess(uint64_t pid);
uint64_t readyProcess(uint64_t pid);
void _callTimerTick();
void * processManager(void * sp);
void printProcessStatus();
void yield();
uint64_t setState(uint64_t pid, t_state newState);
uint64_t getProcessPID();
void setPriority(uint64_t pid, int newPriority);

#endif