#include <memoryTest.h>
#include <userSyscalls.h>

#define MAX_BLOCKS 128
#define MAX_MEMORY 32 * 1024 * 1024

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void testMemory() {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  printf("Initializing Memory Testing\n");

  while (1){
    rq = 0;
    total = 0;

    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc((uint64_t) mm_rqs[rq].size);
      if(mm_rqs[rq].address == NULL) {
        printf("No memory left\n");
        return ;
      }
      total += mm_rqs[rq].size;
      rq++;
    }
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        memset(mm_rqs[i].address, i, mm_rqs[i].size); 
      }
        

    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("ERROR!\n");
        }
          

    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        free(mm_rqs[i].address);
      }
      
  } 
}
