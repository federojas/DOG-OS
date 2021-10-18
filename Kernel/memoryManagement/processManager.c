#include <stdint.h>
#include <interrupts.h>
#include <prints.h>
#include <stddef.h>
#include <memoryManager.h>
#include <processManager.h>

//https://en.wikipedia.org/wiki/Process_control_block

typedef enum { READY, BLOCKED, TERMINATED } t_process_state;

typedef struct {

    uint64_t pid;
    uint64_t ppid;
    uint8_t foreground;
    t_process_state state;
    uint64_t priority;
    char name[30];
    void *rsp;
    void *rbp;

} t_processControlBlock;

typedef struct {

    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;

    uint64_t rip;
    uint64_t rsp;
    uint64_t cs;
    uint64_t eflags;
    uint64_t ss;
    uint64_t base;

} t_stackFrame;

typedef struct process_node {
    t_processControlBlock processControlBlock;
    struct t_process_node * next_process_node;
} t_process_node;

typedef struct process_list {
      uint32_t size;
      t_process_node * first;
} t_process_list;

static t_process_list * processes;
static uint64_t currentPID = 0;

static void idleProcess(int argc, char ** argv) {
    while(1) {
        _hlt();
    }
}

static uint64_t getPID() {
      return currentPID++;
}

static int initializeProcessControlBlock(t_processControlBlock * PCB, char * name, uint8_t foreground) {
    strcpy(name, PCB->name);
    PCB->pid = getPID();
    //??
    //arranca ready? foreground? ni idea
    return 0;
}

static void getArguments(char ** to, char ** from, int count) {
      for (int i = 0; i < count; i++) {
            to[i] = malloc(sizeof(char) * (strlen(from[i]) + 1));
            if(to[i] == NULL) {
                return ;
            }
            strcpy(to[i], from[i]);
      }
}

static void initializeProcessStackFrame(void (*entryPoint)(int, char**), int argc, char** argv, void* rbp) {
      
    t_stackFrame * stackFrame = (t_stackFrame *)rbp - 1; //rbp apunta a un byte despus del principio del bloque de memoria asignado
      
    stackFrame->rax = 0x01;
    stackFrame->rbx = 0x02;
    stackFrame->rcx = 0x03;
    stackFrame->rdx = (uint64_t)argv;
    stackFrame->rbp = 0x04;
    stackFrame->rdi = (uint64_t)entryPoint;
    stackFrame->rsi = (uint64_t)argc;
    stackFrame->r8 = 0x05;
    stackFrame->r9 = 0x06;
    stackFrame->r10 = 0x07;
    stackFrame->r11 = 0x08;
    stackFrame->r12 = 0x09;
    stackFrame->r13 = 0x0A;
    stackFrame->r14 = 0x0B;
    stackFrame->r15 = 0x0C;
    
   // stackFrame->rip = //wrapper lo vemos mas adelante;
    stackFrame->rsp = (uint64_t)(&stackFrame->base);
    stackFrame->cs = 0x8;
    stackFrame->eflags = 0x202;
    stackFrame->ss = 0x0;
    stackFrame->base = 0x0;
}

void initializeProcessManager() {
    processes = malloc(sizeof(t_process_list));
    if(processes == NULL) {
        return ;
    }

    processes->size = 0;

    char *argv[] = {"Initial Idle Process"};
    newProcess(&idleProcess, 1, argv, 0);
}

int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground) {

    if (entryPoint == NULL) {
        return -1;
    }
        

    t_process_node * newProcess = malloc(sizeof(t_process_node));
    if(newProcess == NULL) {
        return -1;
    }
        

    if (initializeProcessControlBlock(&newProcess->processControlBlock, argv[0], foreground) == -1) {
        free(newProcess);
        return -1;
    }

    char ** arguments = malloc(sizeof (char *) * argc);
    if(arguments == 0) {
        return-1;
    }

    getArguments(arguments, argv, argc);

    initializeProcessStackFrame(entryPoint, argc, arguments, newProcess->processControlBlock.rbp);

    return 0;
}

