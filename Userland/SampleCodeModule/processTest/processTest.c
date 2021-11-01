// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <processTest.h>
#include <userSyscalls.h>

#define MAX_PROCESSES 10

enum State { RUNNING, WAITING, KILLED };

typedef struct P_rq {
  uint32_t pid;
  enum State state;
} p_rq;

void testProcesses() {
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  while (1) {
    for (rq = 0; rq < MAX_PROCESSES; rq++) {
      char *argv[] = {"Idle Process"};
      p_rqs[rq].pid = newProcess(&idleProcess, 1, argv, BACKGROUND, NULL);
      if (p_rqs[rq].pid == -1) {
        printf("Error creating process\n");
        return;
      } else {
        p_rqs[rq].state = RUNNING;
        alive++;
      }
    }

    while (alive > 0) {
      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        action = GetUniform(2) % 2;
        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == WAITING) {
              if (killProcess(p_rqs[rq].pid) == -1) {
                printf("Error killing process\n");
                return;
              }
              p_rqs[rq].state = KILLED;
              alive--;
              busyWait(MAJOR_WAIT);
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING) {
              if (blockProcess(p_rqs[rq].pid) == -1) {
                printf("Error blocking process\n");
                return;
              }
              p_rqs[rq].state = WAITING;
            }
            break;
        }
      }
      for (rq = 0; rq < MAX_PROCESSES; rq++) {
        if (p_rqs[rq].state == WAITING && GetUniform(2) % 2) {
          if (unblockProcess(p_rqs[rq].pid) == -1) {
            printf("Error unblocking process\n");
            return;
          }
          p_rqs[rq].state = RUNNING;
        }
      }
    }
  }
}
