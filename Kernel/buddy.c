//https://github.com/evanw/buddy-malloc/blob/master/buddy-malloc.c

#define MEMORY_MANAGER_BUDDY //BORRAR DESPUES, VA EN EL MAKEFILE
#ifdef MEMORY_MANAGER_BUDDY

#include <memoryManager.h>
#include <buddyList.h>
#include <stdint.h>
#include <stdlib.h>
#include <prints.h>

#define MIN_ALLOC_LOG2 4
#define MIN_ALLOC ((size_t)1 << MIN_ALLOC_LOG2)

#define MAX_ALLOC_LOG2 31

#define MAX_BUCKET_COUNT ( MAX_ALLOC_LOG2 - MIN_ALLOC_LOG2 + 1)




//tree levels
static list_t buckets[MAX_BUCKET_COUNT];
static size_t maximum_bucket_size;
static list_t * base_ptr;
static uint8_t buckets_amount;
static size_t minimum_bucket_size;

static void addNodeToBucket(list_t * bucketList, list_t * node, uint32_t bucketLevel);
static size_t getMinimumSuitableBucket(size_t request);
static size_t getAvailableBucket(uint8_t minBucketRequired);
static list_t *getNodeBuddy(list_t *node);
static list_t *getNodeAddress(list_t *node);

void initializeMemoryManager(char * heap_base, size_t heap_size) {
    if(heap_base == NULL) 
        return ;
    base_ptr = (list_t *) heap_base;
    maximum_bucket_size = heap_size;
    buckets_amount = (int)log2(heap_size) - MIN_ALLOC_LOG2 + 1;
    minimum_bucket_size = MIN_ALLOC;

    if (buckets_amount > MAX_BUCKET_COUNT) {
        int aux = pow(2, MAX_BUCKET_COUNT - 1);
        minimum_bucket_size = heap_size / aux;
        buckets_amount = MAX_BUCKET_COUNT;
    }
        

    for (int i = 0; i < buckets_amount; i++) {
        listInitialize(&buckets[i]);
        buckets[i].free = 0;
        buckets[i].bucket = i;
    }

    addNodeToBucket(&buckets[buckets_amount - 1], base_ptr, buckets_amount - 1);
}
void *malloc(size_t nbytes) {
    
    size_t bytesNeeded = nbytes + sizeof(list_t);

    if(nbytes == 0 || bytesNeeded > maximum_bucket_size)
        return NULL;

    uint8_t idealBucket =  getMinimumSuitableBucket(bytesNeeded);
    uint8_t availableBucket = getAvailableBucket(idealBucket);

    if(availableBucket == -1)
        return NULL;

    list_t * resultNode = listPop(&buckets[availableBucket]);

    for (; idealBucket < availableBucket; availableBucket--) {
        resultNode->bucket--;
        addNodeToBucket(&buckets[availableBucket - 1], getNodeBuddy(resultNode), availableBucket - 1);
    }

    resultNode->free = 0;

    return (void *)(resultNode + 1);
}


void free(void *block) {
    if (block == NULL)
        return;

    list_t * freeNode = (list_t *)block - 1;

    freeNode->free = 1;

    list_t *freeNodeBuddy = getNodeBuddy(freeNode);


    while (freeNode->bucket != buckets_amount - 1 && freeNodeBuddy->bucket == freeNode->bucket && freeNodeBuddy->free) {
        listRemove(freeNodeBuddy);
        freeNode = getNodeAddress(freeNode);
        freeNode->bucket++;
        freeNodeBuddy = getNodeBuddy(freeNode);
    }

    listPush(&buckets[freeNode->bucket], freeNode);
}

static void addNodeToBucket(list_t * bucketList, list_t * node, uint32_t bucketLevel) {
    node->bucket = bucketLevel;
    node->free = 1;
    listPush(bucketList, node);
}

static size_t getMinimumSuitableBucket(size_t request) {
  size_t bucket = buckets_amount - 1;
  size_t size = minimum_bucket_size;

  while (size < request) {
    bucket--;
    size *= 2;
  }

  return bucket;
}

static size_t getAvailableBucket(uint8_t minBucketRequired) {
    uint8_t availableBucket;

    for (availableBucket = minBucketRequired; availableBucket < buckets_amount && listIsEmpty(&buckets[availableBucket]); availableBucket++);

    if (availableBucket >= buckets_amount)
        return -1;

    return availableBucket;
}

static list_t *getNodeBuddy(list_t *node) {
    uint8_t bucket = node->bucket;
    uintptr_t nodeCurrentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
    uintptr_t nodeNewOffset = nodeCurrentOffset ^ (1 << (log2(minimum_bucket_size) + bucket) );

    return (list_t *)((uintptr_t)base_ptr + nodeNewOffset);
}

 void memDump(){
    list_t *list, *aux;
    uint32_t idx=0;
    uint32_t spaceAvail=0;

    printf("\n MEMORY DUMP (buddy)\n");
    printf("\n----------------------------------------------------------");
    printf("Buckets with free blocks:\n");
    
    for(int i=buckets_amount-1;i>=0;i--){
        list=&bucketList[i];
        if(!listIsEmpty(list)){
            printf("bucket %d\n", i+MIN_ALLOC_LOG2);
            printf("free buckets of size 2^%d\n", i+MIN_ALLOC_LOG2)
        }
        for(aux=list->next, idx=1; aux!=list;idx++, aux=aux->next){
            //esto no se si despues lo podemos pasar a una funcion si es que necesitamos usarlo en otro lado 
            print("        Block number: %d\n", idx);
            print("            state: free\n");
            spaceAvail+=idx*(1 << (MIN_ALLOC_LOG2+i);  
        }
        printf("------------------------------------------------------\n");
    }
    printf("Available space: %d\n", spaceAvail);
    
    
 }

static list_t *getNodeAddress(list_t *node) {
      uint8_t bucket = node->bucket;
      uintptr_t mask = (1 << (log2(minimum_bucket_size) + bucket) );
      mask = ~mask;

      uintptr_t nodeCurrentOffset = (uintptr_t)node - (uintptr_t)base_ptr;
      uintptr_t nodeNewOffset = nodeCurrentOffset & mask;

      return (list_t *)(nodeNewOffset + (uintptr_t)base_ptr);
}


#endif