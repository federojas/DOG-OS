#ifndef USER_SYSCALLS
#define USER_SYSCALLS

#include <stdint.h>
#include <colors.h>

#define FOREGROUND 1
#define BACKGROUND 0
#define NULL ((void *)0)

typedef enum { READY, BLOCKED, TERMINATED } t_state;

typedef enum {SYS_RTC_ID, SYS_CPUFEATURES_ID, SYS_READ_ID, SYS_WRITE_ID, SYS_INFOREG_ID, 
SYS_PRINTMEM_ID, SYS_CPUID_ID, SYS_CLEAR_ID, SYS_EXIT_ID, SYS_CPUVENDOR_ID, SYS_MALLOC_ID,
SYS_FREE_ID, SYS_MEMORY_STATUS_ID, SYS_NEW_PROCESS_ID, SYS_KILL_PROCESS_ID, SYS_PROCESS_PID_ID,
SYS_PROCESS_STATUS_ID, SYS_BLOCK_PROCESS_ID, SYS_CHANGE_PROCESS_STATE_ID, SYS_YIELD_ID, 
SYS_SET_PRIORITY_ID, SYS_UNBLOCK_PROCESS_ID, SYS_SEM_OPEN_ID, 
SYS_SEM_POST_ID, SYS_SEM_WAIT_ID, SYS_SEM_CLOSE_ID, SYS_SEM_STATUS_ID, SYS_SEC_ELAPSED_ID,
SYS_PIPE_STATUS_ID, SYS_PIPE_OPEN_ID, SYS_PIPE_CLOSE_ID, SYS_PIPE_WRITE_ID,
SYS_PIPE_READ_ID, SYS_WAIT_ID, SYS_HALF_SCREEN_SIZE_ID} t_syscall_id;

uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);
int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground, uint16_t * fd);
int killProcess(uint64_t pid);
int getProcessPID();
void processStatus();
int blockProcess(uint64_t pid);
int setState(uint64_t pid, t_state newState);
void yield();
void setPriority(uint64_t pid, int newPriority);
int unblockProcess(uint64_t pid);
void * malloc(uint64_t size);
void free(void * address);
void memStatus();
int semOpen(uint32_t id, uint64_t initialValue);
int semWait(uint32_t id);
int semClose(uint32_t id);
void semStatus();
int semPost(uint32_t id);
int getSecondsElapsed();
void pipeStatus();
int pipeOpen(int pipeID);
int pipeClose(int pipeID);
int pipeWrite(int pipeID, char * str);
int pipeRead(int pipeID);
void wait(int pipeID);
int getHalfScreenSize();
#endif