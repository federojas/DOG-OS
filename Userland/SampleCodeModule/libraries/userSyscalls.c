#include <userSyscalls.h>

int newProcess(void (*entryPoint)(int, char **), int argc, char ** argv, uint8_t foreground, uint16_t * fd) {
   return _syscall(SYS_NEW_PROCESS_ID, (uint64_t) entryPoint, argc, (uint64_t) argv, (uint64_t) foreground, (uint64_t) fd);
}

int killProcess(uint64_t pid) {
   return _syscall(SYS_KILL_PROCESS_ID, pid, 0, 0, 0, 0);
}

int getProcessPID() {
    return _syscall(SYS_PROCESS_PID_ID, 0, 0, 0, 0, 0);
}

void processStatus() {
    (void)_syscall(SYS_PROCESS_STATUS_ID, 0, 0, 0, 0, 0);
}

int blockProcess(uint64_t pid) {
    return _syscall(SYS_BLOCK_PROCESS_ID, pid, 0, 0, 0, 0);
}

int unblockProcess(uint64_t pid) {
    return _syscall(SYS_UNBLOCK_PROCESS_ID, pid, 0, 0, 0, 0);
}

int setState(uint64_t pid, t_state newState) {
    return _syscall(SYS_CHANGE_PROCESS_STATE_ID, pid, (uint64_t) newState, 0, 0, 0);
}

void yield() {
    (void)_syscall(SYS_YIELD_ID, 0, 0, 0, 0, 0);
}

void setPriority(uint64_t pid, int newPriority) {
    (void)_syscall(SYS_SET_PRIORITY_ID, pid, newPriority, 0, 0, 0);
}

void * malloc(uint64_t size) {
    return (void *)_syscall(SYS_MALLOC_ID, size, 0, 0, 0, 0);
}

void free(void * address) {
    (void)_syscall(SYS_FREE_ID, (uint64_t) address, 0, 0, 0, 0);
}

int semOpen(uint32_t id, uint64_t initialValue) {
    return _syscall(SYS_SEM_OPEN_ID, (uint64_t) id, initialValue, 0, 0, 0);
}

int semWait(uint32_t id) {
    return _syscall(SYS_SEM_WAIT_ID, (uint64_t) id, 0, 0, 0, 0);
}

int semPost(uint32_t id) {
    return _syscall(SYS_SEM_POST_ID, (uint64_t) id, 0, 0, 0, 0);
}

int semClose(uint32_t id) {
    return _syscall(SYS_SEM_CLOSE_ID, (uint64_t) id, 0, 0, 0, 0);
}

void semStatus() {
    (void)_syscall(SYS_SEM_STATUS_ID, 0, 0, 0, 0, 0);
}

void memStatus() {
    (void)_syscall(SYS_MEMORY_STATUS_ID, 0, 0, 0, 0, 0);
}

int getSecondsElapsed() {
    return _syscall(SYS_SEC_ELAPSED_ID, 0, 0, 0, 0, 0);
}

void pipeStatus() {
    (void)_syscall(SYS_PIPE_STATUS_ID, 0, 0, 0, 0, 0);
}

int pipeOpen(int pipeID) {
    return _syscall(SYS_PIPE_OPEN_ID, (uint64_t) pipeID, 0, 0, 0, 0);
}

int pipeClose(int pipeID) {
    return _syscall(SYS_PIPE_CLOSE_ID, (uint64_t) pipeID, 0, 0, 0, 0);
}

int pipeWrite(int pipeID, char * str) {
    return _syscall(SYS_PIPE_WRITE_ID, (uint64_t) pipeID, (uint64_t) str, 0, 0, 0);
}

int pipeRead(int pipeID) {
    return _syscall(SYS_PIPE_READ_ID, (uint64_t) pipeID, 0, 0, 0, 0);
}

void wait(int pipeID) {
    (void)_syscall(SYS_WAIT_ID, (uint64_t) pipeID, 0, 0, 0, 0);
}