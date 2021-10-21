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
    (void)_syscall(SYS_PROCESS_PID_ID, 0, 0, 0, 0, 0);
}