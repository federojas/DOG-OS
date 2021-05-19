#include <syscallDispatcher.h>

#define SYS_WRITE 4


void syscallSelector(uint64_t sysID) {
    switch (sysID)
    {
    case SYS_WRITE:
        //sys_Write
        break;
    
    default:
        break;
    }

}
