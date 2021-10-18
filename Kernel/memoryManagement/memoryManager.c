#ifdef MEMORY_MANAGER_FREE

#include <stdint.h>
#include <stddef.h>
#include <memoryManager.h>
#include <stdbool.h>

typedef long Align; /* for alignment to long boundary */

typedef union header /* block header */
{
    struct
    {
        union header *ptr; /* next block if on free list */
        size_t size;       /* size of this block */
    } s;

    Align x; /* force alignment of blocks */

} Header;

static Header * base;      /* empty list to get started */
static Header *free_node = NULL; /* start of free list */

size_t total_units;

void initializeMemoryManager(char * heap_base, size_t heap_size) {
    if(heap_base == NULL) 
        return ;
    total_units = (heap_size + sizeof(Header) - 1) / sizeof(Header) + 1;
    free_node = base = (Header *) heap_base;
    free_node->s.size = total_units;
    free_node->s.ptr = free_node;
}

/* malloc: general-purpose storage allocator */
void *malloc(size_t nbytes) {

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
        if (current_node->s.size >= nunits) /* big enough */
        {
            if (current_node->s.size == nunits) /* exactly */
            {
                prevptr->s.ptr = current_node->s.ptr;
            }
            else /* allocate tail end */
            {
                current_node->s.size -= nunits;
                current_node += current_node->s.size;
                current_node->s.size = nunits;
            }

            free_node = prevptr;
            result = current_node + 1;
            is_allocating = false; /* we are done */
        }

        if (current_node == free_node) /* wrapped around free list */
            return NULL;

        prevptr = current_node;
    } /* for */

    return result;
}

/* free: put block ap in free list */
void free(void *block) {
    if (block == NULL || (((long)block - (long)base) % sizeof(Header)) != 0) 
        return;

    Header *free_block, *current_node;
    free_block = (Header *)block - 1;                 /* point to block header */

    if (free_block->s.size < base || free_block->s.size >= (base + total_units * sizeof(Header))) {
        return;
    }

    /* the free space is only marked as free, but 'block' still points to it */
    /* to avoid reusing this address and corrupt our structure set it to '\0' */
    block = NULL;

    bool external = false;

    /* look where to insert the free space */

    /* (free_block > p && free_block < p->s.ptr)    => between two nodes */
    /* (p > p->s.ptr)               => this is the end of the list */
    /* (p == p->p.str)              => list is one element only */
    for (current_node = free_node; !(free_block > current_node && free_block < current_node->s.ptr); current_node = current_node->s.ptr) {
        if(free_block == current_node || free_block == current_node->s.ptr)
            return ;
        if (current_node >= current_node->s.ptr && (free_block > current_node || free_block < current_node->s.ptr))
            /* freed block at start or end of arena */
            external = true;
            break;
    }

    if (!external && (current_node + current_node->s.size > free_block || free_block + free_block->s.size > current_node->s.ptr))
            return;
        
    if (free_block + free_block->s.size == current_node->s.ptr) {      /* join to upper nbr */
    /* the new block fits perfect up to the upper neighbor */

        /* merging up: adjust the size */
        free_block->s.size += current_node->s.ptr->s.size;
        /* merging up: point to the second next */
        free_block->s.ptr = current_node->s.ptr->s.ptr;

    } else
        /* set the upper pointer */
        free_block->s.ptr = current_node->s.ptr;

    if (current_node + current_node->s.size == free_block) {              /* join to lower nbr */
    /* the new block fits perfect on top of the lower neighbor */

        /* merging below: adjust the size */
        current_node->s.size += free_block->s.size;
        /* merging below: point to the next */
        current_node->s.ptr = free_block->s.ptr;

    } else
        /* set the lower pointer */
        current_node->s.ptr = free_block;

    /* reset the start of the free list */
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