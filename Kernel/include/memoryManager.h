#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

void initializeMemoryManager(char * heap_base, size_t heap_size);
void *malloc(uint64_t nbytes);
void free(void *block);
void memoryDump();

#endif