#ifndef SYSTEM_CALLS
#define SYSTEM_CALLS

#include <stdint.h>
#include <colors.h>

#define SYS_RTC_ID 1
#define SYS_CPUFEATURES_ID 2
#define SYS_READ_ID 3
#define SYS_WRITE_ID 4
#define SYS_INFOREG_ID 5
#define SYS_PRINTMEM_ID 6
#define SYS_CPUID_ID 7
#define SYS_CLEAR_ID 8

uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9);

#endif