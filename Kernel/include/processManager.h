#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

void initializeProcessManager();
int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground, uint16_t * fd);

#endif