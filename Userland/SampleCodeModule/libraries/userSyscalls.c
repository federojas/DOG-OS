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

void printProcessStatus() {
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

void testingPrintProcessQueue() {
    (void)_syscall(SYS_TEST_PRINT_PROCESS_QUEUE_ID, 0, 0, 0, 0, 0);
}