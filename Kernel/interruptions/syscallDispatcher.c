#include <syscallDispatcher.h>
#include <syscalls.h>
#include <keyboardDriver.h>
#include <videoDriver.h>

uint64_t syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    switch (rdi) {
        case SYS_WRITE_ID:
            sys_write((char *)(rsi), (uint8_t)(rdx), (t_color)(r10), (t_color)(r8));
            break;
        case SYS_RTC_ID:
            return getCurrentTime((uint8_t) rsi);
            break;
        case SYS_READ_ID:
            // return sys_read(rsi, rdx); MODO ACTIVO
            return sys_read(); //MODO HLT
            break;
        case SYS_CPUFEATURES_ID:
            getCPUFeatures((uint32_t *) rsi);
            break;
        case SYS_PRINTMEM_ID:
            getMem(rsi, (uint8_t*) rdx, r10);
            break;
        case SYS_CPUID_ID:
            return _checkCPUID();
            break;
        case SYS_INFOREG_ID:
            return getRegisters();
            break;
        case SYS_CLEAR_ID:
            clearScreen();
            break;
        case SYS_EXIT_ID:
            _exit();
            break;
        case SYS_CPUVENDOR_ID:
            getCPUVendor((char *)(rsi));
            break;
        default:
            break;
    }
    return 0;
}
