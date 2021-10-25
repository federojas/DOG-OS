#ifndef LIB_H
#define LIB_H

#include <stdint.h>

// Memory methods
void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

//Sync methods

void acquire(int *lock);
void release(int *lock);
int _xchg();

//Other methods
char *cpuVendor(char *result);
void _hlt();



#endif