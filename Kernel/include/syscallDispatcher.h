#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stdint.h>
#include <stdlib.h>

int syscallSelector(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx,
                    uint64_t r8, uint64_t r9);

#endif
