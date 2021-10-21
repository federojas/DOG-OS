#include <interrupts.h>
#include <memoryManager.h>
#include <prints.h>
#include <processManager.h>
#include <processManagerQueue.h>
#include <stddef.h>
#include <stdint.h>

static void *const sampleCodeModuleAddress = (void *)0x400000;

#define SIZE_OF_STACK (4 * 1024)
#define BACKGROUND_PRIORITY_DEFAULT 1
#define FOREGROUND_PRIORITY_DEFAULT 2

typedef struct {
  uint64_t gs;
  uint64_t fs;
  uint64_t r15;
  uint64_t r14;
  uint64_t r13;
  uint64_t r12;
  uint64_t r11;
  uint64_t r10;
  uint64_t r9;
  uint64_t r8;
  uint64_t rsi;
  uint64_t rdi;
  uint64_t rbp;
  uint64_t rdx;
  uint64_t rcx;
  uint64_t rbx;
  uint64_t rax;

  uint64_t rip;
  uint64_t cs;
  uint64_t eflags;
  uint64_t rsp;
  uint64_t ss;
  uint64_t base;
} t_stackFrame;

static void idleProcess(int argc, char **argv);
static int initializeProcessControlBlock(t_PCB *PCB, char *name,
                                         uint8_t foreground, uint16_t *fd);
static void getArguments(char **to, char **from, int count);
static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv);
static void initializeProcessStackFrame(void (*entryPoint)(int, char **),
                                        int argc, char **argv, void *rbp);
static uint64_t setState(uint64_t pid, t_state state);
static void freeProcess(t_process_node *p);
static uint64_t getPID();
static void end();
static t_process_node *getProcess(uint64_t pid);

static uint64_t currentPID = 0;
static uint64_t cyclesLeft;

static t_process_list *processes;
static t_process_node *currentProcess = NULL;
static t_process_node *baseProcess;

void initializeProcessManager() {
  processes = malloc(sizeof(t_process_list));
  if (processes == NULL) {
    return;
  }

  processes->first = NULL;
  processes->last = processes->first;
  processes->readySize = 0;
  processes->size = 0;

  char *argv[] = {"Initial Idle Process"};

  newProcess(&idleProcess, 1, argv, BACKGROUND, 0);

  baseProcess = dequeueProcess(processes);
}

void *processManager(void *sp) {

  if (currentProcess != NULL) {
    if (currentProcess->pcb.state == READY && cyclesLeft > 0) {
      cyclesLeft--;
      return sp;
    }

    currentProcess->pcb.rsp = sp;

    if (currentProcess->pcb.pid != baseProcess->pcb.pid) {
      if (currentProcess->pcb.state == TERMINATED) {
        t_process_node *parent = getProcess(currentProcess->pcb.ppid);
        if (parent != NULL && currentProcess->pcb.foreground &&
            parent->pcb.state == BLOCKED) {
          readyProcess(parent->pcb.pid);
        }
        freeProcess(currentProcess);
      } else {
        queueProcess(processes, currentProcess);
      }
    }
  }

  if (processes->readySize > 0) {
    currentProcess = dequeueProcess(processes);
    while (currentProcess->pcb.state != READY) {
      if (currentProcess->pcb.state == TERMINATED) {
        freeProcess(currentProcess);
      }
      if (currentProcess->pcb.state == BLOCKED) {
        queueProcess(processes, currentProcess);
      }
      currentProcess = dequeueProcess(processes);
    }
  } else {
    currentProcess = baseProcess;
  }

  cyclesLeft = currentProcess->pcb.priority;

  return currentProcess->pcb.rsp;
}

int newProcess(void (*entryPoint)(int, char **), int argc, char **argv,
               uint8_t foreground, uint16_t *fd) {

  if (entryPoint == NULL) {
    return -1;
  }

  t_process_node *newProcess = malloc(sizeof(t_process_node));
  if (newProcess == NULL) {
    return -1;
  }

  if (initializeProcessControlBlock(&newProcess->pcb, argv[0], foreground,
                                    fd) == -1) {
    free(newProcess);
    return -1;
  }

  char **arguments = malloc(sizeof(char *) * argc);
  if (arguments == 0) {
    return -1;
  }

  getArguments(arguments, argv, argc);

  newProcess->pcb.argc = argc;
  newProcess->pcb.argv = argv;

  initializeProcessStackFrame(entryPoint, argc, arguments, newProcess->pcb.rbp);

  newProcess->pcb.state = READY;

  queueProcess(processes, newProcess);
  if (newProcess->pcb.foreground && newProcess->pcb.ppid) {
    blockProcess(newProcess->pcb.ppid);
  }

  return newProcess->pcb.pid;
}

uint64_t killProcess(uint64_t pid) {

  int resPID = setState(pid, TERMINATED);

  if (pid == currentProcess->pcb.pid) {
    _callTimerTick();
  }

  return resPID;
}

uint64_t blockProcess(uint64_t pid) {
  int resPID = setState(pid, BLOCKED);

  if (pid == currentProcess->pcb.pid) {
    _callTimerTick();
  }

  return resPID;
}

uint64_t readyProcess(uint64_t pid) { return setState(pid, READY); }

static void idleProcess(int argc, char **argv) {
  while (1) {
    _hlt();
  }
}

static uint64_t getPID() { return currentPID++; }

static int initializeProcessControlBlock(t_PCB *PCB, char *name,
                                         uint8_t foreground, uint16_t *fd) {
  strcpy(PCB->name, name);
  PCB->pid = getPID();
  PCB->ppid = (currentProcess == NULL ? 0 : currentProcess->pcb.pid);
  if (foreground > 1 || foreground < 0) {
    return -1;
  }

  PCB->foreground = (currentProcess == NULL
                         ? foreground
                         : (currentProcess->pcb.foreground ? foreground : 0));
  PCB->rbp = malloc(SIZE_OF_STACK);
  PCB->priority = PCB->foreground ? FOREGROUND_PRIORITY_DEFAULT
                                  : BACKGROUND_PRIORITY_DEFAULT;
  PCB->fileDescriptors[0] = (fd ? fd[0] : 0);
  PCB->fileDescriptors[1] = (fd ? fd[1] : 1);

  if (PCB->rbp == NULL) {
    return -1;
  }

  PCB->rbp = (void *)((char *)PCB->rbp + SIZE_OF_STACK - 1);
  PCB->rsp = (void *)((t_stackFrame *)PCB->rbp - 1);
  return 0;
}

static void getArguments(char **to, char **from, int count) {
  for (int i = 0; i < count; i++) {
    to[i] = malloc(sizeof(char) * (strlen(from[i]) + 1));
    strcpy(to[i], from[i]);
  }
}

static void end() {
  (void)killProcess(currentProcess->pcb.pid);
  _callTimerTick();
}

static void wrapper(void (*entryPoint)(int, char **), int argc, char **argv) {
  entryPoint(argc, argv);
  end();
}

static void initializeProcessStackFrame(void (*entryPoint)(int, char **),
                                        int argc, char **argv, void *rbp) {

  t_stackFrame *stackFrame = (t_stackFrame *)rbp - 1;

  stackFrame->gs = 0x001;
  stackFrame->fs = 0x002;
  stackFrame->r15 = 0x003;
  stackFrame->r14 = 0x004;
  stackFrame->r13 = 0x005;
  stackFrame->r12 = 0x006;
  stackFrame->r11 = 0x007;
  stackFrame->r10 = 0x008;
  stackFrame->r9 = 0x009;
  stackFrame->r8 = 0x00A;
  stackFrame->rsi = (uint64_t)argc;
  stackFrame->rdi = (uint64_t)entryPoint;
  stackFrame->rbp = 0x00B;
  stackFrame->rdx = (uint64_t)argv;
  stackFrame->rcx = 0x00C;
  stackFrame->rbx = 0x00D;
  stackFrame->rax = 0x00E;
  stackFrame->rip = (uint64_t)wrapper;
  stackFrame->cs = 0x008;
  stackFrame->eflags = 0x202;
  stackFrame->rsp = (uint64_t)(&stackFrame->base);
  stackFrame->ss = 0x000;
  stackFrame->base = 0x000;
}

static uint64_t setState(uint64_t pid, t_state newState) {
  t_process_node *process = getProcess(pid);

  if (process == NULL || process->pcb.state == TERMINATED) {
    return -1;
  }

  if (process == currentProcess) {
    process->pcb.state = newState;
    return process->pcb.pid;
  }

  if (process->pcb.state != READY && newState == READY) {
    processes->readySize++;
  }

  if (process->pcb.state == READY && newState != READY) {
    processes->readySize--;
  }

  process->pcb.state = newState;

  return process->pcb.pid;
}

static t_process_node *getProcess(uint64_t pid) {
  if (currentProcess != NULL && currentProcess->pcb.pid == pid) {
    return currentProcess;
  }

  t_process_node *process = currentProcess;
  while (process != NULL) {
    if (process->pcb.pid == pid) {
      return process;
    }
    process = (t_process_node *)process->next;
  }

  return NULL;
}

static void freeProcess(t_process_node *p) {
  for (int i = 0; i < p->pcb.argc; i++) {
    free(p->pcb.argv[i]);
  }
  free(p->pcb.argv);
  free((void *)((char *)p->pcb.rbp - SIZE_OF_STACK + 1));
  free((void *)p);
}

char *stateToStr(t_state state) {
  switch (state) {
  case READY:
    return "READY";
    break;
  case BLOCKED:
    return "BLOCKED";
  default:
    return "D";
    break;
  };
}

char * fgToBoolStr(int fg) {
  return fg > 0 ? "TRUE" : "FALSE";
}

void printProcess(t_process_node *p) {
  if (p != NULL)
    printf("%d          %s      %d  %d    %s      %s\n",
          p->pcb.pid, fgToBoolStr((int)p->pcb.foreground) , (uint64_t)p->pcb.rsp,
          (uint64_t)p->pcb.rbp, stateToStr(p->pcb.state), p->pcb.name);
}

int printPS() {
  printf(
      "PID      FOREGROUND      RSP       RBP      STATE      NAME\n");

  if (currentProcess != NULL)
    printProcess(currentProcess);

  t_process_node *curr = processes->first;
  while (curr) {
    printProcess(curr);
    curr = curr->next;
  }

  return 0;
}