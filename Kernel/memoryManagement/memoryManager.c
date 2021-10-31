// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef MEMORY_MANAGER_FREE

#include <stdint.h>
#include <stddef.h>
#include <memoryManager.h>
#include <stdbool.h>
#include <prints.h>

typedef long Align;

typedef union header
{
    struct
    {
        union header *ptr;
        size_t size;  
    } data;

    Align x; 

} Header;

static Header * base;     
static Header *free_node = NULL;

size_t total_units;

void initializeMemoryManager(char * heap_base, size_t heap_size) {
    if(heap_base == NULL) {
        return ;
    } 
    total_units = (heap_size + sizeof(Header) - 1) / sizeof(Header) + 1;
    free_node = base = (Header *) heap_base;
    free_node->data.size = total_units;
    free_node->data.ptr = free_node;
}


void *malloc(uint64_t nbytes) {

    if(nbytes == 0) {
        return 0;
    }
        
    Header *current_node, *prevptr;
    size_t nunits;
    void *result;
    bool is_allocating;

    nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

    prevptr = free_node;

    is_allocating = true;
    for (current_node = prevptr->data.ptr; is_allocating; current_node = current_node->data.ptr) {
        if (current_node->data.size >= nunits) {
            if (current_node->data.size == nunits) {
                prevptr->data.ptr = current_node->data.ptr;
            } else {
                current_node->data.size -= nunits;
                current_node += current_node->data.size;
                current_node->data.size = nunits;
            }

            free_node = prevptr;
            result = current_node + 1;
            is_allocating = false; 
        }

        if (current_node == free_node) {
            return NULL;
        }
            

        prevptr = current_node;
    }

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

    for (current_node = free_node; !(free_block > current_node && free_block < current_node->data.ptr); current_node = current_node->data.ptr) {
        if(free_block == current_node || free_block == current_node->data.ptr) {
            return ;
        }   
        if (current_node >= current_node->data.ptr && (free_block > current_node || free_block < current_node->data.ptr)) {
            external = true;
            break; 
        }
    }

    if (!external && (current_node + current_node->data.size > free_block || free_block + free_block->data.size > current_node->data.ptr)) {
            return;
    }
            
        
    if (free_block + free_block->data.size == current_node->data.ptr) {
   

        free_block->data.size += current_node->data.ptr->data.size;
   
        free_block->data.ptr = current_node->data.ptr->data.ptr;

    } else {

        free_block->data.ptr = current_node->data.ptr;
    }

    if (current_node + current_node->data.size == free_block) {              

        current_node->data.size += free_block->data.size;

        current_node->data.ptr = free_block->data.ptr;

    } else {
        current_node->data.ptr = free_block;
    }

    free_node = current_node;
}
void memoryDump(){
    int blockNumber = 1;
    Header *original,*current;
    original=current=free_node;
    int flag=1;
    printf("\nMEMORY DUMP (Free List Memory Manager)\n");
    printf("\nTotal memory: %d bytes\n\n", (uint32_t) total_units*sizeof(Header));
    if(free_node==NULL){
        printf("\nNo free blocks available\n");
        return;
    }
    printf("Free blocks:\n\n");

    while(current!=original||flag){
        flag=0;
        printf("    Block number %d\n",blockNumber);
        printf("    Base:%x\n", (uint64_t)current);
        printf("    Free units: %d\n\n", (int) current->data.size);

        current=current->data.ptr;
        blockNumber++;
    }
    printf("\n");
}

#endif