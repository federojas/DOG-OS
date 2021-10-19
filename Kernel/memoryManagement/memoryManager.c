#ifdef MEMORY_MANAGER_FREE

#include <stdint.h>
#include <stddef.h>
#include <memoryManager.h>
#include <stdbool.h>

typedef long Align;

typedef union header
{
    struct
    {
        union header *ptr;
        size_t size;  
    } s;

    Align x; 

} Header;

static Header * base;     
static Header *free_node = NULL;

size_t total_units;

void initializeMemoryManager(char * heap_base, size_t heap_size) {
    if(heap_base == NULL) 
        return ;
    total_units = (heap_size + sizeof(Header) - 1) / sizeof(Header) + 1;
    free_node = base = (Header *) heap_base;
    free_node->s.size = total_units;
    free_node->s.ptr = free_node;
}


void *malloc(uint64_t nbytes) {

    if(nbytes == 0) 
        return 0;

    Header *current_node, *prevptr;
    size_t nunits;
    void *result;
    bool is_allocating;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    prevptr = free_node;

    is_allocating = true;
    for (current_node = prevptr->s.ptr; is_allocating; current_node = current_node->s.ptr)
    {
        if (current_node->s.size >= nunits)
        {
            if (current_node->s.size == nunits)
            {
                prevptr->s.ptr = current_node->s.ptr;
            }
            else
            {
                current_node->s.size -= nunits;
                current_node += current_node->s.size;
                current_node->s.size = nunits;
            }

            free_node = prevptr;
            result = current_node + 1;
            is_allocating = false; 
        }

        if (current_node == free_node)
            return NULL;

        prevptr = current_node;
    } /* for */

    return result;
}


void free(void *block) {
    if (block == NULL || (((long)block - (long)base) % sizeof(Header)) != 0)  {
        return;
    }
        

    Header *free_block, *current_node;
    free_block = (Header *)block - 1;              

    if (free_block < base || free_block >= (base + total_units * sizeof(Header))) {
        return;
    }

    block = NULL;

    bool external = false;

    for (current_node = free_node; !(free_block > current_node && free_block < current_node->s.ptr); current_node = current_node->s.ptr) {
        if(free_block == current_node || free_block == current_node->s.ptr) {
            return ;
        }   
        if (current_node >= current_node->s.ptr && (free_block > current_node || free_block < current_node->s.ptr)) {
            external = true;
            break; 
        }
    }

    if (!external && (current_node + current_node->s.size > free_block || free_block + free_block->s.size > current_node->s.ptr)) {
            return;
    }
            
        
    if (free_block + free_block->s.size == current_node->s.ptr) {
   

        free_block->s.size += current_node->s.ptr->s.size;
   
        free_block->s.ptr = current_node->s.ptr->s.ptr;

    } else {

        free_block->s.ptr = current_node->s.ptr;
    }

    if (current_node + current_node->s.size == free_block) {              

        current_node->s.size += free_block->s.size;

        current_node->s.ptr = free_block->s.ptr;

    } else {
        current_node->s.ptr = free_block;
    }

    free_node = current_node;
}
void memoryDump(){
    long long idx=1;
    Header *original,*current;
    original=current=free_node;
    int flag=1;
    printf("\n MEMORY DUMP (mem Manager list)\n");
    printf("-------------------------------------------\n");
    printf("total memory: %d bytes\n", total_units*sizeof(Header));
    if(free_node==NULL){
        printf("No free blocks available\n");
        return;
    }
    printf("Free blocks:\n");

    while(current!=original||flag){
        flag=0;
        printf(" Block number %d\n",idx);
        printf(" Base:%x\n", (uint64_t)current);
        printf(" Free units: %d\n", current->s.size);

        current=current->s.ptr;
        idx++;
    }
    printf("\n");
}

#endif


//naive 

// #define BLOCK_SIZE 4
// #define MEMORY_SIZE 1024

// char *current;
// size_t size_available = MEMORY_SIZE;

// void initializeMemoryManager(void *const heap_base)
// {
//     current = heap_base;
// }

// int calculate_block_qty(size_t size)
// {
//     return (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
// }

// void *alloc(size_t size)
// {
//     char *tmp;

//     if (size == 0)
//     {
//         return NULL;
//     }

//     size_t block_qty = calculate_block_qty(size);

//     if (block_qty * BLOCK_SIZE > size_available)
//     {
//         return NULL;
//     }

//     size_available -= block_qty * BLOCK_SIZE;
//     tmp = current;
//     current += block_qty * BLOCK_SIZE;
//     return tmp;
// }