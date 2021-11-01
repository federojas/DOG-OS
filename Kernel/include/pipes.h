#ifndef PIPES_H
#define PIPES_H

#define PIPE_BUFFER_SIZE 1024
#define IN_USE 1
#define EMPTY 0

typedef struct t_pipe {
  int id;
  char buffer[PIPE_BUFFER_SIZE];
  int writeIndex, readIndex;
  long totalProcesses;
  int writeLock, readLock;
  int state;
} t_pipe;

int pipeOpen(int pipeId);
int pipeWrite(int pipeId, char *str);
int pipeClose(int pipeId);
int pipeRead(int pipeId);
void pipeStatus();

#endif