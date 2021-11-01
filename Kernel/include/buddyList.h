#ifndef BUDDY_LIST_H
#define BUDDY_LIST_H

#include <stdint.h>
#include <stdlib.h>

typedef struct list_t {
  uint8_t bucket;
  uint8_t free;
  struct list_t *prev, *next;
} list_t;

void listInitialize(list_t *list);
void listPush(list_t *list, list_t *entry);
void listRemove(list_t *entry);
list_t *listPop(list_t *list);
char listIsEmpty(list_t *list);

#endif