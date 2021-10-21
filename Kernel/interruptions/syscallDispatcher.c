#include <syscallDispatcher.h>
#include <syscalls.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <memoryManager.h>
#include <processManager.h> 

typedef int (*functionPointer)(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static int getCurrentTimeWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getCurrentTime((uint8_t) rsi);
}

static int sysWriteWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    sysWrite((char *)(rsi), (uint8_t)(rdx), (t_color)(rcx), (t_color)(r8), (uint64_t) r9);
    return 0;
}

static int getCPUFeaturesWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getCPUFeatures((uint32_t *) rsi);
    return 0;
}

static int sysReadWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return sysRead();
}

static int getRegistersWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getRegisters();
}

static int getMemWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getMem(rsi, (uint8_t*) rdx, rcx);
    return 0;
}

static int checkCPUIDWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return _checkCPUID();
}

static int clearScreenWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    clearScreen();
    return 0;
}

static int exitWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    _exit();
    return 0;
}

static int getCPUVendorWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getCPUVendor((char *)(rsi));
    return 0;
}

static int mallocWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
   return malloc((size_t) rsi);
}

static int freeWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
   free((void *) rsi);
   return 0;
}

static int memDumpWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    memoryDump();
    return 0;
}

static int newProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return newProcess((void (*)(int, char **)) rsi, (int) rdx, (char **) rcx, (uint8_t) r8, (uint16_t *) r9);
}

static int killProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return killProcess(rsi);
}

static int getProcessPIDWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getProcessPID();
}

static int processStatusWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    printProcessStatus();
    return 0;
}

static functionPointer syscall[] = {
    getCurrentTimeWrapper,
    getCPUFeaturesWrapper,
    sysReadWrapper,
    sysWriteWrapper,
    getRegistersWrapper,
    getMemWrapper,
    checkCPUIDWrapper,
    clearScreenWrapper,
    exitWrapper,
    getCPUVendorWrapper,
    mallocWrapper,
    freeWrapper,
    memDumpWrapper,
    newProcessWrapper,
    killProcessWrapper,
    getProcessPIDWrapper, 
    processStatusWrapper 
};

uint64_t syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return syscall[rdi](rsi, rdx, rcx, r8, r9);      
}


