#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

void initializeMemoryManager(char * heap_base, size_t heap_size);
void *kr_malloc(size_t nbytes);


#endif