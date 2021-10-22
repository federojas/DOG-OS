#include <stdint.h>
#include <priorityTest.h>
#include <userSyscalls.h>

#define TOTAL_PROCESSES 3

void testPriority(int argc, char argv[MAX_ARGUMENTS][BUFFER_SIZE]){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++) {
      char *argv[] = {"Idle Process"};
      pids[i] = newProcess(&idleProcess, 1, argv, BACKGROUND, NULL);
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  printf("\nCHANGING PRIORITIES...\n\nCHANGING PRIORITIES...\n\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        setPriority(pids[i], 1); //lowest priority 
        break;
      case 1:
        setPriority(pids[i], 25); //medium priority
        break;
      case 2:
        setPriority(pids[i], 50); //highest priority
        break;
    }
  }

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);

  printf("\nBLOCKING...\n\nBLOCKING...\n\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++) {
      blockProcess(pids[i]);
  }

  printf("\nCHANGING PRIORITIES WHILE BLOCKED...\n\nCHANGING PRIORITIES WHILE BLOCKED...\n\nCHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    setPriority(pids[i], 25);
  }

  printf("\nUNBLOCKING...\n\nUNBLOCKING...\n\nUNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    unblockProcess(pids[i]);

  busyWait(TOTAL_PROCESSES * MAJOR_WAIT);
  printf("\nKILLING...\n\nKILLING...\n\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    killProcess(pids[i]);
}
