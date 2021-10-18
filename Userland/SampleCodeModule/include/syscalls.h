#ifndef SYSTEM_CALLS
#define SYSTEM_CALLS

#include <stdint.h>
#include <colors.h>

typedef enum {SYS_RTC_ID, SYS_CPUFEATURES_ID, SYS_READ_ID, SYS_WRITE_ID, SYS_INFOREG_ID, 
SYS_PRINTMEM_ID, SYS_CPUID_ID, SYS_CLEAR_ID, SYS_EXIT_ID, SYS_CPUVENDOR_ID, SYS_MALLOC_ID, SYS_FREE_ID } t_syscall_id;

uint64_t _syscall(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9);

#endif