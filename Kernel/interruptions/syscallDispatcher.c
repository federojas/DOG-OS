#include <syscallDispatcher.h>
#include <RTC.h>
#include <CPUFeatures.h>
#include <colors.h>

#define SYS_RTC_ID 1
#define SYS_WRITE_ID 4
#define SYS_CPUFEATURES_ID 2

void syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    switch (rdi)
    {
    case SYS_WRITE_ID:
        sys_write((char *)(rsi), (uint8_t)(rdx), (t_color)(r10), (t_color)(r8));
        break;
    case SYS_RTC_ID:
        printCurrentTime();
        break;
    case SYS_CPUFEATURES_ID:
        getCPUFeatures();
        break;
    default:
        break;
    }

}
