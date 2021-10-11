//https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

#define MEMORY_MANAGER_BUDDY //BORRAR DESPUES, VA EN EL MAKEFILE
#ifdef MEMORY_MANAGER_BUDDY

#include <memoryManager.h>
#include <buddyList.h>
#include <memory.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN_ALLOC_LOG2 4
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 31
#define MAX_ALLOC ((size_t)1 << MAX_ALLOC_LOG2)


#define BUCKET_COUNT ( MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)

//tree levels
static list_t buckets[BUCKET_COUNT];
static size_t maximum_bucket_size;
static list_t * base_ptr;
static uint8_t buckets_amount;


void initializeMemoryManager(char * heap_base, size_t heap_size) {
     if(heap_base == NULL) 
        return ;
    base_ptr = heap_base;
    maximum_bucket_size = heap_size;

}
void *malloc(size_t nbytes) {
    return ;
}
void free(void *block) {
    return ;
}


#endif