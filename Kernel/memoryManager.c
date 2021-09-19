#include <stdint.h>
#include <stddef.h>
#include <memoryManager.h>

#define BLOCK_SIZE 4
#define MEMORY_SIZE 1024

char *current;
size_t size_available = MEMORY_SIZE;

void initializeMemoryManager(void * const heap_base){
    current = heap_base;
}

int calculate_block_qty(size_t size){
    return (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
}

void *alloc(size_t size){
    char *tmp;

    if(size == 0) {
        return NULL;
    }

    size_t block_qty = calculate_block_qty(size);

    if( block_qty * BLOCK_SIZE > size_available){
        return NULL;
    }

    size_available -= block_qty * BLOCK_SIZE;
    tmp = current;
    current += block_qty * BLOCK_SIZE;
    return tmp;   
}