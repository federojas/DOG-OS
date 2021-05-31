#include <syscallDispatcher.h>
#include <syscalls.h>
#include <colors.h>
#include <prints.h>
#include <keyboardDriver.h>
#include <prints.h>

#define SYS_RTC_ID 1
#define SYS_CPUFEATURES_ID 2
#define SYS_READ_ID 3
#define SYS_WRITE_ID 4
#define SYS_INFOREG_ID 5
#define SYS_PRINTMEM_ID 6
#define SYS_CPUID_ID 7


uint64_t syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    switch (rdi) {
        case SYS_WRITE_ID:
            sys_write((char *)(rsi), (uint8_t)(rdx), (t_color)(r10), (t_color)(r8));
            break;
        case SYS_RTC_ID:
            return getCurrentTime((uint8_t) rsi);
            break;
        case SYS_READ_ID:
            return sys_read(rsi, rdx);
            break;
        case SYS_CPUFEATURES_ID:
            getCPUFeatures(rsi);
            break;
        case SYS_PRINTMEM_ID:
            //printMem();
            break;
        case SYS_CPUID_ID:
            return _checkCPUID();
            break;
        case SYS_INFOREG_ID:
            return getRegisters();
            break;
        default:
            break;
    }
    return 0;
}
