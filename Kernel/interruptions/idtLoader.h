#ifndef IDT_LOADER_H
#define IDT_LOADER_H

#include <stdint.h>

void configureIDT();
void irqDispatcher(int n);

#endif