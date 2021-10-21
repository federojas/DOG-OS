#include <syscallDispatcher.h>
#include <syscalls.h>
#include <keyboardDriver.h>
#include <videoDriver.h>
#include <memoryManager.h>
#include <processManager.h> 
#include <processManagerQueue.h>

typedef uint64_t (*functionPointer)(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

static uint64_t getCurrentTimeWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getCurrentTime((uint8_t) rsi);
}

static uint64_t sysWriteWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    sysWrite((char *)(rsi), (uint8_t)(rdx), (t_color)(rcx), (t_color)(r8), (uint64_t) r9);
    return 0;
}

static uint64_t getCPUFeaturesWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getCPUFeatures((uint32_t *) rsi);
    return 0;
}

static uint64_t sysReadWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return sysRead();
}

static uint64_t getRegistersWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return (uint64_t) getRegisters();
}

static uint64_t getMemWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getMem(rsi, (uint8_t*) rdx, rcx);
    return 0;
}

static uint64_t checkCPUIDWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return _checkCPUID();
}

static uint64_t clearScreenWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    clearScreen();
    return 0;
}

static uint64_t exitWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    _exit();
    return 0;
}

static uint64_t getCPUVendorWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    getCPUVendor((char *)(rsi));
    return 0;
}

static uint64_t mallocWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
   return (uint64_t) malloc((size_t) rsi);
}

static uint64_t freeWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
   free((void *) rsi);
   return 0;
}

static uint64_t memDumpWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    memoryDump();
    return 0;
}

static uint64_t newProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return newProcess((void (*)(int, char **)) rsi, (int) rdx, (char **) rcx, (uint8_t) r8, (uint16_t *) r9);
}

static uint64_t killProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return killProcess(rsi);
}

static uint64_t getProcessPIDWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return getProcessPID();
}

static uint64_t processStatusWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    printProcessStatus();
    return 0;
}

static uint64_t blockProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return blockProcess(rsi);
}

static uint64_t setStateWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return setState(rsi, (t_state) rdx);
}

static uint64_t yieldWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    yield();
    return 0;
}

static uint64_t setPriorityWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    setPriority(rsi, (int) rdx);
    return 0;
}

static uint64_t readyProcessWrapper(uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return readyProcess(rsi);
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
    processStatusWrapper,
    blockProcessWrapper,
    setStateWrapper,
    yieldWrapper,
    setPriorityWrapper,
    readyProcessWrapper,
};

uint64_t syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9) {
    return syscall[rdi](rsi, rdx, rcx, r8, r9);      
}


