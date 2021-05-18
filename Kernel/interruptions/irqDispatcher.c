#include <time.h>
#include <keyboardDriver.h>
#include <stdint.h>

#define TIMER 0
#define KEYBOARD 1

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case TIMER:
			int_20();
			break;
		case KEYBOARD:
			int_21();
			break;
	}
	return;
}

static void int_20() {
	timer_handler();
}

static void int_21() {
    keyboardHandler();
}
