#include <syscallDispatcher.h>
#include <RTC.h>
#include <CPUFeatures.h>

#define SYS_RTC_ID 1
#define SYS_WRITE_ID 4
#define SYS_CPUFEATURES_ID 2


void syscallSelector(uint64_t sysID) {
    switch (sysID)
    {
    case SYS_WRITE_ID:
        //sys_Write
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
