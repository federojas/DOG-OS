#ifndef PROCESS_MANAGER_QUEUE_H
#define PROCESS_MANAGER_QUEUE_H

#include <stdint.h>
#include <stddef.h>
#include <prints.h>

typedef enum { READY, BLOCKED, TERMINATED } t_state;

typedef struct {

  int pid;
  int ppid;
  int foreground;
  t_state state;
  uint64_t priority;
  char name[30];
  int fileDescriptors[2];
  void *rsp;
  void *rbp;
  int argc;
  char **argv;
} t_PCB;

typedef struct t_process_node {
  t_PCB pcb;
  struct t_process_node * next;
} t_process_node;

typedef struct t_process_list {
  uint32_t size;
  uint32_t readySize;
  t_process_node *first;
  t_process_node *last;
} t_process_list;

t_process_node * dequeueProcess(t_process_list *processes);
int queueIsEmpty(t_process_list *processes);
void queueProcess(t_process_list *processes, t_process_node * process);

#endif