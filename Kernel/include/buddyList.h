#ifndef BUDDY_LIST_H
#define BUDDY_LIST_H

#include <stdlib.h>

typedef struct list_t {
  struct list_t *prev, *next;
} list_t;

void list_init(list_t *list);
void list_push(list_t *list, list_t *entry);
void list_remove(list_t *entry);
list_t *list_pop(list_t *list);
char list_is_empty(list_t *list);

#endif