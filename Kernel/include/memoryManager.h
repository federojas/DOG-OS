#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

void initializeMemoryManager(char * heap_base, size_t heap_size);
void *malloc(size_t nbytes);
void free(void *block);

#endif