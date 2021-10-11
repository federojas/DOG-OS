#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <stddef.h>
#include <stdlib.h>

void initializeMemoryManager(char * heap_base, size_t heap_size);
void *malloc(size_t nbytes);
void free(void *block);

#endif