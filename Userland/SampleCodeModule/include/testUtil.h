#include <stdint.h>

#define MINOR_WAIT 1000000
#define MAJOR_WAIT 10000000

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
void idleProcess();
void busyWait(int time);
